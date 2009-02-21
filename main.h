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

int fcfs_comparison(void *a, void *b);
int sjf_comparison(void *a, void *b);
void build_job(job *j, int id, int arrive, int burst, int waiting, int end, int priority);
void generate_jobs(heap *h, int (*comp_func)(void*, void*), int number_of_jobs);
void process_jobs(heap *h, int (*comp_func)(void*, void*));
void increment_wait_time(heap *h);
void print_job(job *j);
void print_jobs(heap *h);
void print_results(heap *c);