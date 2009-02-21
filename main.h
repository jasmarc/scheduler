#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <inttypes.h>
#include <sys/types.h>
#include <limits.h>
#include <string.h>
#include "heap.h"

typedef struct {
    int id,
        arrive,
        burst,
        waiting,
        start,
        end,
        priority;
} job;

void build_job(job *j, int id, int arrive, int burst, int priority);
void increment_waits(heap *h);
void generate_jobs(heap *h, int (*comp_func)(void*, void*), int number_of_jobs);
void read_jobs_from_file(heap *h, int (*comp_func)(void*, void*), char* filename);
void process_jobs(heap *h, int (*comp_func)(void*, void*), heap *c);
void print_jobs(heap *h);
void print_job(job *j);
void print_results(heap *c);

void build_job(job *j, int id, int arrive, int burst, int priority)
{
    j->id = id;
    j->arrive = arrive;
    j->burst = burst;
    j->priority = priority;
    return;
}

void increment_waits(heap *h)
{
    int i;
    for(i = 1; i <= h->size; i++)
        ((job*)(h->a[i]))->waiting++;
    return;
}

void generate_jobs(heap *h, int (*comp_func)(void*, void*), int number_of_jobs)
{
    int i,
        arrive,
        burst,
        priority;

    for(srand(i), i = 1, arrive = 0, h->size = 0;
        i <= number_of_jobs;
        i++, arrive += (rand() % 7))
    {
        burst = 2 + (rand() % 5);
        priority = rand() % 128;
        job *temp = malloc(sizeof(job));
        build_job(temp, i, arrive, burst, priority);
        heap_insert(h, comp_func, temp);
    }
    return;
}

void read_jobs_from_file(heap *h, int (*comp_func)(void*, void*), char* filename) 
{
    char buffer[256];
    int i = 0,
        arrive,
        burst,
        priority;
        
    FILE *fp = fopen(filename, "r");
    while (!feof(fp)) {
        fgets(buffer, 256, fp);
        arrive = strtol(strtok(buffer, ",\n"), NULL, 10);
        burst = strtol(strtok(NULL, ",\n"), NULL, 10);
        priority = strtol(strtok(NULL, ",\n"), NULL, 10);
        job *temp = malloc(sizeof(job));
        build_job(temp, i, arrive, burst, priority);
        heap_insert(h, comp_func, temp);
    }
    fclose(fp);
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
        increment_waits(h);
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