#ifndef LIST_H
#define LIST_H

typedef int ListElem_t;
#define LIST_ELEM_FORMAT "d"

#define LIST_DEBUG
#include "list_debug.h"
#include "list_graph.h"

// const int BASE_LIST_CAPA = 10;

const int DATA_POISON = 0xDEDBEAF;
const int NEXT_POISON = 0xDEB11;
const int PREV_POISON = 0xEBA110B; 

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

enum ListFuncStatus
{
    LIST_FUNC_OK,
    LIST_FUNC_ERR
};

ListFuncStatus ListCtor    (list_t *list, int start_capa);
ListFuncStatus ListDtor    (list_t *list);

ListElem_t    *GetHeadVal  (list_t *list);
ListElem_t    *GetTailVal  (list_t *list);
ListFuncStatus ListBind           (list_t *list, int prev_el_num, int next_el_num);
ListFuncStatus ListPasteHead      (list_t *list, ListElem_t elem);
ListFuncStatus ListPasteTail      (list_t *list, ListElem_t elem);
ListFuncStatus ListPasteAfter     (list_t *list, ListElem_t elem, int elem_num);
ListFuncStatus ListDelElem        (list_t *list, int elem_num);
int            GetNumInData       (list_t *list, int num_in_list);

// debug functions
void  ListAssert   (list_t *list, const char *file, int line, const char *func);
void  PrintListErr (int error);
int   ListVerify   (list_t *list);
char *GetFilePath  (const char *name, const char *folder, char *path);
FILE *OpenLogFile  (const int argc, const char *argv[]);
void  CloseLogFile (void);
void  ListDump     (list_t *list, const char *file, int line, const char *func);

// graph functions
GraphFuncStatus    GraphsCtor      (graph_arr_t *graphs);
GraphFuncStatus    GraphsDtor      (graph_arr_t *graphs);
GraphFuncStatus    MakeGraph       (list_t *list);
GraphFuncStatus    WriteDotCode    (list_t *list);
GraphFuncStatus    DrawGraphInFile (const char *dotfile_name, char *picture_file_name);
GraphFuncStatus    MakeEdge        (FILE *dot_file, node_t node_from, node_t node_to);
node_t            *GetNodesArr     (list_t *list);

#endif