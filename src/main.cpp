#include <stdio.h>

#include "list.h"

extern FILE *LogFile;

int main(const int argc, const char *argv[])
{
    // TODO: remove 
    ON_LIST_DEBUG(OpenLogFile(argc, argv));
    list_t list = {};
    ListCtor(&list, 15);
    LIST_DUMP(&list);

    // fprintf(stderr, "h = %p\n", GetHeadVal(&list));
    // fprintf(stderr, "t = %p\n", GetTailVal(&list));

    // ListPasteHead(&list, 10);
    // ListPasteHead(&list, 20);
    // ListPasteHead(&list, 30);
    ListPasteHead(&list, 100);
    // ListPasteHead(&list, 200);
    // ListPasteHead(&list, 300);
    // ListPasteHead(&list, 400);
    // ListPasteHead(&list, 500);

    ListPasteAfter(&list, 150, 1);
fprintf(stderr, "NE padaet zdes\n");
    // ListPasteAfter(&list, 350, 3);
    // ListPasteAfter(&list, 250, 2);

    fprintf(stderr, "num in data = %d\n", GetNumInData(&list, 7));

    LIST_DUMP(&list);
    LIST_DUMP(&list);
    ListDtor(&list);

    fprintf(stderr, "END!\n");
    return 0;
}