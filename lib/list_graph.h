#ifndef LIST_GRAPH_H
#define LIST_GRAPH_H

#include <stdio.h>

#include "list.h"

const int START_GRAPH_ARR_SIZE = 10;

struct graph_arr_t
{
    size_t ip;
    size_t size;

    FILE **graph_files;
    FILE  *tmp_dotfile;
};

struct node_t
{
    int ip;
    ListElem_t val;
    int next;
    int prev;
};

#endif