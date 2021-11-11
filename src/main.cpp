#include <stdio.h>

#include "list.h"

int main() {
    List lst = {};

    ListCtor(&lst, 10);

    for (long long i = 1; i < 10; i++) {
        ListInsertBack(&lst, i);
        printf("Inserting %lld in the back, head : %zu, tail %zu\n", i,
                lst.head, lst.tail);
    } 

    printf("Printing list:\n");
    size_t cur = lst.head;
    while (cur != 0) {
        printf("Current index: %zu ;\n Current value :%lld;\n Next index : %zu\n",
                cur, lst.nodes[cur].data, lst.nodes[cur].next);
        cur = lst.nodes[cur].next;
    }

    ListDtor(&lst);
}
