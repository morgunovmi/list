#include <assert.h>
#include <stdlib.h> 
#include <string.h>
#include <errno.h>

#include "list.h"

int ListError(List *list) {
    if (!list) {
        return ERR_NULL;
    } 

    if (!list->nodes) {
        return ERR_NULL_ND;
    }

    if (list->size > list->capacity) {
        return ERR_CAP_OVERFL;
    }

    if (list->size == LST_SIZE_POISON || list->capacity == LST_SIZE_POISON) {
        return ERR_INV_SIZE;
    }

    if (list->nodes[0].next != 0 || list->nodes[0].prev != 0) {
        return ERR_INV_ZERO;
    }
    
    for (size_t i = 1; i < list->capacity; i++) {
        if (list->nodes[i].next == LST_SIZE_POISON) {
            return ERR_INV_NEXT;
        }

        if (list->nodes[i].prev == 0 &&
            list->tail != list->head && i != list->head) {
            return ERR_INV_HEAD;
        }

        if (list->nodes[i].next == 0 && i != list->tail
            && list->nodes[i].prev != LST_SIZE_POISON) {
            return ERR_INV_TAIL;
        }

        if (i == list->head && list->size > 0 && list->nodes[i].prev != 0) {
            return ERR_INV_HEAD;
        }

        if (i == list->tail && list->size > 0 && list->nodes[i].next != 0) {
            return ERR_INV_TAIL;
        }
    } 

    return 0;
}

int writeErrToFile(FILE *file, int err) {
    int res = 0;
    switch (err) {
        case ERR_NULL:
            res = fprintf(file, "ERR_NULL");
            break;

        case ERR_INV_POS:
            res = fprintf(file, "ERR_INV_POS");
            break;
            
        case ERR_NULL_ND:
            res = fprintf(file, "ERR_NULL_NODES");
            break;

        case ERR_CAP_OVERFL:
            res = fprintf(file, "ERR_CAPACITY_OVERFLOW");
            break;
            
        case ERR_INV_SIZE:
            res = fprintf(file, "ERR_INVALID_SIZE");
            break;

        case ERR_INV_ZERO:
            res = fprintf(file, "ERR_INVALID_ZERO_ELEM");
            break;

        case ERR_INV_HEAD:
            res = fprintf(file, "ERR_INVALID_HEAD");
            break;

        case ERR_INV_TAIL:
            res = fprintf(file, "ERR_INVALID_TAIL");
            break;

        case ERR_INV_NEXT:
            res = fprintf(file, "ERR_INVALID_NEXT");
            break;

        default:
            res = fprintf(file, "UNDEFINED ERROR");
    }
    return res;
}

int ListDump_(List *list, const char *reason, callInfo info) {
    assert(list);
    assert(list->nodes);

    system("mkdir -p dumps\n");

    char dotFileName[64] = "";
    sprintf(dotFileName, "dumps/dump%zu.dot", list->dumpNum);

    FILE *dotFile = fopen(dotFileName, "w");
    if (!dotFile) {
        PRINT_ERROR("Error opening file for dump : %s\n", strerror(errno));
        return ERR_DOT_FILE_OPN;
    }

    int errCode = ListError(list);

    fprintf(dotFile, "digraph list{\n"
                    "{\nrankdir=LR;\n"
                    "node[shape=plaintext];\nedge[color=white]\n"
                     "\"List<%s>[%p]\n dumped from %s() at %s (%d)\n\n", 
                             typeName, (void *)list, info.funcName,
							 info.file, info.line);

    if (errCode == 0) {
        fprintf(dotFile, "ok\n");

    } else {
        fprintf(dotFile, "Error ");
        writeErrToFile(dotFile, errCode);
        fprintf(dotFile, "\n");
    }

    fprintf(dotFile, "Constructed in %s() at %s (%d)\n"
                             "Dump reason : %s\"\n}\n",
                             list->ctorCallFuncName,
							 list->ctorCallFile, list->ctorCallLine, reason);

    fprintf(dotFile, "data [shape=record, label=\"{nodes | head | tail | free | size |"
            "capacity | isSorted } | { %p | <head> %zu | <tail> %zu | <free> %zu | %zu | %zu | %d }\"];",
            (void *)list->nodes, list->head, list->tail, list->free, list->size,
            list->capacity, list->isSorted);

    for (size_t i = 0; i < list->capacity; i++) {
        fprintf(dotFile, "node%zu [shape=record, label=\"{data\\n%lld |"
                "index\\n%zu} | next\\n%zu | prev\\n%zu\"];", i, 
                list->nodes[i].data, i, list->nodes[i].next, list->nodes[i].prev);
    }

    fprintf(dotFile, "\n{\nedge[color=white];\n");
    for (size_t i = 0; i < list->capacity - 1; i++) {
        fprintf(dotFile, "node%zu -> node%zu;\n", i, i+1); 
    }
    fprintf(dotFile, "}\n");

    fprintf(dotFile, "data:<head> -> node%zu[color=\"green\", label=\"head\"];\n", list->head);
    fprintf(dotFile, "data:<tail> -> node%zu[color=\"red\", label=\"tail\"];\n", list->tail);
    fprintf(dotFile, "data:<free> -> node%zu[color=\"yellow\", label=\"free\"];\n", list->free);

    for (size_t i = 1; i < list->capacity; i++) {
        fprintf(dotFile, "node%zu -> node%zu[label=\"next\"];\n", i, list->nodes[i].next); 

        if (list->nodes[i].prev != LST_SIZE_POISON) {
            fprintf(dotFile, "node%zu -> node%zu[label=\"prev\"];\n", i, list->nodes[i].prev); 
        }
    }

    fprintf(dotFile, "}\n");
    if (fclose(dotFile) == -1) {
        PRINT_ERROR("Error closing dot file : %s\n", strerror(errno));
        return ERR_FILE_CLS;
    }

    char command[128] = "";
    sprintf(command, "dot -Tjpg %s -o dumps/dump%zu.jpg", dotFileName, list->dumpNum);
    system(command);
    sprintf(command, "rm %s\n", dotFileName);
    system(command);

    list->dumpNum++;

    return 0;
}

