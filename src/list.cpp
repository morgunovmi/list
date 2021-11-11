#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "list.h"

int ListCtor(List *list, size_t capacity) {
    assert(list);
    assert(list->nodes == nullptr);
    assert(list->head == 0);
    assert(list->tail == 0);

    list->nodes = (node_t *)calloc(capacity, sizeof(node_t));
    if (!list->nodes) {
        perror("Error allocating memory for the list");
        return ERR_NOMEM; 
    }
    list->nodes[0].data = 0;
    list->nodes[0].next = 0;
    for (size_t i = 1; i < capacity; i++) {
        list->nodes[i].next = LST_SIZE_POISON;
    }

    list->head = LST_SIZE_POISON; 
    list->tail = LST_SIZE_POISON; 
    list->capacity = capacity;
    return 0;
}

void ListDtor(List *list) {
    assert(list);
    assert(list->nodes);

    free(list->nodes);
    list->head = LST_SIZE_POISON;
    list->tail = LST_SIZE_POISON;
    list->capacity = LST_SIZE_POISON;
}

size_t ListFindFree(List *list) {
    assert(list);
    assert(list->nodes);

    for (size_t i = 1; i < list->capacity; i++) {
        if (list->nodes[i].next == LST_SIZE_POISON) {
            return i;
        }
    }
    return LST_SIZE_POISON;
}

size_t ListFindPrev(List *list, size_t pos) {
    if (list->nodes[pos].next == LST_SIZE_POISON) {
        return LST_SIZE_POISON;
    }

    size_t cur = list->head;

    while (cur != list->tail) {
        if (list->nodes[cur].next == pos) {
            return cur;
        }
    } 

    return LST_SIZE_POISON;
}

int ListInsertFront(List *list, elem_t value) {
    assert(list);
    assert(list->nodes);

    size_t free = ListFindFree(list);
    if (free == LST_SIZE_POISON) {
        return ERR_OVFLOW;
    } 

    list->nodes[free].data = value;
    if (list->head != LST_SIZE_POISON) {
        list->nodes[free].next = list->head;
    } else {
        list->tail = free;
        list->nodes[free].next = 0;
    }
    list->head = free;
    return 0;
}

int ListInsertBack(List *list, elem_t value) {
    assert(list);
    assert(list->nodes);

    size_t free = ListFindFree(list);
    if (free == LST_SIZE_POISON) {
        return ERR_OVFLOW;
    }

    list->nodes[free].data = value;
    list->nodes[free].next = 0;
    
    if (list->tail != LST_SIZE_POISON) {
        list->nodes[list->tail].next = free;
    } else {
        list->head = free;
    }
    list->tail = free;
    return 0;
}

int ListInsertAfter(List *list, size_t pos, elem_t value) {
    assert(list);
    assert(list->nodes);

    if (pos >= list->capacity || list->nodes[pos].next == LST_SIZE_POISON) {
        return ERR_INV_POS;
    }

    if (pos == list->head) {
        return ListInsertFront(list, value);
    } else if (pos == list->tail) {
        return ListInsertBack(list, value);
    } else {
        size_t free = ListFindFree(list);
        if (free == LST_SIZE_POISON) {
            return ERR_OVFLOW;
        } 

        list->nodes[free].data = value;
        list->nodes[free].next = list->nodes[pos].next;
        list->nodes[pos].next = free;
    }

    return 0;
}

int ListRemove(List *list, size_t pos) {
    assert(list);
    assert(list->nodes);

    if (pos >= list->capacity || list->nodes[pos].next == LST_SIZE_POISON) {
        return ERR_INV_POS;
    }

    size_t prev = ListFindPrev(list, pos);
    if (prev == LST_SIZE_POISON) {
        return ERR_INV_POS;
    }

    list->nodes[prev].next = list->nodes[pos].next;

    list->nodes[pos].data = 0;
    list->nodes[pos].next = LST_SIZE_POISON;

    return 0;
}
