#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#ifndef __FUNCTION_NAME__
    #ifdef WIN32   //WINDOWS
        #define __FUNCTION_NAME__   __FUNCTION__  
    #else          //*NIX
        #define __FUNCTION_NAME__   __func__ 
    #endif
#endif

#define PRINT_ERROR(...) {                                                                                  \
    fprintf(stderr, __VA_ARGS__);                                                                           \
    fprintf(stderr, "file \"%s\", line %d, function \"%s()\"\n", __FILE__, __LINE__, __FUNCTION_NAME__);  \
}                                                                                                           \

#define STRINGIFY(name) #name

typedef long long elem_t;

//! String of the type name for dumping
static const char *typeName = "long long";

//! Value to insert into invalid elements
const size_t LST_SIZE_POISON = SIZE_MAX;

//! Defines the list node type
struct node_t {
    elem_t data;
    size_t next;
    size_t prev;
};

//! Doubly linked array-based list
struct List {
    node_t *nodes; /**< Array of list nodes */
    size_t head; /**< Index of first list element */
    size_t tail; /**< Index of last list element */ 
    size_t free; /**< Index of the first free list element */
    size_t size; /**< Current number of elements in the list */
    size_t capacity; /**< Full capacity of the node array */
    bool isSorted; /**< Signifies if the list is currently sorted, i.e. elements are placed conguously in the beginning of the list */

    const char *ctorCallFuncName; /**< Function that called the stack constructor */
    const char *ctorCallFile; /**< File of the constructor call */
    int ctorCallLine; /**< Line of the constructor call */

    size_t dumpNum; /**< Current dump number */
};

enum ListError : int {
    NO_ERR = 0,
    ERR_NOMEM = 1,
    ERR_OVFLOW = 2,
    ERR_INV_POS = 3,
    ERR_DOT_FILE_OPN = 4,
    ERR_FILE_CLS = 5,
    ERR_NULL = 6,
    ERR_NULL_ND = 7,
    ERR_CAP_OVERFL = 8,
    ERR_INV_SIZE = 9,
    ERR_INV_ZERO = 10,
    ERR_INV_HEAD = 11,
    ERR_INV_TAIL = 12,
    ERR_INV_NEXT = 13,
};

static const char *errorStrs[] = {
    STRINGIFY(NO_ERR),
    STRINGIFY(ERR_NOMEM),
    STRINGIFY(ERR_OVFLOW),
    STRINGIFY(ERR_INV_POS),
    STRINGIFY(ERR_DOT_FILE_OPN),
    STRINGIFY(ERR_FILE_CLS),
    STRINGIFY(ERR_NULL),
    STRINGIFY(ERR_NULL_ND),
    STRINGIFY(ERR_CAP_OVERFL),
    STRINGIFY(ERR_INV_SIZE),
    STRINGIFY(ERR_INV_ZERO),
    STRINGIFY(ERR_INV_HEAD),
    STRINGIFY(ERR_INV_TAIL),
    STRINGIFY(ERR_INV_NEXT),
};

//! Encapsulates info about function call for debug
struct callInfo {
    const char *funcName; /**< Name of the calling function */
    const char *file; /**< File where the function call happened */
    int line; /**< Line of the function call in the file */
};

int ListCtor_(List *list, size_t capacity, callInfo info);

void ListDtor(List *list);

typedef int (*compFunc)(const void *, const void *);

typedef void (*sortFunc)(void *, size_t, size_t, compFunc);

int ListNodeComp(const void *lhs, const void *rhs);

int ListSortByValue(List *list, sortFunc); 

int ListSort(List *list);

int ListResize(List *list, size_t newCap);

size_t ListLogicalToPhysicalIdx(List *list, size_t idx);

size_t ListFindElem(List *list, elem_t value);

int ListInsertFront(List *list, elem_t value);

int ListInsertBack(List *list, elem_t value);

int ListInsertBefore(List *list, size_t pos, elem_t value);

int ListInsertAfter(List *list, size_t pos, elem_t value);

int ListRemove(List *list, size_t pos);

int ListDump_(List *list, const char *reason, callInfo info);

int ListErrorCheck(List *list);

void writeListErrToFile(FILE *file, int err);

#define ASSERT_OK(list)                            \
    do {                                            \
        int ret = ListErrorCheck(list);                \
        if (ret != 0) {                             \
            ListDump(list, "ASSERT_OK failed");   \
            assert(!"Invariant failure");           \
        }                                           \
    } while (0)

#define ListCtor(list, capacity)    \
do {                                           \
    callInfo inf = {};                         \
    inf.funcName = __FUNCTION_NAME__;          \
    inf.file = __FILE__;                       \
    inf.line = __LINE__;                       \
    ListCtor_(list, capacity, inf); \
} while (0)

#define ListDump(list, reason)      \
do {                                           \
    callInfo inf = {};                         \
    inf.funcName = __FUNCTION_NAME__;          \
    inf.file = __FILE__;                       \
    inf.line = __LINE__;                       \
    ListDump_(list, reason, inf);   \
} while (0)

#endif
