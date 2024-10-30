#include <stdio.h>
#include <stdlib.h>

#include "list.h"

void ListAssert(list_t *list, const char *file, int line, const char *func)
{
    int error = ListVerify(list);

    if (list == NULL)
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

    if (list->data == NULL)
        return LIST_PTR_ERR;

    if (list->data == NULL)
        res_err |= DATA_PTR_ERR;

    if (list->head >= list->capacity)
        res_err |= HEAD_OVERFLOW;

    if (list->head < 1)
        res_err |= HEAD_UNDERFLOW;

    if (list->tail >= list->capacity)
        res_err |= TAIL_OVERFLOW;

    if (list->tail < 1)
        res_err |= TAIL_UNDERFLOW;    
        
    if (list->free >= list->capacity)
        res_err |= FREE_OVERFLOW;

    if (list->free < 1)
        res_err |= FREE_UNDERFLOW;

    return res_err;
}