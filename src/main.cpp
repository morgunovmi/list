#include <stdio.h>

#include "list.h"

void printList(List *list, size_t size) {
    printf("Head : %zu, Tail: %zu, Free: %zu\n", list->head, list->tail, list->free);
    printf("Index : ");
    for (size_t i = 0; i < size; i++) {
        printf("%zu ", i);
    }
    printf("\n");
    printf("Data : ");
    for (size_t i = 0; i < size; i++) {
        printf("%lld ", list->nodes[i].data);
    }
    printf("\n");
    printf("Next : ");
    for (size_t i = 0; i < size; i++) {
        printf("%zu ", list->nodes[i].next);
    }
    printf("\n");
    printf("Prev : ");
    for (size_t i = 0; i < size; i++) {
        printf("%zu ", list->nodes[i].prev);
    }
    printf("\n");
}

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
    printList(&lst, 10);
    

    ListDtor(&lst);
}
