#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdint.h>

#ifndef __FUNCTION_NAME__
    #ifdef WIN32   //WINDOWS
        #define __FUNCTION_NAME__   __FUNCTION__  
    #else          //*NIX
        #define __FUNCTION_NAME__   __func__ 
    #endif
#endif

typedef long long elem_t;

static const char *typeName = "long long";

const size_t LST_SIZE_POISON = SIZE_MAX;

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
    size_t size;
    size_t capacity;
    bool isSorted;

    const char *ctorCallFuncName; /**< Function that called the stack constructor */
    const char *ctorCallFile; /**< File of the constructor call */
    int ctorCallLine; /**< Line of the constructor call */

    size_t dumpNum;
};

enum ListError : int {
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

//! Encapsulates info about function call for debug
struct callInfo {
    const char *funcName; /**< Name of the calling function */
    const char *file; /**< File where the function call happened */
    int line; /**< Line of the function call in the file */
};

int ListCtor_(List *list, size_t capacity, callInfo info);

void ListDtor(List *list);

int ListSort_VERY_SLOWWWWWWWWWWWWWWWWWW(List *list);

int ListResize(List *list, size_t newCap);

size_t ListLogicalToPhysicalIdx_DONT_CALL_SLOW_ASF(List *list, size_t idx);

int ListInsertFront(List *list, elem_t value);

int ListInsertBack(List *list, elem_t value);

int ListInsertAfter(List *list, size_t pos, elem_t value);

int ListRemove(List *list, size_t pos);

int ListDump_(List *list, const char *reason, callInfo info);

int ListError(List *list);

#define ASSERT_OK(list)                            \
    do {                                            \
        int ret = ListError(list);                \
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
