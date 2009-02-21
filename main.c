#include "main.h"

int fcfs_comparison(void *a, void *b)
{
    return (((job*)b)->arrive - ((job*)a)->arrive);
}

int sjf_comparison(void *a, void *b)
{
    return (((job*)b)->burst - ((job*)a)->burst);
}

int main (int argc, char const *argv[])
{
    heap *h = malloc(sizeof(heap));
    heap *c = malloc(sizeof(heap));
    heap_init(h);
    heap_init(c);
//    generate_jobs(h, fcfs_comparison, 5);
    read_jobs_from_file(h, fcfs_comparison, "data.txt");
    printf("number of jobs: %d\n", h->size);
    print_jobs(h);
    process_jobs(h, fcfs_comparison, c);
    print_results(c);
    return 0;
}