#define MAX_HEAP_SIZE 10
#define PARENT(i) (i/2)
#define LEFT(i) (2*i)
#define RIGHT(i) (2*i + 1)
#define HEAPSIZE(a) (a[0])
#define SWAPINT(a, b)  {a^=b; b^=a; a^=b;}
#define SWAP(a, b, temp)  {temp=a; a=b; b=temp;}

void heapify(int a[MAX_HEAP_SIZE], int (*comp_func)(int, int), int i);
void build_heap(int a[MAX_HEAP_SIZE], int (*comp_func)(int, int));
void heap_sort(int a[MAX_HEAP_SIZE], int (*comp_func)(int, int));
int heap_extract_max(int a[MAX_HEAP_SIZE], int (*comp_func)(int, int));
void heap_insert(int a[MAX_HEAP_SIZE], int (*comp_func)(int, int), int key);