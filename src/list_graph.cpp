#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"
#include "list_graph.h"

void GraphsCtor(graph_arr_t *graphs)
{
    assert(graphs && "graphs == nullptr in GraphsCtor\n");
    graphs->ip   = 0;
    graphs->size = START_GRAPH_ARR_SIZE;

    graphs->graph_files = (FILE **) calloc(graphs->size, sizeof(FILE *));

    graphs->dotfile_name = TMP_DOTFILE_NAME;
}

void GraphsDtor(graph_arr_t *graphs)
{
    assert(graphs && "graphs == nullptr in GraphsDtor\n");

    graphs->ip   = 0;
    graphs->size = 0;

    free(graphs->graph_files);

    remove(graphs->dotfile_name);
}

void MakeGraph(list_t *list)
{
    size_t graph_num = list->graphs.ip++;
    FILE  *cur_graph_file = list->graphs.graph_files[graph_num];

    char graph_name[PATH_NAME_LEN] = {};
    sprintf(graph_name, "graph_%llu.png", graph_num);

    // GetFilePath(graph_name, GRAPH_FOLDER, graph_name);       // TODO whyyyyyyy
    char graph_full_name[PATH_NAME_LEN] = {};
    GetFilePath(graph_name, GRAPH_FOLDER, graph_full_name);

    WriteDotCode(list);

    DrawGraphInFile(TMP_DOTFILE_NAME, graph_full_name);
}

void WriteDotCode(list_t *list)
{
    FILE *dot_file = fopen(list->graphs.dotfile_name, "w");

    node_t *nodes = GetNodesArr(list);

    for (int i = 0; i < list->capacity; i++)
    {
        fprintf(stderr, "ip = %d  val = %d  next = %d  prev = %d\n", nodes[i].ip, nodes[i].val, nodes[i].next, nodes[i].prev);
    }

    fprintf(dot_file, "digraph G{ \n");

    fprintf(dot_file, "rankdir = LR;\n");

    for (int i = 0; i < list->capacity; i++)
    {
        MakeEdge(dot_file, nodes[i], nodes[nodes[i].next]);
        fprintf(stderr, "\t\t i = %d, nx = %d\n", i, nodes[i].next);
    }

    fprintf(dot_file, "} \n");

    free(nodes);

    fclose(dot_file);
}

void DrawGraphInFile(const char *dotfile_name, char *picture_file_name)
{
    char cmd_command[CMD_COMMAND_LEN] = {};
    sprintf(cmd_command, "dot %s -T png -o %s", dotfile_name, picture_file_name);
    fprintf(stderr, "comanda = %s\n", cmd_command);
    system(cmd_command);
}

node_t *GetNodesArr(list_t *list)
{
    node_t *nodes = (node_t *) calloc(list->capacity, sizeof(node_t));

    for (int i = 0; i < list->capacity; i++)
    {
        nodes[i].ip   = i;
        nodes[i].val  = list->data[i];
        nodes[i].next = list->next[i];
        nodes[i].prev = list->prev[i];
    }

    return nodes;
}

void MakeEdge(FILE *dot_file, node_t node_from, node_t node_to)
{
    char node_from_text[NODE_TEXT_LEN] = {};
    sprintf(node_from_text, "%d", node_from.ip);

    char node_to_text[NODE_TEXT_LEN] = {};
    sprintf(node_to_text, "%d", node_to.ip);

    fprintf(dot_file, "%s -> %s; \n", node_from_text, node_to_text);
    fprintf(stderr, "%s -> %s; \n", node_from_text, node_to_text);
}

