#include <stdio.h>
#include <stdlib.h>

#include "list.h"

int main() {

    List lst = {};

    ListCtor(&lst, 10);

    ListDump(&lst, "start");

    for (long long i = 1; i < 10; i++) {
        ListInsertFront(&lst, i);
    } 

    ListDump(&lst, "main 1");

    ListRemove(&lst, 3);
    ListRemove(&lst, 6);

    ListSort_VERY_SLOWWWWWWWWWWWWWWWWWW(&lst);

    for (long long i = 10; i < 25; i++) {
        ListInsertBack(&lst, i);
    } 

    ListInsertAfter(&lst, 5, 115);
    ListInsertAfter(&lst, 10, 115);
    ListInsertAfter(&lst, 15, 115);
    ListInsertAfter(&lst, 13, 115);
    ListInsertAfter(&lst, 3, 115);
    ListInsertAfter(&lst, 2, 115);

    ListDump(&lst, "Dumb dump");
    ListRemove(&lst, 10);
    ListRemove(&lst, 7);
    ListRemove(&lst, 5);
    ListDump(&lst, "Dumb dump 2");

    for (size_t i = 1; i < 50; i++) {
        printf("%zu\n", i);
        ListInsertAfter(&lst, 
                ListLogicalToPhysicalIdx_DONT_CALL_SLOW_ASF(&lst, rand() % lst.size), rand());
        ListInsertBefore(&lst, 
                ListLogicalToPhysicalIdx_DONT_CALL_SLOW_ASF(&lst, rand() % lst.size), rand());
    }
    printf("Removing \n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    for (size_t i = 1; i < 20; i++) {
        printf("%zu\n", i);
        ListRemove(&lst, ListLogicalToPhysicalIdx_DONT_CALL_SLOW_ASF(&lst, rand() % lst.size));
    }
    ListDump(&lst, "Dumb dump 3");

    ListSort_VERY_SLOWWWWWWWWWWWWWWWWWW(&lst);
    ListDump(&lst, "After sort");

    ListDtor(&lst);
}
