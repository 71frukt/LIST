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

    // ListPasteHead(&list, 10);
    // ListPasteHead(&list, 20);
    // ListPasteHead(&list, 30);

    ListPasteHead(&list, 100);
    ListPasteHead(&list, 200);
    ListPasteHead(&list, 300);
    ListPasteHead(&list, 400);
    ListPasteHead(&list, 500);

    ListPasteAfter(&list, 150, 1);
    ListPasteAfter(&list, 350, 3);
    ListPasteAfter(&list, 250, 2);

    fprintf(stderr, "num in data = %d\n", GetNumInData(&list, 7));

    // ListDtor(&list);
    return 0;
}