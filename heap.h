#define MAX_HEAP_SIZE 10
#define PARENT(i) (i/2)
#define LEFT(i) (2*i)
#define RIGHT(i) (2*i + 1)
#define HEAPSIZE(a) ((int)a[0])
#define SWAPINT(a, b)  {a^=b; b^=a; a^=b;}
#define SWAP(a, b, temp)  {temp=a; a=b; b=temp;}

typedef struct {
    int id,
        arrive,
        burst,
        waiting,
        end,
        priority;
} job;

void build_job(job *j, int id, int arrive, int burst, int waiting, int end, int priority);
void heapify(job *a[MAX_HEAP_SIZE], int (*comp_func)(job*, job*), int i);
void build_heap(job *a[MAX_HEAP_SIZE], int (*comp_func)(job*, job*));
void heap_sort(job *a[MAX_HEAP_SIZE], int (*comp_func)(job*, job*));
job* heap_extract_max(job *a[MAX_HEAP_SIZE], int (*comp_func)(job*, job*));
void heap_insert(job *a[MAX_HEAP_SIZE], int (*comp_func)(job*, job*), job *key);

