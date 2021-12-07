#ifndef SORT_H
#define SORT_H

#include <sys/types.h>

//------------------------------------------------------------ 
//! Swaps values of variables pointed to by pointers a and b
//!
//! @param[out] a       Pointer to first value
//! @param[out] b       Pointer to second value
//! @param[in]  size    Size of the type of variables a and b 
//------------------------------------------------------------ 
void swap(void *a, void *b, size_t size);

//------------------------------------------------------------ 
//! Builds a max heap with an array representation
//!
//! @param[out] array   Array with data to build the heap from
//! @param[in]  n       Size of the heap
//! @param[in]  i       Index of the array at which the subtree is rooted
//! @param[in]  size    Size of the type of variables stored in array
//! @param[in]  cmp     Comparator function that returns positive values 
//!                     if the left value is larger, 0 if the values are equal
//------------------------------------------------------------ 
void heapify(void *array, size_t n, size_t i, size_t size, int (*cmp)(const void *, const void *));

//------------------------------------------------------------ 
//! Sorts data in base array using heap sort algorithm
//!
//! @param[out] base    Array with data to sort
//! @param[in]  nmemb   Number of members in the array
//! @param[in]  size    Size of the type of variables stored in array
//! @param[in]  cmp     Comparator function that returns positive values 
//!                     if the left value is larger, 0 if the values are equal
//------------------------------------------------------------ 
void myHeapSort(void *base, size_t nmemb, size_t size, int (*cmp)(const void *, const void *));

#endif
