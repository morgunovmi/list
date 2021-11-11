#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdint.h>

typedef long long elem_t;

const size_t LST_SIZE_POISON = SIZE_MAX;
const size_t LST_DEFAULT_CAPACITY = 100;

struct node_t {
    elem_t data;
    size_t next;
    size_t prev;
};

struct List {
    node_t *nodes;
    size_t head;
    size_t tail;
    size_t free;
    size_t capacity;
};

enum ListError : int {
    ERR_NOMEM = 1,
    ERR_OVFLOW = 2,
    ERR_INV_POS = 3,
};

int ListCtor(List *list, size_t capacity=LST_DEFAULT_CAPACITY);

void ListDtor(List *list);

int ListInsertFront(List *list, elem_t value);

int ListInsertBack(List *list, elem_t value);

int ListInsertAfter(List *list, size_t pos, elem_t value);

int ListRemove(List *list, size_t pos);

#endif
