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

    size_t graph_num = list->graphs.index++;
    graph_t *cur_graph = &list->graphs.data[graph_num];

    cur_graph->nodes = GetNodesArr(list);

    sprintf(cur_graph->node_head.label, "head");
    cur_graph->node_head.next = list->head;

    sprintf(cur_graph->node_tail.label, "tail");
    cur_graph->node_tail.next = list->tail;

    sprintf(cur_graph->node_free.label, "free");
    cur_graph->node_free.next = list->free;

cur_graph->nodes[0].next = list->head;  // KOLHOS!!!!
cur_graph->nodes[0].prev = list->tail;

    WriteDotCode(cur_graph);

    char graph_file_name[PATH_NAME_LEN] = {};
    sprintf(graph_file_name, "graph_%llu.png", graph_num);

    // GetFilePath(graph_name, GRAPH_FOLDER, graph_name);
    char graph_file_fullname[PATH_NAME_LEN] = {};
    GetFilePath(graph_file_name, GRAPH_FOLDER, graph_file_fullname);

    DrawGraphInFile(TMP_DOTFILE_NAME, graph_file_fullname);

    return GRAPH_FUNC_OK;
}

GraphFuncStatus WriteDotCode(graph_t *graph)
{
    assert(graph);

    FILE *dot_file = fopen(TMP_DOTFILE_NAME, "w");

    if (dot_file == NULL)
    {
        fprintf(stderr, "can not open dot_file in WriteDotCode()\n");
        return GRAPH_FUNC_ERR;
    }

    fprintf(dot_file, "digraph G{           \n"   
                        "rankdir = LR;      \n"  
                        "bgcolor = \"%s\";  \n" , BACKGROUND_COLOR);

    InitNodes(graph, dot_file);
    node_t *nodes = graph->nodes;
    
    MakeEdge(dot_file, graph->node_head, nodes[graph->node_head.next], EDGE_HEAD_COLOR, 1);
    MakeEdge(dot_file, graph->node_tail, nodes[graph->node_tail.next], EDGE_TAIL_COLOR, 1);
    MakeEdge(dot_file, graph->node_free, nodes[graph->node_free.next], EDGE_FREE_COLOR, 1);

// manager
    MakeEdge(dot_file, nodes[0], nodes[nodes[0].next], EDGE_MANAGER_COLOR, 1);
    MakeEdge(dot_file, nodes[nodes[0].prev], nodes[0], EDGE_MANAGER_COLOR, 1);

// зан€тые €чейки
    for (size_t i = nodes[0].next; nodes[i].index != nodes[0].index; i = nodes[i].next)
    {
        fprintf(stderr, "NODES[%lld] NEXT = %d\n\n", i, nodes[i].next);
        if (nodes[i].next != NEXT_POISON && nodes[i].next != nodes[0].index)
            MakeEdge(dot_file, nodes[i], nodes[nodes[i].next], EDGE_NEXT_COLOR, 1);
    }

    // size_t index = 0;
    // while (true)
    // {
        // fprintf(stderr, "NODES[%lld] NEXT = %d\n\n", index, nodes[index].next);
        // if (nodes[index].next != NEXT_POISON)
            // MakeEdge(dot_file, nodes[index], nodes[nodes[index].next], EDGE_NEXT_COLOR, 1);
// 
        // index = nodes[index].next;
// 
        // if (nodes[index].index == nodes[0].index)
            // break;
    // }

// свободные €чейки    
    for (size_t i = graph->node_free.next; nodes[i].next != END_OF_FREE; i = nodes[i].next)
    {
        // fprintf(stderr, "free: i = %lld, [i].next = %d, [0].prev = %d\n", i, nodes[i].next, nodes[0].prev);
        MakeEdge(dot_file, nodes[i], nodes[nodes[i].next], EDGE_FREE_COLOR, 1);
    }
    
    fprintf(dot_file, "} \n");

    free(nodes);

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

    fprintf(dotfile, "%s [label = \"manager\"]\n", 
            nodes[0].label);

    for (size_t i = 1; i < graph->nodes_count; i++)
    {
        char next_val_str[10] = {};
        char node_val_str[10] = {};
        char prev_val_str[10] = {};

        VALUE_TO_STR(nodes[i].next, "d",              NEXT_POISON, NEXT_POISON_MARK, next_val_str);
        VALUE_TO_STR(nodes[i].next, "d",              END_OF_FREE, END_OF_FREE_MARK, next_val_str);
        VALUE_TO_STR(nodes[i].prev, "d",              PREV_POISON, PREV_POISON_MARK, prev_val_str);
        VALUE_TO_STR(nodes[i].val,  LIST_ELEM_FORMAT, DATA_POISON, DATA_POISON_MARK, node_val_str);

        fprintf(dotfile, "%s [shape = \"record\", label = \"index = %d | value = %s | next = %s | prev = %s\"]\n", 
            nodes[i].label, nodes[i].index, node_val_str, next_val_str, prev_val_str);
    }

    for (size_t i = 0; i < graph->nodes_count - 1; i++)
    {
        MakeEdge(dotfile, nodes[i], nodes[i + 1], BACKGROUND_COLOR, 1000);
    }

    node_t node_head = graph->node_head;
    fprintf(dotfile, "%s [shape = \"component\", label = \"%s:\\n %d\"]\n", 
            node_head.label, node_head.label, node_head.next);

    node_t node_tail = graph->node_tail;
    fprintf(dotfile, "%s [shape = \"component\", label = \"%s:\\n %d\"]\n", 
            node_tail.label, node_tail.label, node_tail.next);

    node_t node_free = graph->node_free;
    fprintf(dotfile, "%s [shape = \"component\", label = \"%s:\\n %d\"]\n", 
            node_free.label, node_free.label, node_free.next);

    fprintf(dotfile, " { rank = same; %s; %s; %s; %s; }\n", node_head.label, node_tail.label, node_free.label, nodes[0].label);
    MakeEdge(dotfile, node_head, node_tail, BACKGROUND_COLOR, 1000);
    MakeEdge(dotfile, node_tail, node_free, BACKGROUND_COLOR, 1000);
    MakeEdge(dotfile, node_free, nodes[0],  BACKGROUND_COLOR, 1000);

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
    // fprintf(stderr, "%s -> %s; \n", node_from.label, node_to.label);

    return GRAPH_FUNC_OK;
}
