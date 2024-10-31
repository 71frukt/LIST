#ifndef LIST_H
#define LIST_H

typedef int ListElem_t;

#define LIST_DEBUG
#include "list_debug.h"
#include "list_graph.h"

// const int BASE_LIST_CAPA = 10;

const int DATA_POISON = 0xDEDBEAF;
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

    ON_LIST_DEBUG (
        FILE        *logfile;
        int          error;
        graph_arr_t  graphs;
    );
};


ListCtorVal ListCtor    (list_t *list, int start_capa);
ListDtorVal ListDtor    (list_t *list);

ListElem_t *GetHeadVal  (list_t *list);
ListElem_t *GetTailVal  (list_t *list);

void ListBind           (list_t *list, int prev_el_num, int next_el_num);
void ListPasteHead      (list_t *list, ListElem_t elem);
void ListPasteTail      (list_t *list, ListElem_t elem);
void ListPasteAfter     (list_t *list, ListElem_t elem, int elem_num);
void ListDelElem        (list_t *list, int elem_num);
int  GetNumInData       (list_t *list, int num_in_list);

// debug functions
void  ListAssert   (list_t *list, const char *file, int line, const char *func);
void  PrintListErr (int error);
int   ListVerify   (list_t *list);
char *GetFilePath  (const char *name, const char *folder, char *path);
FILE *OpenLogFile  (const int argc, const char *argv[]);
void  CloseLogFile (void);
void  ListDump     (list_t *list, const char *file, int line, const char *func);

// graph functions
void    GraphsCtor      (graph_arr_t *graphs);
void    GraphsDtor      (graph_arr_t *graphs);
void    MakeGraph       (list_t *list);
void    WriteDotCode    (list_t *list);
void    DrawGraphInFile (const char *dotfile_name, char *picture_file_name);
node_t *GetNodesArr     (list_t *list);

#endif