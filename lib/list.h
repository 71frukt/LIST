#ifndef LIST_H
#define LIST_H

#define LIST_DEBUG
#include "list_debug.h"

typedef int ListElem_t;

// const int BASE_LIST_CAPA = 10;

const int NEXT_POISON = -0xDEB;
const int PREV_POISON = -0xEBA; 

enum ListCtorVal
{
    CTOR_OK,
    CTOR_ERR
};

enum ListDtorVal
{
    DTOR_OK,
    DTOR_ERR
};

struct list_t
{
    ListElem_t *data;
    int        *next;
    int        *prev;

    int capacity;
    int head;
    int tail;
    int free;

    int error;
};


ListCtorVal ListCtor    (list_t *list, int start_capa);
ListDtorVal ListDtor    (list_t *list);

ListElem_t *GetHeadVal  (list_t *list);
ListElem_t *GetTailVal  (list_t *list);

void ListPasteTail      (list_t *list, ListElem_t elem);
void ListPasteHead      (list_t *list, ListElem_t elem);
void ListPasteAfter     (list_t *list, ListElem_t elem, int elem_num);

// debug functions
void ListAssert   (list_t *list, const char *file, int line, const char *func);
void PrintListErr (int error);
int  ListVerify   (list_t *list);

#endif