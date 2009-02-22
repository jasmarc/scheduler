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
    int id,         // the id of the job
        arrive,     // the arrive time
        burst,      // the remaining burst time
        waiting,    // the cumulative wait time
        start,      // the start time
        end,        // the end time
        priority;   // the priority of the job
} job;

// Scheduler Algorithms (used by getopts parser)
enum {
  SJF = 0,
  FCFS,
  SRTF,
  RR,
  THE_END
};
// Scheduler Algorithms (used by getopts parser)
char *scheduler_opts[] = {
  [SJF]     = "sjf",
  [FCFS]    = "fcfs",
  [SRTF]    = "srtf",
  [RR]      = "rr",
  [THE_END] = NULL

};

// comparison routines
//     each takes two pointers to obects and compares them, returning
//     0 if they are equal, a negative value if a < b, or a positive
//     value if a > b
int fcfs_comparison(void *a, void *b);
int sjf_comparison(void *a, void *b);
int srtf_comparison(void *a, void *b);
int rr_comparison(void *a, void *b);

// processing routines
void build_job(job *j, int id, int arrive, int burst, int priority);
void increment_waits(heap *h);
void generate_jobs(heap *h, int (*comp_func)(void*, void*), int number_of_jobs);
void read_jobs_from_file(heap *h, int (*comp_func)(void*, void*), char *filename);
void process_jobs(int (*comp_func)(void*, void*), char *filename, int n, int verbose);

// printing routines
void print_jobs(heap *h);
void print_job(job *j);
void print_results(heap *c);
void print_usage(int argc, char *argv[]);

// compare based on arrive time, then id
int fcfs_comparison(void *a, void *b)
{
    int retval;
    retval = (((job*)b)->arrive - ((job*)a)->arrive);
    if(retval == 0) retval = (((job*)b)->id - ((job*)a)->id);
    return retval;
}

// compare based on remaining burst time, then by id
int sjf_comparison(void *a, void *b)
{
    int retval;
    retval = (((job*)b)->burst - ((job*)a)->burst);
    if(retval == 0) retval = (((job*)b)->id - ((job*)a)->id);
    return retval;
}

// compare based on remaining burst time (but with preemption), then by id
int srtf_comparison(void *a, void *b)
{
    int retval;
    retval = (((job*)b)->burst - ((job*)a)->burst);
    if(retval == 0) retval = (((job*)b)->id - ((job*)a)->id);
    return retval;
}

// compare based on priority first, then by arrive time, then by id
int rr_comparison(void *a, void *b)
{
    int retval;
    retval = (((job*)b)->priority - ((job*)a)->priority);
    if(retval == 0) retval = (((job*)b)->arrive - ((job*)a)->arrive);
    if(retval == 0) retval = (((job*)b)->id - ((job*)a)->id);
    return retval;
}

// populates a job based on given data
void build_job(job *j, int id, int arrive, int burst, int priority)
{
    j->id       = id;
    j->arrive   = arrive;
    j->burst    = burst;
    j->priority = priority;
    return;
}

// iterates through all jobs and increments their "wait" value
void increment_waits(heap *h)
{
    int i;
    for(i = 1; i <= h->size; i++)
        ((job*)(h->a[i]))->waiting++;
    return;
}

// generates random jobs and inserts them into the provided queue
void generate_jobs(heap *h, int (*comp_func)(void*, void*), int number_of_jobs)
{
    int i,
        arrive,
        burst,
        priority;

    for(srand(time(NULL)), i = 1, arrive = 0, h->size = 0; // initialize variables
        i <= number_of_jobs; // loop until i = the number_of_jobs specified
        i++, arrive += (rand() % 7)) // we increment "arrive" by a random amount
    {
        burst = 2 + (rand() % 5); // set burst to random number
        priority = rand() % 128; // set priority to random number
        job *temp = malloc(sizeof(job)); // create a job
        build_job(temp, i, arrive, burst, priority); // populate job
        heap_insert(h, comp_func, temp); // stick it in the queue
    }
    return;
}

// reads jobs from a CSV file. format is "arrive,burst,priority" one per line.
void read_jobs_from_file(heap *h, int (*comp_func)(void*, void*), char *filename)
{
    char buffer[256];
    int i = 0,
        arrive,
        burst,
        priority;
    FILE *fp = fopen(filename, "r");
    while (!feof(fp)) {
        fgets(buffer, 256, fp); // read a line

        // tokenize the line by commas and newlines
        arrive = strtol(strtok(buffer, ",\n"), NULL, 10);
        burst = strtol(strtok(NULL, ",\n"), NULL, 10);
        priority = strtol(strtok(NULL, ",\n"), NULL, 10);

        job *temp = malloc(sizeof(job)); // create a new job
        build_job(temp, i++, arrive, burst, priority); // populate it
        heap_insert(h, comp_func, temp); // stick it in the queue
    }
    fclose(fp);
    return;
}

