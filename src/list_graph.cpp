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

    graphs->doc_tmp_codefile = fopen(DOC_TMP_CODEFILE, "w");
}

void GraphsDtor(graph_arr_t *graphs)
{
    assert(graphs && "graphs == nullptr in GraphsDtor\n");

    graphs->ip   = 0;
    graphs->size = 0;

    free(graphs->graph_files);

    fclose(graphs->doc_tmp_codefile);
    remove(DOC_TMP_CODEFILE);
}

void MakeGraph(list_t *list)
{
    size_t graph_num = list->graphs.ip++;

    char graph_name[PATH_NAME_LEN] = {};
    sprintf(graph_name, "graph_%llu.png", graph_num);

    // GetFilePath(graph_name, GRAPH_FOLDER, graph_name);       // TODO whyyyyyyy
    char graph_path[PATH_NAME_LEN] = {};
    GetFilePath(graph_name, GRAPH_FOLDER, graph_path);

    list->graphs.graph_files[graph_num] = fopen(graph_path, "w");

    WriteDocCode(list);
}

void WriteDocCode(list_t *list)
{
    FILE *doc_file = list->graphs.doc_tmp_codefile;

    fprintf(doc_file, "sobaka siela bobra\n");
}