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

const char *const BASE_LOGFILE_NAME = "list_logfile.log";
const char *const LOGFILE_FOLDER    = "logs/";
const int   LOGFILE_NAME_LEN  = 50;

// #define GET_LOGFILE_NAME(logfile_name)  LOGFILE_FOLDER logfile_name

#ifdef LIST_DEBUG

#define ON_LIST_DEBUG(...) __VA_ARGS__

#define LIST_DUMP(list)  ListDump(list, __FILE__, __LINE__, __func__)

#define LIST_ASSERT(list)                                                   \
{                                                                           \
    ListAssert(list, __FILE__, __LINE__, __func__);                         \
    LIST_DUMP(list);                                                     \
}            

#else
#define ON_LIST_DEBUG(...)
#define LIST_DUMP(list_ptr)
#define LIST_ASSERT(list)

#endif

#endif