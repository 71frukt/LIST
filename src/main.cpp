#include <stdio.h>

#include "list.h"

extern FILE *LogFile;

int main(const int argc, const char *argv[])
{
    ON_LIST_DEBUG(OpenLogFile(argc, argv));

    list_t list = {};
    ListCtor(&list, 20);
    LIST_DUMP(&list);

    fprintf(stderr, "h = %p\n", GetHeadVal(&list));
    fprintf(stderr, "t = %p\n", GetTailVal(&list));

    ListPasteHead(&list, 10);
    ListPasteHead(&list, 20);
    ListPasteHead(&list, 30);

    ListPasteTail(&list, 100);
    ListPasteTail(&list, 200);
    ListPasteTail(&list, 300);

    ListDtor(&list);
    return 0;
}