#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "heap.h"

void print_job(job *j);
int fcfs_comparision(job *a, job *b);

int main (int argc, char const *argv[])
{
    int i;
    job *a[10];
    job *temp;
    srand(1);
    printf("heapsize: %d\n", HEAPSIZE(a));
    for(i = 1; i < 10; i++) {
        temp = malloc(sizeof(job));
        build_job(temp, i, rand() % 100, rand() % 100, 0, 0, 0);
        heap_insert(a, fcfs_comparision, temp);
    }
    printf("heapsize: %d\n", HEAPSIZE(a));
    for(i = 1; i < 10; i++) {
        print_job(heap_extract_max(a, fcfs_comparision));
    }
    return 0;
}

void print_job(job *j)
{
    printf("id: %3d\tarrive: %3d\tburst: %3d\twaiting: %3d\tend: %3d\tpriority: %3d\n", 
        j->id,
        j->arrive,
        j->burst,
        j->waiting,
        j->end,
        j->priority);
    return;
}

int fcfs_comparision(job *a, job *b)
{
    return (b->arrive - a->arrive);
}