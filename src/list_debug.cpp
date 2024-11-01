#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

FILE *LogFile;

void ListAssert(list_t *list, const char *file, int line, const char *func)
{
    int error = ListVerify(list);

    if (list != NULL)
        list->error |= error;
    if (error != LIST_OK)
    {
        fprintf(stderr, "my assertion failed in\t%s:%d\t(%s)\nErrors:\t", file, line, func);
        PrintListErr(error);
        abort();
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
    PRINT_ERROR (error, DATA_PTR_ERR);
    PRINT_ERROR (error, HEAD_UNDERFLOW);
    PRINT_ERROR (error, HEAD_OVERFLOW);
    PRINT_ERROR (error, TAIL_UNDERFLOW);
    PRINT_ERROR (error, TAIL_OVERFLOW);
    PRINT_ERROR (error, FREE_UNDERFLOW);
    PRINT_ERROR (error, FREE_OVERFLOW);

    #undef PRINT_ERROR  

    printf("\n");
}

int ListVerify(list_t *list)
{
    int res_err = 0;

    if (list == NULL)
        return LIST_PTR_ERR;

    if (list->data == NULL)
        res_err |= DATA_PTR_ERR;

    if (list->head >= list->capacity)
        res_err |= HEAD_OVERFLOW;

    if (list->head < 0)
        res_err |= HEAD_UNDERFLOW;

    if (list->tail >= list->capacity)
        res_err |= TAIL_OVERFLOW;

    if (list->tail < 0)
        res_err |= TAIL_UNDERFLOW;    
        
    if (list->free >= list->capacity)
        res_err |= FREE_OVERFLOW;

    if (list->free < 1)
        res_err |= FREE_UNDERFLOW;

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

    return LogFile;

    atexit(CloseLogFile);
}

void CloseLogFile(void)
{
    fclose(LogFile);
}

void ListDump(list_t *list, const char *file, int line, const char *func)
{
    fprintf(LogFile, "LIST_DUMP called from %s:%d  (%s)\n{\n", file, line, func);

    fprintf(LogFile, "\thead = %d\n\ttail = %d\n\tfree = %d\n\tcapacity = %d\n\n",
                       list->head, list->tail, list->free, list->capacity);

    fprintf(LogFile, "\t\t\t\t\t\t�\t\t");

    for (int i = 0; i < list->capacity; i++)
        fprintf(LogFile, "%3d ", i);

// data
    fprintf(LogFile, "\n\n\tdata [%p]:", list->data);

    fprintf(LogFile, "\t");

    for (int i = 0; i < list->capacity; i++)
    {
        if (list->data[i] == DATA_POISON)
            fprintf(LogFile, "DT# ");
        
        else
            fprintf(LogFile, "%3d ", list->data[i]);
    }

// next
    fprintf(LogFile, "\n\tnext [%p]:", list->next);

    fprintf(LogFile, "\t");

    for (int i = 0; i < list->capacity; i++)
    {
        if (list->next[i] == NEXT_POISON)
            fprintf(LogFile, "NX# ");

        else
            fprintf(LogFile, "%3d ", list->next[i]);
    }

// prev
    fprintf(LogFile, "\n\tprev [%p]:", list->prev);

    fprintf(LogFile, "\t");

    for (int i = 0; i < list->capacity; i++)
    {
        if (list->prev[i] == PREV_POISON)
            fprintf(LogFile, "PR# ");

        else
            fprintf(LogFile, "%3d ", list->prev[i]);
    }

// list
    fprintf(LogFile, "\n\n\tlist [%p]:", list->prev);

    fprintf(LogFile, "\t");

    int num = list->head;

    while (num != 0)
    {
        fprintf(LogFile, "%3d ", list->data[num]);
        num = list->next[num];
    }

    fprintf(LogFile, "\n}\n\n");

    MakeGraph(list);
}