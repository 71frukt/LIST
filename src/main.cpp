#include <stdio.h>

#include "list.h"

int main()
{
    list_t list = {};
    // ListCtor(nullptr, 20);

    fprintf(stderr, "p = %p\n", GetHeadVal(&list));
    fprintf(stderr, "p = %p\n", GetTailVal(&list));

    ListPasteTail(&list, 10);
    for (size_t i = 1; i < 6; i++)
        fprintf(stderr, "val = %d \th = %d \t t = %d\n", list.data[i], list.head, list.tail);


    ListPasteTail(&list, 20);
    for (size_t i = 1; i < 6; i++)
        fprintf(stderr, "val = %d \th = %d \t t = %d\n", list.data[i], list.head, list.tail);

    ListPasteTail(&list, 30);
    for (size_t i = 1; i < 6; i++)
        fprintf(stderr, "val = %d \th = %d \t t = %d\n", list.data[i], list.head, list.tail);

    ListPasteHead(&list, 3);
    for (size_t i = 1; i < 6; i++)
        fprintf(stderr, "val = %d \th = %d \t t = %d\n", list.data[i], list.head, list.tail);

    ListPasteHead(&list, 4);
    for (size_t i = 1; i < 6; i++)
        fprintf(stderr, "val = %d \th = %d \t t = %d\n", list.data[i], list.head, list.tail);

    ListPasteHead(&list, 5);
    for (size_t i = 1; i < 6; i++)
        fprintf(stderr, "val = %d \th = %d \t t = %d\n", list.data[i], list.head, list.tail);

    ListDtor(&list);
    return 0;
}