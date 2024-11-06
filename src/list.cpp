#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"

ListFuncStatus ListCtor(list_t *list, int start_capa)
{
    assert(list);
    assert(start_capa > 0 && "start_capa =< 0 in ListCtor!");

    list->capacity = start_capa + 1;        // нулевой элемент - управл€ющий

    // list->capacity =+ ((start_capa > BASE_LIST_CAPA) ? start_capa : BASE_LIST_CAPA);
    list->data = (ListElem_t *) calloc(list->capacity, sizeof(ListElem_t));
    list->next = (int *)        calloc(list->capacity, sizeof(int));
    list->prev = (int *)        calloc(list->capacity, sizeof(int));

    list->free     = 1;

    list->next[0]  = 0;                                         //aka head
    list->prev[0]  = 0;                                         //aka tail
    list->data[0]  = DATA_POISON;

    list->head     = list->next[0];
    list->tail     = list->prev[0];

    for (int i = 1; i < list->capacity - 1; i++)                // св€зный список свободных €чеек
    {
        list->next[i] = i + 1;
    }

    if (list->capacity > 0)
        list->next[list->capacity - 1] = END_OF_FREE;           // последн€€ свободна€ €чейка

    ON_LIST_DEBUG 
    (
        OpenLogFile();

        for (int i = 1; i < list->capacity; i++)
        {
            list->data[i] = DATA_POISON;
            list->prev[i] = PREV_POISON;
        } 

        GraphsCtor(&list->graphs);

        for (size_t i = 0; i < list->graphs.size; i++)
        {
            list->graphs.data[i].nodes_count = list->capacity;
        }
    );

    LIST_ASSERT(list);
    LIST_DUMP(list);

    return LIST_FUNC_OK;
}

ListFuncStatus ListDtor(list_t *list)
{
    LIST_ASSERT(list);
    LIST_DUMP(list);

    free(list->data);
    free(list->next);
    free(list->prev);

    ON_LIST_DEBUG (
        GraphsDtor(&list->graphs); 
    );

    list->capacity = 0;
    list->free     = 1;
    list->head     = 0;
    list->tail     = 0;

    return LIST_FUNC_OK;
}

ListElem_t GetHeadVal(list_t *list)
{
    LIST_ASSERT(list);

    return list->data[list->head];
}

ListElem_t GetTailVal(list_t *list)
{
    LIST_ASSERT(list);

    return list->data[list->tail];
}

int GetNumInData(list_t *list, int num_in_list)
{
    LIST_ASSERT(list);
    assert(num_in_list >= 0);
    assert(num_in_list < list->capacity);

    int num_in_data = list->head;

    for (int i = 0; i < num_in_list; i++)
    {
        num_in_data = list->next[num_in_data];
    }

    return num_in_data;
}

ListFuncStatus ListPasteHead(list_t *list, ListElem_t elem)
{
    LIST_ASSERT(list);

    ListPasteAfter(list, elem, 0);

    return LIST_FUNC_OK;
}

ListFuncStatus ListPasteTail(list_t *list, ListElem_t elem)
{
    LIST_ASSERT(list);

    ListPasteAfter(list, elem, list->tail);

    return LIST_FUNC_OK;
}

ListFuncStatus ListPasteAfter(list_t *list, ListElem_t elem, int elem_num)
{
    LIST_ASSERT(list);
    LIST_DREE_USAGE_ASSERT(list, elem_num);
    assert(elem_num >= 0);

fprintf(stderr, "\n\nin paste after\n");

    int free_cell_num = list->free;
    list->free = list->next[list->free];

    ListBind(list, free_cell_num, list->next[elem_num]);    // queue is important !
    ListBind(list, elem_num,      free_cell_num);

    list->head = list->next[0];
    list->tail = list->prev[0];

    list->data[free_cell_num] = elem;
    LIST_ASSERT(list);
    LIST_DUMP(list);
    return LIST_FUNC_OK;
}

ListFuncStatus ListBind(list_t *list, int prev_el_num, int next_el_num)
{
    LIST_ASSERT(list);
    assert(prev_el_num >= 0);
    assert(next_el_num >= 0);

    list->next[prev_el_num] = next_el_num;
    list->prev[next_el_num] = prev_el_num;

    LIST_ASSERT(list);
    return LIST_FUNC_OK;
}

ListFuncStatus ListDelElem(list_t *list, int elem_num)
{
    LIST_ASSERT(list);
    assert(elem_num > 0);

    ON_LIST_DEBUG (
        list->data[elem_num] = DATA_POISON;
    )

    int prev_free = list->free;                                     // добавить €чейку к списку пустых
    list->free = elem_num;
    ListBind(list, list->prev[elem_num], list->next[elem_num]);     // queue is important !
    ListBind(list, list->free, prev_free);

    list->head = list->next[0];
    list->tail = list->prev[0];

    LIST_ASSERT(list);
    LIST_DUMP(list);

    return LIST_FUNC_OK;
}