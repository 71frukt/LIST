#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "list.h"

ListFuncStatus ListCtor(list_t *list, int start_capa, int item_size)
{
    assert(list);
    assert(start_capa > 0 && "start_capa =< 0 in ListCtor!");

    list->item_size = item_size;
    list->capacity  = start_capa + 1;

    // list->capacity = ((start_capa > BASE_LIST_CAPA) ? start_capa : BASE_LIST_CAPA);
    list->data = (void *) calloc(list->capacity, item_size);
    list->next = (int  *) calloc(list->capacity, sizeof(int));
    list->prev = (int  *) calloc(list->capacity, sizeof(int));

    list->free     = 1;

    list->next[0]  = 0;                                         //aka head
    list->prev[0]  = 0;                                         //aka tail
    // list->data[0]  = NULL;

    list->head     = list->next[0];
    list->tail     = list->prev[0];

    for (int i = 1; i < list->capacity - 1; i++) 
    {
        list->next[i] = i + 1;
    }

    if (list->capacity > 0)
        list->next[list->capacity - 1] = END_OF_FREE;

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

ListFuncStatus ListRealloc(list_t *list, int new_capa)
{
    assert(list);
    assert(new_capa > 0 && "new_capa =< 0 in ListRecalloc!");
fprintf(stderr, "LIST_REALLOC!\n\n");
    list->capacity = new_capa + 1;

    list->data = (void **) realloc(list->data, list->capacity * list->item_size);
    list->next = (int *)        realloc(list->next, list->capacity * sizeof(int));
    list->prev = (int *)        realloc(list->prev, list->capacity * sizeof(int));


    for (int i = list->free; i < list->capacity - 1; i++) 
        list->next[i] = i + 1;

    if (list->capacity > 0)
        list->next[list->capacity - 1] = END_OF_FREE;

    LIST_ASSERT(list);
    return LIST_FUNC_OK;
}

void *GetHeadVal(list_t *list)
{
    LIST_ASSERT(list);

    return (void *) ((char *) list->data + list->head * list->item_size);
}

void *GetTailVal(list_t *list)
{
    LIST_ASSERT(list);

    return (void *) ((char *) list->data + list->tail * list->item_size);
}

void *ListGetItem(list_t *list, int item_num)
{
    return (void *) ((char *) list->data + item_num * list->item_size);
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

ListFuncStatus ListPasteHead(list_t *list, void *item)
{
    LIST_ASSERT(list);

    ListPasteAfter(list, item, 0);

    return LIST_FUNC_OK;
}

ListFuncStatus ListPasteTail(list_t *list, void *item)
{
    LIST_ASSERT(list);

    ListPasteAfter(list, item, list->tail);

    return LIST_FUNC_OK;
}

ListFuncStatus ListPasteAfter(list_t *list, void *item, int item_num)
{
    LIST_ASSERT(list);
    LIST_FREE_USAGE_ASSERT(list, item_num);
    assert(item_num >= 0);

fprintf(stderr, "\n\nin paste after\n");

    if (list->size >= list->capacity - 1)
        ListRealloc(list, list->capacity * 2);

    int free_cell_num = list->free;
    list->free = list->next[list->free];

    ListBind(list, free_cell_num, list->next[item_num]);    // queue is important !
    ListBind(list, item_num,      free_cell_num);

    list->head = list->next[0];
    list->tail = list->prev[0];

    memcpy(ListGetItem(list, item_num), item, list->item_size);
    // list->data[free_cell_num] = item;

    list->size++;

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

ListFuncStatus ListDelElem(list_t *list, int item_num)
{
    LIST_ASSERT(list);
    assert(item_num > 0);

    ON_LIST_DEBUG (
        list->data[item_num] = DATA_POISON;
    )

    int prev_free = list->free;
    list->free = item_num;
    ListBind(list, list->prev[item_num], list->next[item_num]);     // queue is important !
    ListBind(list, list->free, prev_free);

    list->head = list->next[0];
    list->tail = list->prev[0];

    list->size--;

    LIST_ASSERT(list);
    LIST_DUMP(list);

    return LIST_FUNC_OK;
}
