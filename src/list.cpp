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

    for (int i = 1; i < list->capacity - 1; i++)     // ������� ������ ��������� �����
    {
        list->next[i] = i + 1;
    }

    if (list->capacity > 0)
        list->next[list->capacity - 1] = 0;             // ��������� ��������� ������

    ON_LIST_DEBUG (
        for (int i = 0; i < list->capacity; i++)
        {
            list->data[i] = DATA_POISON;
            list->prev[i] = PREV_POISON;
        } )

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

void ListPasteTail(list_t *list, ListElem_t elem)
{
    LIST_ASSERT(list);

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

    LIST_ASSERT(list);
}

void ListPasteHead(list_t *list, ListElem_t elem)
{
    LIST_ASSERT(list);

    int free_cell_num = list->free;
    list->free = list->next[list->free];

    // list->next[free_cell_num] = list->head;

    if (list->head == 0)                // ���� ������ ������� ������
        list->tail = free_cell_num;

    ListBind(list, free_cell_num, list->head);

    list->head = free_cell_num;

    list->data[free_cell_num] = elem;

    LIST_ASSERT(list);
}

void ListPasteAfter(list_t *list, ListElem_t elem, int elem_num)
{
    assert(elem_num > 0);
    LIST_ASSERT(list);

    if (elem_num == list->tail)
    {
        ListPasteTail(list, elem);
        return;
    }

    int free_cell_num = list->free;
    list->free = list->next[list->free];

    // list->next[free_cell_num] = list->next[elem_num];
    // list->next[elem_num] = free_cell_num;

    ListBind(list, free_cell_num, list->next[elem_num]);    // queue is important !
    ListBind(list, elem_num,      free_cell_num);

    list->data[free_cell_num] = elem;

    LIST_ASSERT(list);
}

void ListBind(list_t *list, int prev_el_num, int next_el_num)
{
    LIST_ASSERT(list);
    assert(prev_el_num >= 0);
    assert(next_el_num >= 0);

    list->next[prev_el_num] = next_el_num;
    list->prev[next_el_num] = prev_el_num;
}

void ListDelElem(list_t *list, int elem_num)
{
    LIST_ASSERT(list);

    ON_LIST_DEBUG (
        list->data[elem_num] = DATA_POISON;
    )

    int prev_free = list->free;                 // �������� ������ � ������ ������
    list->free = elem_num;
    ListBind(list, list->free, prev_free);

    ListBind(list, list->prev[elem_num], list->next[elem_num]);

    LIST_ASSERT(list);
}