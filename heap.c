#include "heap.h"

void heapify(heap *h, int (*comp_func)(void*, void*), int i)
{
    void *temp = NULL;
    int largest,
        l = LEFT(i),
        r = RIGHT(i);
    if(l <= h->size && comp_func(h->a[l], h->a[i]) > 0)
        largest = l;
    else
        largest = i;
    if(r <= h->size && comp_func(h->a[r], h->a[largest]) > 0)
        largest = r;
    if(largest != i) {
        SWAP(h->a[i], h->a[largest], temp);
        heapify(h, comp_func, largest);
    }
    return;
}

void build_heap(heap *h, int (*comp_func)(void*, void*))
{
    int i;
    for(i = h->size/2; i > 0; i--)
        heapify(h, comp_func, i);
    return;
}

void heap_sort(heap *h, int (*comp_func)(void*, void*))
{
    void *temp = NULL;
    int i;
    build_heap(h, comp_func);
    for(i = h->size; i > 1; i--) {
        SWAP(h->a[1], h->a[i], temp);
        h->size--;
        heapify(h, comp_func, 1);
    }
    return;
}

void* heap_extract_max(heap *h, int (*comp_func)(void*, void*))
{
    if(h->size < 1)
        fprintf(stderr, "heap underflow.\n");
    void *max = h->a[1];
    h->a[1] = h->a[h->size];
    h->size--;
    heapify(h, comp_func, 1);
    return max;
}

void heap_insert(heap *h, int (*comp_func)(void*, void*), void *key)
{
    int i = ++h->size;
    while(i > 1 && comp_func(h->a[PARENT(i)], key) < 0) {
        h->a[i] = h->a[PARENT(i)];
        i = PARENT(i);
    }
    h->a[i] = key;
    return;
}
