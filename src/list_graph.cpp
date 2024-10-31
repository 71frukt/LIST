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

    graphs->tmp_dotfile = fopen(TMP_DOTFILE_NAME, "w");
}

void GraphsDtor(graph_arr_t *graphs)
{
    assert(graphs && "graphs == nullptr in GraphsDtor\n");

    graphs->ip   = 0;
    graphs->size = 0;

    free(graphs->graph_files);

    remove(TMP_DOTFILE_NAME);
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
    FILE *dot_file = list->graphs.tmp_dotfile;

    node_t *nodes = GetNodesArr(list);

    for (int i = 0; i < list->capacity; i++)
    {
        fprintf(stderr, "ip = %d  val = %d  next = %d  prev = %d\n", nodes[i].ip, nodes[i].val, nodes[i].next, nodes[i].prev);
    }

    fprintf(dot_file, "digraph G{ \n");

    fprintf(dot_file, "\tGOVNO -> ZALUPA -> PENIS -> HER -> DAVALKA -> HUI -> BLYADINA;\n");

    fprintf(dot_file, "} \n");

    fclose(list->graphs.tmp_dotfile);
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
    LIST_ASSERT(list);

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

// void MakeEdge(FILE *dot_file, )
// {
// 
// }

