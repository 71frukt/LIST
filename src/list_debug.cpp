#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "list.h"


#ifdef LIST_DEBUG
FILE *ListLogFile = NULL;

void ListAssert(list_t *list, const char *file, int line, const char *func)
{
    int error = ListVerify(list);

    if (list != NULL)
        list->error |= error;

    if (list->error != LIST_OK)
    {
        fprintf(stderr, "my assertion failed in\t%s:%d\t(%s)\nErrors:\t", file, line, func);
        PrintListErr(list->error);

        CloseLogFile();
        assert(0);
    }
}

void ListFreeUsageInData(list_t *list, int index, const char *file, int line, const char *func)
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

ListFuncStatus ListConnectionsVerify(list_t *list)
{
    int counter = 0;
    int i = 0;
    do
    {
        if (i != list->next[list->prev[i]] || counter >= list->capacity)
            return LIST_FUNC_ERR;

        i = list->next[i];

        counter++;
    }
    while (i != 0);

    return LIST_FUNC_OK;
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
    PRINT_ERROR (error, LIST_CONNECTIONS_ERR);

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

    if (ListConnectionsVerify(list) != LIST_FUNC_OK)
        res_err |= LIST_CONNECTIONS_ERR;

    return res_err;
}

char *GetFilePath(const char *name, const char *folder, char *path)
{
    sprintf(path, "%s%s", folder, name);
    return path;
}

FILE *OpenLogFile()
{
    system("mkdir -p build/logs");

    char logfile_name[PATH_NAME_LEN] = {};

    GetFilePath(LOGFILE_NAME, LOGFILE_FOLDER, logfile_name);

    ListLogFile = fopen(logfile_name, "w");

    fprintf(ListLogFile, "<html>                                                                                                    \n"
                    "\t<head>                                                                                                   \n"
                    "\t<title>List Logs</title>                                                                                 \n"
                    "\t<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css\"> \n"
                    "\t</head>                                                                                                  \n"
                    "\t<body>                                                                                                   \n"
                    "\t<title>List Logs</title>                                                                                 \n"
                    "\t<div class=\"jumbotron text-center\">                                                                    \n"
                    "\t\t<h1>List logs</h1>                                                                                     \n"
                    "\t</div>                                                                                                   \n"
                    "\t<pre>                                                                                                    \n");
    atexit(CloseLogFile);

    return ListLogFile;
}

void CloseLogFile(void)
{
    fprintf(ListLogFile,    "\t\t</pre>     \n"
                        "\t</body       \n"
                        "</html>");
            
    fprintf(stderr, "CLOSE_LOG_FILE\n");
    fclose(ListLogFile);
}

void ListDump(list_t *list, const char *file, int line, const char *func)
{
    fprintf(stderr, "start of dump\n");

    fprintf(ListLogFile, "  LIST_DUMP called from %s:%d  (%s)\n  {\n", file, line, func);

    fprintf(ListLogFile, "\thead = %d\n\ttail = %d\n\tfree = %d\n\tcapacity = %d\n\n",
                       list->head, list->tail, list->free, list->capacity);

// list
    fprintf(ListLogFile, "\tlist [%p]:   ", list->prev);

    int num = list->head;

    while (num != 0)
    {
        // fprintf(stderr, "data[%d] = %3" LIST_ELEM_FORMAT " \n", num, list->data[num]);
        fprintf(ListLogFile, "%3" LIST_ELEM_FORMAT " ", list->data[num]);
        num = list->next[num];
    } 

// table
    fprintf(ListLogFile, "</pre>\n<table border width = \"85%%\"style=\"margin-left: 3%%\">\n");

    fprintf(ListLogFile, "<tr>\n");

    fprintf(ListLogFile, "<td>index</td>");

    for (int i = 0; i < list->capacity; i++)
        fprintf(ListLogFile, "<td>%d</td>", i);

    fprintf(ListLogFile, "</tr>\n");

// data
    fprintf(ListLogFile, "<tr>\n");

    // fprintf(ListLogFile, "\n\n\tdata [%p]:", list->data);
    fprintf(ListLogFile, "<td>data [%p]:</td>", list->data);

    fprintf(ListLogFile, "\t");

    for (int i = 0; i < list->capacity; i++)
    {
        fprintf(ListLogFile, "<td>");

        if (list->data[i] == DATA_POISON)
            fprintf(ListLogFile, "DT#");
        
        else
            fprintf(ListLogFile, "%3d", list->data[i]);

        fprintf(ListLogFile, "</td>\n");   
    }

    fprintf(ListLogFile, "</tr>\n");

// next
    fprintf(ListLogFile, "<tr>\n");

    // fprintf(ListLogFile, "\n\tnext [%p]:", list->next);
    fprintf(ListLogFile, "<td>next [%p]:</td>", list->next);

    fprintf(ListLogFile, "\t");

    for (int i = 0; i < list->capacity; i++)
    {
        fprintf(ListLogFile, "<td>");

        if (list->next[i] == NEXT_POISON)
            fprintf(ListLogFile, "NX# ");

        if (list->next[i] == END_OF_FREE)
            fprintf(ListLogFile, END_OF_FREE_MARK);

        else
            fprintf(ListLogFile, "%3d ", list->next[i]);

        fprintf(ListLogFile, "</td>\n");
    }

    fprintf(ListLogFile, "</tr>\n");

// prev
    fprintf(ListLogFile, "<tr>\n");

    fprintf(ListLogFile, "<td>prev [%p]:</td>", list->prev);

    fprintf(ListLogFile, "\t");

    for (int i = 0; i < list->capacity; i++)
    {
        fprintf(ListLogFile, "<td>");

        if (list->prev[i] == PREV_POISON)
            fprintf(ListLogFile, "PR# ");

        else
            fprintf(ListLogFile, "%3d ", list->prev[i]);

        fprintf(ListLogFile, "</td>\n");
    }

    fprintf(ListLogFile, "</tr>\n");

    fprintf(ListLogFile, "</table>\n<pre>\n");

    #ifdef LIST_GRAPH_DEBUG
    fprintf(ListLogFile, "\n\n\tGraph\n");
    MakeGraph(list);

    fprintf(ListLogFile, "<img src = " GRAPH_FOLDER  "%s width = \"%d%%\" style=\"margin-left: 3%%\">", list->graphs.data[list->graphs.index - 1].name, GRAPH_PICTURE_WIDTH);
    fprintf(stderr,  "<img src = " GRAPH_FOLDER  "%s width = \"%d%%\" style=\"margin-left: 3%%\">", list->graphs.data[list->graphs.index - 1].name, GRAPH_PICTURE_WIDTH);
    #endif

    fprintf(ListLogFile, "\n  }\n\n");

    fprintf(stderr, "end of dump\n");
}

#endif