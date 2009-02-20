#define MAX_HEAP_SIZE 10
#define PARENT(i) (i/2)
#define LEFT(i) (2*i)
#define RIGHT(i) (2*i + 1)
#define SWAP(a, b, temp)  {temp=a; a=b; b=temp;}

typedef struct {
    void *a[MAX_HEAP_SIZE];
    int size;
} heap;

void heapify(heap *h, int (*comp_func)(void*, void*), int i);
void build_heap(heap *h, int (*comp_func)(void*, void*));
void heap_sort(heap *h, int (*comp_func)(void*, void*));
void* heap_extract_max(heap *h, int (*comp_func)(void*, void*));
void heap_insert(heap *h, int (*comp_func)(void*, void*), void *key);

