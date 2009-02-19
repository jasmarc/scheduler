#include <stdio.h>
#include <stdarg.h>
#include "heap.h"

void build_job(job *j, int id, int arrive, int burst, int waiting, int end, int priority)
{
    j->id = id;
    j->arrive = arrive;
    j->burst = burst;
    j->waiting = waiting;
    j->end = end;
    j->priority = priority;
    return;
}

void heapify(job *a[MAX_HEAP_SIZE], int (*comp_func)(job*, job*), int i)
{
    job *temp;
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
        SWAP(a[i], a[largest], temp);
        heapify(a, comp_func, largest);
    }
    return;
}

void build_heap(job *a[MAX_HEAP_SIZE], int (*comp_func)(job*, job*))
{
    int i;
    for(i = HEAPSIZE(a)/2; i > 0; i--)
        heapify(a, comp_func, i);
    return;
}

void heap_sort(job *a[MAX_HEAP_SIZE], int (*comp_func)(job*, job*))
{
    job *temp;
    int i;
    build_heap(a, comp_func);
    for(i = HEAPSIZE(a); i > 1; i--) {
        SWAP(a[1], a[i], temp);
        HEAPSIZE(a)--;
        heapify(a, comp_func, 1);
    }
    return;
}

job* heap_extract_max(job *a[MAX_HEAP_SIZE], int (*comp_func)(job*, job*))
{
    if(HEAPSIZE(a) < 1)
        fprintf(stderr, "heap underflow.\n");
    job *max = a[1];
    a[1] = a[HEAPSIZE(a)];
    HEAPSIZE(a)--;
    heapify(a, comp_func, 1);
    return max;
}

void heap_insert(job *a[MAX_HEAP_SIZE], int (*comp_func)(job*, job*), job *key)
{
    int i = ++HEAPSIZE(a);
    while(i > 1 && comp_func(a[PARENT(i)], key) < 0) {
        a[i] = a[PARENT(i)];
        i = PARENT(i);
    }
    a[i] = key;
    return;
}
