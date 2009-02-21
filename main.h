#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
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

enum {
  SJF = 0,
  FCFS,
  THE_END
};

const char *scheduler_opts[] = {
  [SJF] = "sjf",
  [FCFS] = "fcfs",
  [THE_END] = NULL
};

void build_job(job *j, int id, int arrive, int burst, int priority);
void increment_waits(heap *h);
void generate_jobs(heap *h, int (*comp_func)(void*, void*), int number_of_jobs);
void read_jobs_from_file(heap *h, int (*comp_func)(void*, void*), FILE *fp);
void process_jobs(heap *h, int (*comp_func)(void*, void*), heap *c);
void print_jobs(heap *h);
void print_job(job *j);
void print_results(heap *c);
void print_usage(int argc, char *argv[]);

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

    for(srand(time(NULL)), i = 1, arrive = 0, h->size = 0;
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

void read_jobs_from_file(heap *h, int (*comp_func)(void*, void*), FILE *fp)
{
    char buffer[256];
    int i = 0,
        arrive,
        burst,
        priority;

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
    printf("id: %2d\tarrive: %2d\tburst: %2d\twaiting: %2d\tstart: %2d\tend: %2d\tpriority: %3d\n",
        j->id,
        j->arrive,
        j->burst,
        j->waiting,
        j->start,
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

void print_usage(int argc, char *argv[])
{
    printf("usage: %s [OPTIONS]\n", argv[0]);
    printf("\t-i <file>\t\tRead comma-separated file with arrive,burst,priority\n");
    printf("\t-s <scheduler(s)>\tSpecify scheduler(s) to use.\n");
    printf("\t\t\t\tValid schedulers are: sjf, fcfs\n");
    return;
}
