#ifndef LIST_GRAPH_H
#define LIST_GRAPH_H

#include <stdio.h>

#include "list.h"

#define DOT_FOLDER         LOGFILE_FOLDER "dot/"
#define GRAPH_FOLDER       "graphs/"
#define TMP_DOTFILE_NAME   "tmp_doc_code.doc"
#define GRAPH_NAME_PREFIX  "graph_"

const char *const BACKGROUND_COLOR   = "#FFF3D1";
const char *const EDGE_NEXT_COLOR    = "darkcyan";
// const char *const EDGE_PREV_COLOR    = "darkred";
const char *const EDGE_FREE_COLOR    = "darkgreen";
const char *const EDGE_HEAD_COLOR    = "#0000CC";
const char *const EDGE_TAIL_COLOR    = "#B701EF";
const char *const EDGE_MANAGER_COLOR = "darkred";

const int NODE_NAME_LEN        = 20;
const int GRAPH_NAME_LEN       = 20;
const int START_GRAPH_ARR_SIZE = 100;

const int GRAPH_PICTURE_WIDTH = 85;     // (%)

struct node_t
{
    ListElem_t val;
    char label[NODE_NAME_LEN];
    int  index;
    int  next;
    int  prev;
};

struct graph_t
{
    char    name[GRAPH_NAME_LEN];
    
    size_t  nodes_count;
    node_t  node_free;
    node_t  node_head;
    node_t  node_tail;
 
    node_t *nodes;
};

struct graph_arr_t
{
    size_t index;           // указывает на первый пустой граф
    size_t size;

    graph_t *data;
};

enum GraphFuncStatus
{
    GRAPH_FUNC_OK,
    GRAPH_FUNC_ERR
};

#endif