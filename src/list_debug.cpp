#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "list.h"

FILE *LogFile = NULL;

void ListAssert(list_t *list, const char *file, int line, const char *func)
{
    int error = ListVerify(list);

    if (list != NULL)
        list->error |= error;

    if (error != LIST_OK)
    {
        fprintf(stderr, "my assertion failed in\t%s:%d\t(%s)\nErrors:\t", file, line, func);
        PrintListErr(error);
        assert(0);
    }
}

void ListSegfaultAssert(list_t *list, int index, const char *file, int line, const char *func)
{
    int *next = list->next;
    int  free = list->free;

    for (int i = free; i != END_OF_FREE; i = next[i])
    {
        if (i == index)
        {
            fprintf(stderr, "\nsegfault in\t%s:%d\t(%s)\n", file, line, func);
            fprintf(stderr, "accessing a non-existent list item with an index = %d\n", index);
            assert(0);
        }
    }
}

void PrintListErr(int error)
{   
    #define PRINT_ERROR(err, code)                      \
    if (err & code)                                     \
    {                                                   \
        fprintf(stderr, #code);                         \
        fprintf(stderr, " ");                           \
    }                                                   
    
    PRINT_ERROR (error, LIST_PTR_ERR);
    PRINT_ERROR (error, LIST_DATA_PTR_ERR);
    PRINT_ERROR (error, LIST_HEAD_UNDERFLOW);
    PRINT_ERROR (error, LIST_HEAD_OVERFLOW);
    PRINT_ERROR (error, LIST_TAIL_UNDERFLOW);
    PRINT_ERROR (error, LIST_TAIL_OVERFLOW);
    PRINT_ERROR (error, LIST_FREE_UNDERFLOW);
    PRINT_ERROR (error, LIST_FREE_OVERFLOW);

    #undef PRINT_ERROR  

    printf("\n");
}

int ListVerify(list_t *list)
{
    int res_err = 0;

    if (list == NULL)
        return LIST_PTR_ERR;

    if (list->data == NULL)
        res_err |= LIST_DATA_PTR_ERR;

    if (list->head >= list->capacity)
        res_err |= LIST_HEAD_OVERFLOW;

    if (list->head < 0)
        res_err |= LIST_HEAD_UNDERFLOW;

    if (list->tail >= list->capacity)
        res_err |= LIST_TAIL_OVERFLOW;

    if (list->tail < 0)
        res_err |= LIST_TAIL_UNDERFLOW;    
        
    if (list->free >= list->capacity)
        res_err |= LIST_FREE_OVERFLOW;

    if (list->free < 1)
        res_err |= LIST_FREE_UNDERFLOW;

    return res_err;
}

char *GetFilePath(const char *name, const char *folder, char *path)
{
    sprintf(path, "%s%s", folder, name);
    return path;
}

FILE *OpenLogFile(const int argc, const char *argv[])
{
    char logfile_name[PATH_NAME_LEN] = {};

    if (argc < 2)
        GetFilePath(BASE_LOGFILE_NAME, LOGFILE_FOLDER, logfile_name);

    else
        GetFilePath(argv[1], LOGFILE_FOLDER, logfile_name);

    LogFile = fopen(logfile_name, "w");

    fprintf(LogFile, "<html>                                            \n"
                            "\t<head>                                   \n"
                            "\t<title>List Logs</title>                 \n"
                            "\t<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css\"> \n"
                            "\t</head>                                  \n"
                            "\t<body>                                   \n"
                            "\t<title>List Logs</title>                 \n"
                            "\t<div class=\"jumbotron text-center\">    \n"
                            "\t\t<h1>List logs</h1>                     \n"
                            "\t</div>                                   \n"
                            "\t<pre>                                    \n");
    atexit(CloseLogFile);

    return LogFile;
}

void CloseLogFile(void)
{
    fprintf(LogFile,    "\t\t</pre>     \n"
                        "\t</body       \n"
                        "</html>");
            
    fprintf(stderr, "CLOSE_LOG_FILE\n");
    fclose(LogFile);
}

void ListDump(list_t *list, const char *file, int line, const char *func)
{
    fprintf(stderr, "start of dump\n");

    fprintf(LogFile, "  LIST_DUMP called from %s:%d  (%s)\n  {\n", file, line, func);

    fprintf(LogFile, "\thead = %d\n\ttail = %d\n\tfree = %d\n\tcapacity = %d\n\n",
                       list->head, list->tail, list->free, list->capacity);

// list
    fprintf(LogFile, "\tlist [%p]:   ", list->prev);

    int num = list->head;

    while (num != 0)
    {
        // fprintf(stderr, "data[%d] = %3" LIST_ELEM_FORMAT " \n", num, list->data[num]);
        fprintf(LogFile, "%3" LIST_ELEM_FORMAT " ", list->data[num]);
        num = list->next[num];
    } 

// table
    fprintf(LogFile, "</pre>\n<table border width = \"85%\"style=\"margin-left: 3%\">\n");

    fprintf(LogFile, "<tr>\n");

    fprintf(LogFile, "<td>index</td>");

    for (int i = 0; i < list->capacity; i++)
        fprintf(LogFile, "<td>%d</td>", i);

    fprintf(LogFile, "</tr>\n");

// data
    fprintf(LogFile, "<tr>\n");

    // fprintf(LogFile, "\n\n\tdata [%p]:", list->data);
    fprintf(LogFile, "<td>data [%p]:</td>", list->data);

    fprintf(LogFile, "\t");

    for (int i = 0; i < list->capacity; i++)
    {
        fprintf(LogFile, "<td>");

        if (list->data[i] == DATA_POISON)
            fprintf(LogFile, "DT#");
        
        else
            fprintf(LogFile, "%3d", list->data[i]);

        fprintf(LogFile, "</td>\n");   
    }

    fprintf(LogFile, "</tr>\n");

// next
    fprintf(LogFile, "<tr>\n");

    // fprintf(LogFile, "\n\tnext [%p]:", list->next);
    fprintf(LogFile, "<td>next [%p]:</td>", list->next);

    fprintf(LogFile, "\t");

    for (int i = 0; i < list->capacity; i++)
    {
        fprintf(LogFile, "<td>");

        if (list->next[i] == NEXT_POISON)
            fprintf(LogFile, "NX# ");

        if (list->next[i] == END_OF_FREE)
            fprintf(LogFile, END_OF_FREE_MARK);

        else
            fprintf(LogFile, "%3d ", list->next[i]);

        fprintf(LogFile, "</td>\n");
    }

    fprintf(LogFile, "</tr>\n");

// prev
    fprintf(LogFile, "<tr>\n");

    fprintf(LogFile, "<td>prev [%p]:</td>", list->prev);

    fprintf(LogFile, "\t");

    for (int i = 0; i < list->capacity; i++)
    {
        fprintf(LogFile, "<td>");

        if (list->prev[i] == PREV_POISON)
            fprintf(LogFile, "PR# ");

        else
            fprintf(LogFile, "%3d ", list->prev[i]);

        fprintf(LogFile, "</td>\n");
    }

    fprintf(LogFile, "</tr>\n");

    fprintf(LogFile, "</table>\n<pre>\n");


    fprintf(LogFile, "\n\n\tGraph\n");
    MakeGraph(list);

    fprintf(LogFile, "<img src = " GRAPH_FOLDER  "%s width = \"85%\" style=\"margin-left: 3%\">", list->graphs.data[list->graphs.index - 1].name);
    fprintf(stderr, "<img src = " GRAPH_FOLDER  "%s width = \"85%\" style=\"margin-left: 3%\">", list->graphs.data[list->graphs.index - 1].name);

    fprintf(LogFile, "\n  }\n\n");


    fprintf(stderr, "end of dump\n");
}