#include <stdio.h>
#include <stdarg.h>
#include "heap.h"

void heapify(int a[MAX_HEAP_SIZE], int (*comp_func)(int, int), int i)
{
    int largest;
    int l = LEFT(i);
    int r = RIGHT(i);
    if(l <= HEAPSIZE(a) && comp_func(a[l], a[i]) > 0)
        largest = l;
    else
        largest = i;
    if(r <= HEAPSIZE(a) && comp_func(a[r], a[largest]) > 0)
        largest = r;
    if(largest != i) {
        SWAPINT(a[i], a[largest]);
        heapify(a, comp_func, largest);
    }
    return;
}

void build_heap(int a[MAX_HEAP_SIZE], int (*comp_func)(int, int))
{
    int i;
    for(i = HEAPSIZE(a)/2; i > 0; i--)
        heapify(a, comp_func, i);
    return;
}

void heap_sort(int a[MAX_HEAP_SIZE], int (*comp_func)(int, int))
{
    int i;
    build_heap(a, comp_func);
    for(i = HEAPSIZE(a); i > 1; i--) {
        SWAPINT(a[1], a[i]);
        HEAPSIZE(a)--;
        heapify(a, comp_func, 1);
    }
    return;
}

int heap_extract_max(int a[MAX_HEAP_SIZE], int (*comp_func)(int, int))
{
    if(HEAPSIZE(a) < 1)
        fprintf(stderr, "heap underflow.\n");
    int max = a[1];
    a[1] = a[HEAPSIZE(a)];
    HEAPSIZE(a)--;
    heapify(a, comp_func, 1);
    return max;
}

void heap_insert(int a[MAX_HEAP_SIZE], int (*comp_func)(int, int), int key)
{
    int i = ++HEAPSIZE(a);
    while(i > 1 && comp_func(a[PARENT(i)], key) < 0) {
        a[i] = a[PARENT(i)];
        i = PARENT(i);
    }
    a[i] = key;
    return;
}