int ListCtor_(List *list, size_t capacity, callInfo info) {
    assert(list);
    assert(list->nodes == nullptr);
    assert(list->head == 0);
    assert(list->tail == 0);

    system("rm -rf dumps\n");

    list->nodes = (node_t *)calloc((capacity + 1), sizeof(node_t));

    if (!list->nodes) {
        PRINT_ERROR("Error allocating memory for the list : %s\n", strerror(errno));
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

    list->ctorCallFuncName = info.funcName;
    list->ctorCallFile = info.file;
    list->ctorCallLine = info.line; 

    list->dumpNum = 0;

    ASSERT_OK(list);
    return 0;
}

void ListDtor(List *list) {
    ASSERT_OK(list);

    free(list->nodes);
    list->head = LST_SIZE_POISON;
    list->tail = LST_SIZE_POISON;
    list->free = LST_SIZE_POISON;
    list->size = LST_SIZE_POISON;
    list->capacity = LST_SIZE_POISON;
}

int ListSort_VERY_SLOWWWWWWWWWWWWWWWWWW(List *list) {
    ASSERT_OK(list);

    node_t *newNodes = (node_t *)calloc(list->capacity + 1, sizeof(node_t));

    if (!newNodes) {
        PRINT_ERROR("Error allocating memory for the new list : %s\n", strerror(errno));
        return ERR_NOMEM;
    }

    size_t cur = list->head;
    size_t newIdx = 1; 

    while (cur != list->tail) {
        newNodes[newIdx] = list->nodes[cur];
        newNodes[newIdx].prev = newIdx - 1;
        newNodes[newIdx].next = newIdx + 1;
        cur = list->nodes[cur].next;

        newIdx++;
    }
    newNodes[newIdx] = list->nodes[cur];
    newNodes[newIdx].next = 0;
    newNodes[newIdx].prev = newIdx - 1;

    list->free = list->size + 1;

    for (++newIdx; newIdx < list->capacity - 1; newIdx++) {
        newNodes[newIdx].next = newIdx + 1;
        newNodes[newIdx].prev = LST_SIZE_POISON;
    } 

    newNodes[newIdx].next = 0;
    newNodes[newIdx].prev = LST_SIZE_POISON;

    list->head = 1;
    list->tail = list->size;

    free(list->nodes);
    list->nodes = newNodes;

    list->isSorted = true;

    ASSERT_OK(list);
    return 0;
}

int ListResize(List *list, size_t newCap) {
    ASSERT_OK(list);

    node_t *newNodes = (node_t *)realloc(list->nodes,
            sizeof(node_t) * (newCap + 1));

    if (!newNodes) {
        PRINT_ERROR("Error allocating memory for the new list : %s\n", strerror(errno));
        return ERR_NOMEM;
    }

    list->free = list->capacity;
    newNodes[list->free].next = list->capacity + 1;

    size_t i = list->capacity;

    for (; i < newCap - 1; i++) {
        newNodes[i].data = 0;
        newNodes[i].prev = LST_SIZE_POISON;
        newNodes[i].next = i + 1;
    }

    newNodes[i].data = 0;
    newNodes[i].prev = LST_SIZE_POISON;
    newNodes[i].next = 0;

    list->nodes = newNodes;

    list->capacity = newCap;

    ASSERT_OK(list);
    return 0;
}

size_t ListLogicalToPhysicalIdx_DONT_CALL_SLOW_ASF(List *list, size_t idx) {
    ASSERT_OK(list);

    if (idx >= list->size) {
        return LST_SIZE_POISON;
    }

    if (list->isSorted) {
        return idx + 1;
    }

    size_t cur = list->head;

    while (idx--) {
        cur = list->nodes[cur].next;
    }

    ASSERT_OK(list);
    return cur;
}

size_t ListFindElem_SLOWWOLSSLOW(List *list, elem_t value) {
    ASSERT_OK(list);

    size_t cur = list->head;

    while (cur != 0) {
        if (list->nodes[cur].data == value) {
            ASSERT_OK(list);
            return cur;
        }
    }
    
    ASSERT_OK(list);
    return LST_SIZE_POISON;
}

int ListInsertFront(List *list, elem_t value) {
    ASSERT_OK(list);

    if (list->free == 0) {
        if (ListResize(list, list->capacity * 2) != 0) {
            return ERR_NOMEM;
        }
    }

    size_t nextFree = list->nodes[list->free].next;

    list->nodes[list->free].data = value;
    if (list->head != LST_SIZE_POISON) {
        list->nodes[list->head].prev = list->free;
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

    ASSERT_OK(list);
    return 0;
}

int ListInsertBack(List *list, elem_t value) {
    ASSERT_OK(list);

    if (list->free == 0) {
        if (ListResize(list, list->capacity * 2) != 0) {
            return ERR_NOMEM;
        }
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

    ASSERT_OK(list);
    return 0;
}

int ListInsertAfter(List *list, size_t pos, elem_t value) {
    ASSERT_OK(list);

    if (pos >= list->capacity || list->nodes[pos].prev == LST_SIZE_POISON) {
        return ERR_INV_POS;
    }

    if (pos == 0) {
        return ListInsertFront(list, value);

    } else if (pos == list->tail) {
        return ListInsertBack(list, value);

    } else {
        if (list->free == 0) {
            if (ListResize(list, list->capacity * 2) != 0) {
                return ERR_NOMEM;
            }
        }

        size_t nextFree = list->nodes[list->free].next;

        list->nodes[list->free].data = value;
        list->nodes[list->free].next = list->nodes[pos].next;
        list->nodes[list->free].prev = pos;

        list->nodes[pos].next = list->free;
        list->nodes[list->nodes[list->free].next].prev = list->free;

        list->free = nextFree;
    }
    list->size++;

    ASSERT_OK(list);
    return 0;
}

int ListInsertBefore(List *list, size_t pos, elem_t value) {
    ASSERT_OK(list);

    if (pos >= list->capacity || list->nodes[pos].prev == LST_SIZE_POISON) {
        return ERR_INV_POS;
    }

    if (pos == 0) {
        return ListInsertBack(list, value);

    } else if (pos == list->head) {
        return ListInsertFront(list, value);

    } else {
        if (list->free == 0) {
            if (ListResize(list, list->capacity * 2) != 0) {
                return ERR_NOMEM;
            }
        }

        size_t nextFree = list->nodes[list->free].next;

        list->nodes[list->free].data = value;
        list->nodes[list->free].next = pos;
        list->nodes[list->free].prev = list->nodes[pos].prev;

        list->nodes[pos].prev = list->free;
        list->nodes[list->nodes[list->free].prev].next = list->free;

        list->free = nextFree;
    }
    list->size++;

    ASSERT_OK(list);
    return 0;
}

int ListRemove(List *list, size_t pos) {
    ASSERT_OK(list);

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
        list->nodes[list->nodes[pos].next].prev = list->nodes[pos].prev;
    }

    list->nodes[pos].data = 0;
    list->nodes[pos].prev = LST_SIZE_POISON;

    list->nodes[pos].next = list->free;
    list->free = pos;

    list->size--;

    ASSERT_OK(list);
    return 0;
}
