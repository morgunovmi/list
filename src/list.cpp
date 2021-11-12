#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "list.h"

int ListCtor(List *list, size_t capacity) {
    assert(list);
    assert(list->nodes == nullptr);
    assert(list->head == 0);
    assert(list->tail == 0);

    list->nodes = (node_t *)calloc((capacity + 1), sizeof(node_t));
    if (!list->nodes) {
        perror("Error allocating memory for the list");
        return ERR_NOMEM; 
    }
    list->nodes[0].data = 0;
    list->nodes[0].next = 0;
    list->nodes[0].prev = 0;

    for (size_t i = 1; i < capacity; i++) {
        list->nodes[i].prev = LST_SIZE_POISON;
        list->nodes[i].next = i + 1;
    }
    list->nodes[capacity - 1].next = 0;

    list->head = LST_SIZE_POISON; 
    list->tail = LST_SIZE_POISON; 
    list->free = 1;
    list->size = 0;
    list->capacity = capacity;
    return 0;
}

void ListDtor(List *list) {
    assert(list);
    assert(list->nodes);

    free(list->nodes);
    list->head = LST_SIZE_POISON;
    list->tail = LST_SIZE_POISON;
    list->free = LST_SIZE_POISON;
    list->size = LST_SIZE_POISON;
    list->capacity = LST_SIZE_POISON;
}

size_t ListLogicalToPhysicalIdx_DONT_CALL_SLOW_ASF(List *list, size_t idx) {
    assert(list);
    assert(list->nodes);

    if (idx >= list->size) {
        return LST_SIZE_POISON;
    }

    size_t cur = list->head;
    while (idx--) {
        cur = list->nodes[cur].next;
    }
    return cur;
}

int ListInsertFront(List *list, elem_t value) {
    assert(list);
    assert(list->nodes);

    if (list->free == 0) {
        return ERR_OVFLOW;
    }
    size_t nextFree = list->nodes[list->free].next;

    list->nodes[list->free].data = value;
    if (list->head != LST_SIZE_POISON) {
        list->nodes[list->free].next = list->head;
        list->nodes[list->free].prev = 0; 
    } else {
        list->tail = list->free;
        list->nodes[list->free].next = 0;
        list->nodes[list->free].prev = 0;
    }
    list->head = list->free;
    list->free = nextFree;
    list->size++;
    return 0;
}

int ListInsertBack(List *list, elem_t value) {
    assert(list);
    assert(list->nodes);

    if (list->free == 0) {
        return ERR_OVFLOW;
    }
    size_t nextFree = list->nodes[list->free].next;

    list->nodes[list->free].data = value;
    list->nodes[list->free].next = 0;
    
    if (list->tail != LST_SIZE_POISON) {
        list->nodes[list->tail].next = list->free;
        list->nodes[list->free].prev = list->tail;
    } else {
        list->head = list->free;
        list->nodes[list->free].prev = 0;
    }
    list->tail = list->free;
    list->free = nextFree;
    list->size++;
    return 0;
}

int ListInsertAfter(List *list, size_t pos, elem_t value) {
    assert(list);
    assert(list->nodes);

    if (pos >= list->capacity || list->nodes[pos].prev == LST_SIZE_POISON) {
        return ERR_INV_POS;
    }

    if (pos == 0) {
        return ListInsertFront(list, value);
    } else if (pos == list->tail) {
        return ListInsertBack(list, value);
    } else {
        if (list->free == 0) {
            return ERR_OVFLOW;
        }
        size_t nextFree = list->nodes[list->free].next;

        list->nodes[list->free].data = value;
        list->nodes[list->free].next = list->nodes[pos].next;
        list->nodes[pos].next = list->free;
        list->free = nextFree;
    }
    list->size++;

    return 0;
}

int ListRemove(List *list, size_t pos) {
    assert(list);
    assert(list->nodes);

    if (pos == 0 || 
        pos >= list->capacity || 
        list->nodes[pos].prev == LST_SIZE_POISON) {
        return ERR_INV_POS;
    }

    if (pos == list->head && pos == list->tail) {
        list->head = LST_SIZE_POISON;
        list->tail = LST_SIZE_POISON;
    } else if (pos == list->head) {
        list->head = list->nodes[pos].next;
        list->nodes[list->nodes[pos].next].prev = 0;
    } else if (pos == list->tail) {
        list->tail = list->nodes[pos].prev;
        list->nodes[list->nodes[pos].prev].next = 0;
    } else {
        list->nodes[list->nodes[pos].prev].next = list->nodes[pos].next;
    }

    list->nodes[pos].data = 0;
    list->nodes[pos].prev = LST_SIZE_POISON;

    list->nodes[pos].next = list->free;
    list->free = pos;
    list->size--;

    return 0;
}
