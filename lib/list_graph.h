#ifndef LIST_GRAPH_H
#define LIST_GRAPH_H

#include <stdio.h>

#include "list.h"

const int NODE_NAME_LEN        = 20;
const int START_GRAPH_ARR_SIZE = 100;

struct node_t
{
    ListElem_t val;
    char name[NODE_NAME_LEN];
    int  index;
    int  next;
    int  prev;
};

struct graph_t
{
    size_t  nodes_num;
    node_t *nodes;
};

struct graph_arr_t
{
    size_t index;
    size_t size;

    graph_t *data;

    const char *dotfile_name;
    // FILE **graph_files;
};

enum GraphFuncStatus
{
    GRAPH_FUNC_OK,
    GRAPH_FUNC_ERR
};

#endif