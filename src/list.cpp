#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"

ListCtorVal ListCtor(list_t *list, int start_capa)
{
    assert(list);

    if (start_capa <= 0)
    {
        fprintf(stderr, "start_capa <= 0!\n");
        return CTOR_ERR;
    }

    list->capacity = start_capa;
    list->head     = 0;
    list->tail     = 0;
    list->free     = 1;

    // list->capacity =+ ((start_capa > BASE_LIST_CAPA) ? start_capa : BASE_LIST_CAPA);
    list->data = (ListElem_t *) calloc(list->capacity, sizeof(ListElem_t));
    list->next = (int *)        calloc(list->capacity, sizeof(int));
    list->prev = (int *)        calloc(list->capacity, sizeof(int));

    for (int i = 1; i < list->capacity - 1; i++)     // св€зный список свободных €чеек
    {
        list->next[i] = i + 1;
    }

    if (list->capacity > 0)
        list->next[list->capacity - 1] = 0;             // последн€€ свободна€ €чейка

    for (int i = 0; i < list->capacity; i++)
    {
        list->prev[i] = PREV_POISON;
    }

    list->error = 0;

    LIST_ASSERT(list);

    return CTOR_OK;
}

ListDtorVal ListDtor(list_t *list)
{
    LIST_ASSERT(list);

    free(list->data);
    free(list->next);
    free(list->prev);

    list->capacity = 0;
    list->head     = 1;
    list->tail     = 1;
    list->free     = 1;

    return DTOR_OK;
}

ListElem_t *GetHeadVal(list_t *list)
{
    LIST_ASSERT(list);

    return &list->data[list->head];
}

ListElem_t *GetTailVal(list_t *list)
{
    LIST_ASSERT(list);

    return &list->data[list->tail];
}

void ListPasteTail(list_t *list, ListElem_t elem)
{
    LIST_ASSERT(list);

    int free_cell_num = list->free;
    list->free = list->next[list->free];

    list->next[list->tail]    = free_cell_num;
    list->next[free_cell_num] = 0;

    if (list->tail == 0)
        list->head = free_cell_num;
        
    list->tail = free_cell_num;

    list->data[free_cell_num] = elem;

    LIST_ASSERT(list);
}

void ListPasteHead(list_t *list, ListElem_t elem)
{
    LIST_ASSERT(list);

    int free_cell_num = list->free;
    list->free = list->next[list->free];

    list->next[free_cell_num] = list->head;

    if (list->head == 0)
        list->tail = free_cell_num;

    list->head = free_cell_num;

    list->data[free_cell_num] = elem;

    LIST_ASSERT(list);
}

void ListPasteAfter(list_t *list, ListElem_t elem, int elem_num)
{
    assert(elem_num > 0);
    LIST_ASSERT(list);

    int free_cell_num = list->free;
    list->free = list->next[list->free];

    list->next[free_cell_num] = list->next[elem_num];
    list->next[elem_num] = free_cell_num;

    list->data[free_cell_num] = elem;

    LIST_ASSERT(list);
}

// void ListDelElem(list_t *list, int elem_num)
// {
//     assert(list);
//     assert(elem_num > 0);


// }