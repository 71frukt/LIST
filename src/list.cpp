#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"

ListFuncStatus ListCtor(list_t *list, int start_capa)
{
    assert(list);

    if (start_capa <= 0)
    {
        fprintf(stderr, "start_capa <= 0!\n");
        return LIST_FUNC_ERR;
    }

    list->capacity = start_capa;

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

    for (int i = 1; i < list->capacity - 1; i++)                // ������� ������ ��������� �����
    {
        list->next[i] = i + 1;
    }

    if (list->capacity > 0)
        list->next[list->capacity - 1] = END_OF_FREE;             // ��������� ��������� ������

    ON_LIST_DEBUG 
    (
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

    // return list->data[list->next[0]];
    return list->data[list->head];
}

ListElem_t GetTailVal(list_t *list)
{
    LIST_ASSERT(list);

    // return list->data[list->prev[0]];
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

    /*
    int free_cell_num = list->free;
    list->free = list->next[list->free];

    // list->next[free_cell_num] = list->head;

    if (list->head == 0)                // ���� ������ ������� ������
    {
        list->tail = free_cell_num;
    }
    
    ListBind(list, free_cell_num, list->head);
    
    list->head = free_cell_num;

    list->data[free_cell_num] = elem;

    LIST_ASSERT(list);
    LIST_DUMP(list);
    fprintf(stderr, "end of paste head\n");
    return LIST_FUNC_OK;
    */
}

ListFuncStatus ListPasteTail(list_t *list, ListElem_t elem)
{
    LIST_ASSERT(list);

    ListPasteAfter(list, elem, list->tail);

    return LIST_FUNC_OK;
    /*
    int free_cell_num = list->free;
    list->free = list->next[list->free];

    // list->next[list->tail]    = free_cell_num;
    // list->next[free_cell_num] = 0;

    if (list->tail == 0)                // ���� ������ ������� ������
        list->head = free_cell_num;

    ListBind(list, list->tail, free_cell_num);
    ListBind(list, free_cell_num, 0);
        
    list->tail = free_cell_num;

    list->data[free_cell_num] = elem;
    fprintf(stderr, "\n\t\tIn Paste tail: free_cell_num = %d, head = %d\n", free_cell_num, list->head);
    LIST_ASSERT(list);
    LIST_DUMP(list);
    return LIST_FUNC_OK;
    */
}

ListFuncStatus ListPasteAfter(list_t *list, ListElem_t elem, int elem_num)
{
    LIST_ASSERT(list);
    assert(elem_num >= 0);

fprintf(stderr, "\n\nin paste after\n");
    // if (elem_num == list->tail)
    // {
        // ListPasteTail(list, elem);
        // return LIST_FUNC_OK;
    // }

    int free_cell_num = list->free;
    list->free = list->next[list->free];

    // list->next[free_cell_num] = list->next[elem_num];
    // list->next[elem_num] = free_cell_num;

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

    int prev_free = list->free;                 // �������� ������ � ������ ������
    list->free = elem_num;
    ListBind(list, list->prev[elem_num], list->next[elem_num]);
    ListBind(list, list->free, prev_free);

    list->head = list->next[0];
    list->tail = list->prev[0];

    LIST_ASSERT(list);
    LIST_DUMP(list);

    return LIST_FUNC_OK;
}