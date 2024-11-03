#ifndef LIST_DEBUG_H
#define LIST_DEBUG_H

enum ListError_t
{
    LIST_OK = 0,

    LIST_PTR_ERR        = 1 << 0,
    LIST_DATA_PTR_ERR   = 1 << 1,
    LIST_HEAD_UNDERFLOW = 1 << 2,
    LIST_HEAD_OVERFLOW  = 1 << 3,
    LIST_TAIL_UNDERFLOW = 1 << 4,
    LIST_TAIL_OVERFLOW  = 1 << 5,
    LIST_FREE_UNDERFLOW = 1 << 6,
    LIST_FREE_OVERFLOW  = 1 << 7
};

#define BASE_LOGFILE_NAME  "list_logfile.log"
#define LOGFILE_FOLDER     "logs/"
#define DOT_FOLDER         LOGFILE_FOLDER "dot/"
#define GRAPH_FOLDER       LOGFILE_FOLDER "graphs/"
#define TMP_DOTFILE_NAME   "tmp_doc_code.doc"  

const int PATH_NAME_LEN   = 50;
const int CMD_COMMAND_LEN = 100;
const int NODE_TEXT_LEN   = 100;

const ListElem_t  DATA_POISON = 0xDEDBEAF;
const int         NEXT_POISON = 0xDEB11;
const int         PREV_POISON = 0xEBA110B;
const int         END_OF_FREE = 0xF3EEE7D;

const char *const DATA_POISON_MARK = "DATA#";
const char *const NEXT_POISON_MARK = "NEXT#";
const char *const PREV_POISON_MARK = "PREV#";
const char *const END_OF_FREE_MARK = "FREE_END";

#define VALUE_TO_STR(val, val_type_specifier, poison_val, poison_mark, res_str)             \
{                                                                                           \
    if (val == poison_val)                                                                  \
        sprintf(res_str, "%s", poison_mark);                                                 \
    else                                                                                    \
        sprintf(res_str, "%" val_type_specifier, val);                                       \
}

#ifdef LIST_DEBUG

#define ON_LIST_DEBUG(...) __VA_ARGS__

#define LIST_DUMP(list)  ListDump(list, __FILE__, __LINE__, __func__)

#define LIST_ASSERT(list)                                                   \
{                                                                           \
    ListAssert(list, __FILE__, __LINE__, __func__);                         \
    /*LIST_DUMP(list);*/                                                    \
}            

#else
#define ON_LIST_DEBUG(...)
#define LIST_DUMP(list_ptr)
#define LIST_ASSERT(list)

#endif

#endif