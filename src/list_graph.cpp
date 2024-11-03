#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"
#include "list_graph.h"

GraphFuncStatus GraphsCtor(graph_arr_t *graphs)
{
    assert(graphs && "graphs == nullptr in GraphsCtor\n");
    graphs->index = 0;
    graphs->size  = START_GRAPH_ARR_SIZE;
    graphs->data = (graph_t *) calloc(graphs->size, sizeof(graph_t));

    if (graphs->data == NULL)
    {
        fprintf(stderr, "can not calloc graphs->data in GraphsCtor()\n");
        return GRAPH_FUNC_ERR;
    }

    graphs->dotfile_name = TMP_DOTFILE_NAME;

    return GRAPH_FUNC_OK;
}

GraphFuncStatus GraphsDtor(graph_arr_t *graphs)
{
    assert(graphs && "graphs == nullptr in GraphsDtor\n");

    graphs->index = 0;
    graphs->size  = 0;

    free(graphs->data);

    remove(graphs->dotfile_name);

    return GRAPH_FUNC_OK;
}

GraphFuncStatus MakeGraph(list_t *list)
{
    assert(list);

    size_t graph_num = list->graphs.index;

    WriteDotCode(list);

    char graph_file_name[PATH_NAME_LEN] = {};
    sprintf(graph_file_name, "graph_%llu.png", graph_num);

    // GetFilePath(graph_name, GRAPH_FOLDER, graph_name);
    char graph_file_fullname[PATH_NAME_LEN] = {};
    GetFilePath(graph_file_name, GRAPH_FOLDER, graph_file_fullname);

    DrawGraphInFile(TMP_DOTFILE_NAME, graph_file_fullname);

    list->graphs.index++;
    return GRAPH_FUNC_OK;
}

GraphFuncStatus WriteDotCode(list_t *list)
{
    assert(list);

// fprintf(stderr, "list [%p]\ngraphs [%p]\ndotf [%s]\n", list, &list->graphs, list->graphs.dotfile_name);
    FILE *dot_file = fopen(TMP_DOTFILE_NAME, "w");

    if (dot_file == NULL)
    {
        fprintf(stderr, "can not open dot_file in WriteDotCode()\n");
        return GRAPH_FUNC_ERR;
    }

    graph_t *cur_graph = &list->graphs.data[list->graphs.index];

    cur_graph->nodes = GetNodesArr(list);

    if (cur_graph->nodes == NULL)
    {
        fprintf(stderr, "cur_graph.nodes == NULL in WriteDotCode()\n");
        return GRAPH_FUNC_ERR;
    }

    fprintf(dot_file, "digraph G{ \n");

    fprintf(dot_file, "rankdir = LR;\n");

    InitNodes(cur_graph, dot_file);

    for (int i = 0; i < list->capacity; i++)
    {
        MakeEdge(dot_file, cur_graph->nodes[i], cur_graph->nodes[cur_graph->nodes[i].next], EDGE_PREV_COLOR, 1);
        fprintf(stderr, "\t i = %d, nx = %d\n", i, cur_graph->nodes[i].next);
    }

    fprintf(dot_file, "} \n");

    free(cur_graph->nodes);

    if (fclose(dot_file) != 0)
    {
        fprintf(stderr, "can not close dot_file in WriteDotCode()\n");
        return GRAPH_FUNC_ERR;
    }

    return GRAPH_FUNC_OK;
}

GraphFuncStatus InitNodes(graph_t *graph, FILE *dotfile)
{
    assert(graph);
    assert(dotfile);

    node_t *nodes = graph->nodes;


    for (size_t i = 0; i < graph->nodes_num; i++)
    {
        char next_val_str[10] = {};
        VALUE_TO_STR(nodes[i].next, "d", NEXT_POISON, NEXT_POISON_MARK, next_val_str);

        char prev_val_str[10] = {};
        VALUE_TO_STR(nodes[i].prev, "d", PREV_POISON, PREV_POISON_MARK, prev_val_str);

        char node_val_str[10] = {};
        VALUE_TO_STR(nodes[i].val, LIST_ELEM_FORMAT, DATA_POISON, DATA_POISON_MARK, node_val_str);

        // if (nodes[i].next == NEXT_POISON)
            // sprintf(next_val_str, "%s", NEXT_POISON_MARK);
        // else 
            // sprintf(next_val_str, "%d", nodes[i].next);

        fprintf(dotfile, "%s [shape = \"record\", label = \" index = %d | value = %s | next = %s | prev = %s\"]\n", 
            nodes[i].label, nodes[i].index, node_val_str, next_val_str, prev_val_str);
    }

    for (size_t i = 0; i < graph->nodes_num - 1; i++)
    {
        MakeEdge(dotfile, nodes[i], nodes[i + 1], "white", 1000);
    }

    return GRAPH_FUNC_OK;
}

GraphFuncStatus DrawGraphInFile(const char *dotfile_name, char *picture_file_name)
{
    assert(dotfile_name);
    assert(picture_file_name);

    char cmd_command[CMD_COMMAND_LEN] = {};
    sprintf(cmd_command, "dot %s -T png -o %s", dotfile_name, picture_file_name);
    fprintf(stderr, "comanda = %s\n", cmd_command);
    system(cmd_command);

    return GRAPH_FUNC_OK;
}

node_t *GetNodesArr(list_t *list)
{
    assert(list);

    node_t *nodes = (node_t *) calloc(list->capacity, sizeof(node_t));

    for (int i = 0; i < list->capacity; i++)
    {
        nodes[i].index = i;
        nodes[i].val   = list->data[i];
        nodes[i].next  = list->next[i];
        nodes[i].prev  = list->prev[i];
        sprintf(nodes[i].label, "NODE_%d", i);
    }

    return nodes;
}

GraphFuncStatus MakeEdge(FILE *dot_file, node_t node_from, node_t node_to, const char *edge_color, size_t edge_weight)
{
    assert(dot_file);
    assert(edge_color);

    fprintf(dot_file, "%s -> %s[color = \"%s\", weight = %lld]; \n", node_from.label, node_to.label, edge_color, edge_weight);
    fprintf(stderr, "%s -> %s; \n", node_from.label, node_to.label);

    return GRAPH_FUNC_OK;
}
