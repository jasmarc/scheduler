#include "main.h"

int main (int argc, char const *argv[])
{
    heap *h = malloc(sizeof(heap));
    heap_init(h);
    generate_jobs(h, fcfs_comparison, 5);

    printf("number of jobs: %d\n", h->size);

    print_jobs(h);
    process_jobs(h, fcfs_comparison);
    print_jobs(h);

    int i;
    for(i = 1; i <= 5; i++)
        print_job(h->a[i]);

    return 0;
}

int fcfs_comparison(void *a, void *b)
{
    return (((job*)b)->arrive - ((job*)a)->arrive);
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

void process_jobs(heap *h, int (*comp_func)(void*, void*))
{
    int i;
    job *current = NULL;
    for(i = 0, current = heap_extract_max(h, comp_func);
        current != NULL;
        i++)
    {
        increment_wait_time(h);
        printf("clock: %2d\t", i);
        print_job(current);
        current->burst--;
        if(current->burst == 0)
            current = heap_extract_max(h, comp_func);
    }
    return;
}

void increment_wait_time(heap *h)
{
    int i;
    for(i = 1; i <= h->size; i++)
        ((job*)(h->a[i]))->waiting++;
    return;
}

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