#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "heap.h"

typedef struct {
    int id,
        arrive,
        burst,
        waiting,
        end,
        priority;
} job;

void build_job(job *j, int id, int arrive, int burst, int waiting, int end, int priority);
void increment_wait_times(heap *h);
void generate_jobs(heap *h, int (*comp_func)(void*, void*), int number_of_jobs);
void process_jobs(heap *h, int (*comp_func)(void*, void*), heap *c);
void print_jobs(heap *h);
void print_job(job *j);
void print_results(heap *c);

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

void increment_wait_times(heap *h)
{
    int i;
    for(i = 1; i <= h->size; i++)
        ((job*)(h->a[i]))->waiting++;
    return;
}

void generate_jobs(heap *h, int (*comp_func)(void*, void*), int number_of_jobs)
{
    int i,
        arrive_time,
        burst_time;

    for(srand(i), i = 1, arrive_time = 0, h->size = 0;
        i <= number_of_jobs;
        i++, arrive_time += (rand() % 7))
    {
        job *temp = malloc(sizeof(job));
        burst_time = 2 + (rand() % 5);
        build_job(temp, i, arrive_time, burst_time, 0, 0, 0);
        heap_insert(h, comp_func, temp);
    }
    return;
}

void process_jobs(heap *h, int (*comp_func)(void*, void*), heap *c)
{
    int i;
    job *current = NULL;
    for(i = 0, current = heap_extract_max(h, comp_func);
        current != NULL;
        i++)
    {
        increment_wait_times(h);
        printf("clock: %2d\t", i);
        print_job(current);
        current->burst--;
        if(current->burst == 0) {
            heap_insert(c, comp_func, current);
            current = heap_extract_max(h, comp_func);
        }
    }
    return;
}

void print_jobs(heap *h)
{
    int i;
    for(i = 1; i <= h->size; i++)
        print_job(h->a[i]);
    return;
}

void print_job(job *j)
{
    printf("id: %2d\tarrive: %2d\tburst: %2d\twaiting: %2d\tend: %2d\tpriority: %2d\n",
        j->id,
        j->arrive,
        j->burst,
        j->waiting,
        j->end,
        j->priority);
    return;
}

void print_results(heap *c)
{
    int j, 
        sum_waiting = 0;
    for(j = 1; j <= c->size; j++)
        sum_waiting += ((job*)(c->a[j]))->waiting;
    printf("average waiting time: %3.2f\n", (float)sum_waiting / (float)c->size);
    return;
}