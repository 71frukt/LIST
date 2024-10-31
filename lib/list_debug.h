#ifndef LIST_DEBUG_H
#define LIST_DEBUG_H

enum ListError_t
{
    LIST_OK = 0,

    LIST_PTR_ERR   = 1 << 0,
    DATA_PTR_ERR   = 1 << 1,
    HEAD_UNDERFLOW = 1 << 2,
    HEAD_OVERFLOW  = 1 << 3,
    TAIL_UNDERFLOW = 1 << 4,
    TAIL_OVERFLOW  = 1 << 5,
    FREE_UNDERFLOW = 1 << 6,
    FREE_OVERFLOW  = 1 << 7
};

#define BASE_LOGFILE_NAME  "list_logfile.log"
#define LOGFILE_FOLDER     "logs/"
#define DOT_FOLDER         LOGFILE_FOLDER "dot/"
#define GRAPH_FOLDER       LOGFILE_FOLDER "graphs/"
#define TMP_DOTFILE_NAME   "tmp_doc_code.doc"   

const int PATH_NAME_LEN   = 50;
const int CMD_COMMAND_LEN = 100;
const int NODE_TEXT_LEN   = 100;

#ifdef LIST_DEBUG

#define ON_LIST_DEBUG(...) __VA_ARGS__

#define LIST_DUMP(list)  ListDump(list, __FILE__, __LINE__, __func__)

#define LIST_ASSERT(list)                                                   \
{                                                                           \
    ListAssert(list, __FILE__, __LINE__, __func__);                         \
    LIST_DUMP(list);                                                        \
}            

#else
#define ON_LIST_DEBUG(...)
#define LIST_DUMP(list_ptr)
#define LIST_ASSERT(list)

#endif

#endif