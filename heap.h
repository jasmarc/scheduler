#include <stdio.h>
#include <stdarg.h>

#define MAX_HEAP_SIZE 1000
#define PARENT(i) (i/2)     // given the index of a heap node, the returns the parent index
#define LEFT(i) (2*i)       // given the index of a heap node, the returns the left child index
#define RIGHT(i) (2*i + 1)  // given the index of a heap node, the returns the right child index
#define SWAP(a, b, temp)  {temp=a; a=b; b=temp;}

typedef struct {
    void *a[MAX_HEAP_SIZE];
    int size;
} heap;

void heap_init(heap *h);
void heapify(heap *h, int (*comp_func)(void*, void*), int i);
void build_heap(heap *h, int (*comp_func)(void*, void*));
void* heap_extract_max(heap *h, int (*comp_func)(void*, void*));
void heap_insert(heap *h, int (*comp_func)(void*, void*), void *key);