// this is the heart of the scheduler
// comp_func    - a comparison function to use to determine priority
// filename     - a file to read data from
// n            - number of random jobs to generate (if we're not reading from file)
// verbose      - if true, will print job data for each CPU cycle
void process_jobs(int (*comp_func)(void*, void*), char *filename, int n, int verbose)
{
    int i; // this is the primary counter variable
    job *current = NULL; // this is the currently processing job

    heap *g = malloc(sizeof(heap)); // genereated queue - holds jobs either generated or read from file
    heap *p = malloc(sizeof(heap)); // process queue - holds jobs currently being processed
    heap *c = malloc(sizeof(heap)); // complete queue - hold jobs that have been processed
    heap_init(g);
    heap_init(p);
    heap_init(c);

    if(filename) // if we got a filename, lets read data from the file
        read_jobs_from_file(g, fcfs_comparison, filename);
    else // otherwise, we'll just generate some random data
        generate_jobs(g, fcfs_comparison, n);

    // print a nice title to show which algorithm we're using and the number of jobs
    char *algorithm_name = NULL;
    if(comp_func == &sjf_comparison) {
        algorithm_name = "Shortest Job First";
    } else if(comp_func == &fcfs_comparison) {
        algorithm_name = "First Come First Served";
    } else if(comp_func == &srtf_comparison) {
        algorithm_name = "Shortest Remaining Time First";
    } else
        algorithm_name = "Unknown";
    printf("*** %s ***\nNumber of jobs: %d\n", algorithm_name, g->size);
    print_jobs(g);

    for(i = 0, current = heap_extract_max(g, comp_func); // pull the first item out of the generated queue
        current != NULL; // go until we don't have a job to process!
        i++)
    {
        // grab the next "arrived" jobs out of the generated queue and put
        // them into the process queue
        job *insert;
        while((insert = heap_extract_max(g, fcfs_comparison)) && insert->arrive <= i) {
            // for round robin, newly arrived processes have same priority as current process
            if(comp_func == &rr_comparison) insert->priority = current->priority;
            // insert newly arrived process in process queue
            heap_insert(p, comp_func, insert);
        }
        if(insert && insert->arrive > i) // we might have pulled one too many out in the while loop
            heap_insert(g, fcfs_comparison, insert); // so put it back

        // print the job info if verbose mode
        if(verbose) {
            printf("clock: %2d\t", i);
            print_job(current);
        }

        current->burst--; // simulating "doing work" by decrementing the remaining burst time

        // if we're done with this job, then put it in the "complete" queue
        if(current->burst == 0) {
            current->end = i; // mark the end time for the outgoing job
            heap_insert(c, comp_func, current); // put the job in the "complete" queue
            current = heap_extract_max(p, comp_func); // grab the next job from the "process" queue
            if(current)
        current->start = i + 1; // mark the start time for the new job
        }
        // if this is srtf, then shove the current job back into the "process" queue
        // and re-evaluate everyone's remaining burst time and pull the next shortest
        // remaining burst-time job out, possibly preempting the current job
        if(comp_func == &srtf_comparison && current) {
            heap_insert(p, comp_func, current);
            current = heap_extract_max(p, comp_func);
        }
        // round robin preemption
        if(comp_func == &rr_comparison && current) {
            current->priority++;
            heap_insert(p, comp_func, current);
            current = heap_extract_max(p, comp_func);
        }
        increment_waits(p); // for all "waiting" jobs, increment their "wait" value
    }
    if(verbose) print_jobs(c); // print all jobs when done if verbose
    print_results(c); // print all algorithm analysis results
    return;
}

// print all jobs in the queue
void print_jobs(heap *h)
{
    int i;
    for(i = 1; i <= h->size; i++)
        print_job(h->a[i]);
    return;
}

// print the given job
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

// print all analytical results
void print_results(heap *c)
{
    int j,
        sum_waiting = 0;
    for(j = 1; j <= c->size; j++)
        sum_waiting += ((job*)(c->a[j]))->waiting;
    printf("average waiting time: %3.2f\n", (float)sum_waiting / (float)c->size);
    return;
}

// print the command line usage
void print_usage(int argc, char *argv[])
{
    printf("usage:\t\t%s [OPTIONS]\n", argv[0]);
    printf("example:\t%s -i data.txt -s sjf,fcfs,srtf -v\n", argv[0]);
    printf("\t\t%s -n 5 -s sjf\n", argv[0]);
    printf("options:\n");
    printf(" -h\t\tPrint this message.\n");
    printf(" -i <file>\tRead comma-separated file with arrive,burst,priority\n");
    printf(" -n <number>\tNumber of jobs to generate if not reading from file.\n");
    printf(" -s <sched(s)>\tSpecify scheduler(s) to use.\n");
    printf(" \t\tValid schedulers are: sjf, fcfs, srtf, rr\n");
    printf(" -v\t\tVerbose mode. Prints an output for each CPU cycle.\n");
    return;
}
