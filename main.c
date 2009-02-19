#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "heap.h"

void print_job(job *j);
void generate_jobs(job *a[MAX_HEAP_SIZE], int number_of_jobs);
int fcfs_comparision(job *a, job *b);
void increment_wait_time(job *a[MAX_HEAP_SIZE]);

int main (int argc, char const *argv[])
{
    job *a[6];
    generate_jobs(a, 5);
/* 	*temp = heap_extract_max(a, fcfs_comparision); */
/*     int i; */
/*     generate_jobs(a, 6); */
/*     do { */
/* 	increment_wait_time(a); */
/* 	printf("clock: %3d\t", i); */
/* 	print_job(temp); */
/* 	temp->burst--; */
/* 	i++; */
/* 	if(temp->burst == 0) */
/* 	    temp = heap_extract_max(a, fcfs_comparision); */
/*     } while(HEAPSIZE(a) > 0 && temp->burst > 0); */
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

void generate_jobs(job *a[MAX_HEAP_SIZE], int number_of_jobs)
{
    int i,
	arrive_time, 
	burst_time;
    job *temp;
    a[0] = malloc(sizeof(int));
    for(srand(i), i = 1, arrive_time = 0, HEAPSIZE(a) = 0; i <= number_of_jobs; i++) {
        temp = malloc(sizeof(job));
	burst_time = 2 + (rand() % 15);
        build_job(temp, i, arrive_time, burst_time, 0, 0, 0);
	arrive_time += (rand() % 5);
        heap_insert(a, fcfs_comparision, temp);
    }
    return;
}

void increment_wait_time(job *a[MAX_HEAP_SIZE])
{
    int i;
    for(i = 1; i <= HEAPSIZE(a); i++)
	a[i]->waiting++;
    return;
}

int fcfs_comparision(job *a, job *b)
{
    return (b->arrive - a->arrive);
}
