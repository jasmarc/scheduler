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

#define TRUE  1
#define FALSE 0

typedef struct {
    int id,         // the id of the job
        arrive,     // the arrive time
        burst,      // the remaining burst time
        waiting,    // the cumulative wait time
        start,      // the start time
        end,        // the end time
        priority,   // the priority of the job
        service;    // most recent service time
} job;

// Scheduler Algorithms (used by getopts parser)
enum {
  SJF = 0,
  FCFS,
  SRTF,
  RR,
  UNIX,
  THE_END
};
// Scheduler Algorithms (used by getopts parser)
char *scheduler_opts[] = {
  [SJF]     = "sjf",
  [FCFS]    = "fcfs",
  [SRTF]    = "srtf",
  [RR]      = "rr",
  [UNIX]    = "unix",
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
int id_comparison(void *a, void *b);

// processing routines
void build_job(job *j, int id, int arrive, int burst);
void increment_waits(heap *h);
void generate_jobs(heap *h, int (*comp_func)(void*, void*), int number_of_jobs);
void read_jobs_from_file(heap *h, int (*comp_func)(void*, void*), char *filename);
void process_jobs(int (*comp_func)(void*, void*), char *filename, int n, int verbose);

// printing routines
void print_title(int (*comp_func)(void*, void*));
void print_job(job *j);
void print_results(heap *c, int verbose);
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

// compare based on priority first, then by arrive time, then by id
int unix_comparison(void *a, void *b)
{
    int retval;
    retval = (((job*)b)->priority - ((job*)a)->priority);
    if(retval == 0) retval = (((job*)b)->arrive - ((job*)a)->arrive);
    if(retval == 0) retval = (((job*)b)->id - ((job*)a)->id);
    return retval;
}

// compare based on id (for administrative purposes only)
int id_comparison(void *a, void *b)
{
    return (((job*)b)->id - ((job*)a)->id);
}

// populates a job based on given data
void build_job(job *j, int id, int arrive, int burst)
{
    j->id       = id;
    j->arrive   = arrive;
    j->burst    = burst;
    j->priority = 0;
    j->waiting  = 0;
    j->start    = -1;
    j->end      = 0;
    j->service  = 0;
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

// iterates through all jobs and recalculates priorites (for unix scheduler only).
// clarification from instructor, Saurav Karmakar, below:
// The calculation of CPU usage is just calculation of frequency of
// usage. Suppose the base=60 and assume that recent CPU usage for
// process P1 is 40, process P2 is 18, and process P3 is 10 at time
// t0(i.e P1 has used CPU 40 times since last time you checked and so
// on). Now what will be the new priorities for these three processes
// when priorities are recalculated, say at t1?
//  Priority for P1= (40/2)+60=80
//  Priority for P2= (18/2)+60=69
//  Priority for P1= (10/2)+60=65
void recalculate_priorities(heap *h, int current_time)
{
    int i;
    job *j = NULL;
    for(i = 1; i <= h->size; i++) {
        j = (job*)(h->a[i]);
        j->priority = (j->service)/2 + 60;
    }
    return;
}

// generates random jobs and inserts them into the provided queue
void generate_jobs(heap *h, int (*comp_func)(void*, void*), int number_of_jobs)
{
    int i,
        arrive,
        burst;

    for(srand(time(NULL)), i = 1, arrive = 0, h->size = 0; // initialize variables
        i <= number_of_jobs; // loop until i = the number_of_jobs specified
        i++, arrive += (rand() % 7)) // we increment "arrive" by a random amount
    {
        burst = 2 + (rand() % 5); // set burst to random number
        job *temp = malloc(sizeof(job)); // create a job
        build_job(temp, i, arrive, burst); // populate job
        heap_insert(h, comp_func, temp); // stick it in the queue
    }
    return;
}

// reads jobs from a CSV file. format is "arrive,burst,priority" one per line.
void read_jobs_from_file(heap *h, int (*comp_func)(void*, void*), char *filename)
{
    char buffer[256];
    char *temp;
    int i           = 0,
        arrive      = 0,
        burst       = 0;
    FILE *fp        = NULL;
    if((fp = fopen(filename, "r"))) {
        while (!feof(fp)) {
            fgets(buffer, 256, fp); // read a line

            // tokenize the line by commas and newlines
            if(strlen(buffer) > 1) {        
                if((temp = strtok(buffer, ",\n")) != NULL && strlen(temp) > 0)
                    arrive += strtol(temp, NULL, 10);
                if((temp = strtok(NULL, ",\n")) != NULL && strlen(temp) > 0)
                    burst = strtol(temp, NULL, 10);
                job *temp = malloc(sizeof(job)); // create a new job
                build_job(temp, i++, arrive, burst); // populate it
                heap_insert(h, comp_func, temp); // stick it in the queue
            }
        }
        fclose(fp);
    }
    else {
        fprintf(stderr, "error opening file %s\n", filename);
        exit(1);
    }
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
    int done = FALSE;
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

    print_title(comp_func);
    
    for(i = 0; !done; i++)
    {
        // grab the next "arrived" jobs out of the generated queue and put
        // them into the process queue
        job *insert, *temp;
        while((insert = heap_extract_max(g, fcfs_comparison)) && insert->arrive <= i) {
            if(comp_func == &rr_comparison) insert->priority = i;
            heap_insert(p, comp_func, insert);
            if(comp_func == &unix_comparison) recalculate_priorities(p, i);                     
            insert = NULL;
        }
        if(insert && insert->arrive > i) // we might have pulled one too many out in the while loop
            heap_insert(g, fcfs_comparison, insert); // so put it back

        if(current == NULL) {
            temp = current;
            current = heap_extract_max(p, comp_func);
            if(verbose && temp != current) printf("clock: %2d\tcontext switch\n", i++);
        }
        else {
            if(comp_func == &srtf_comparison
                || comp_func == &rr_comparison
                || comp_func == &unix_comparison)
            {
                temp = current;
                if(comp_func == &rr_comparison) current->priority = i;
                heap_insert(p, comp_func, current);
                if(comp_func == &unix_comparison) recalculate_priorities(p, i);
                current = heap_extract_max(p, comp_func);
                if(verbose && temp != current) printf("clock: %2d\tcontext switch\n", i++);
            }
        }
        if(current == NULL) {
            if(g->size == 0) done = TRUE;
            else if(verbose) printf("clock: %2d\tidle\n", i);
        }
        else {
            if(current->arrive <= i) {
                if(current->start < 0) current->start = i; // markstart if first time at CPU
                current->service++; // increment the service time
                current->burst--; // decrement the remaining burst time
                if(verbose) {
                    printf("clock: %2d\t", i);
                    print_job(current);
                }

                // if we're done with this job, then put it in the "complete" queue
                if(current->burst <= 0) {
                    current->end = i; // mark the end time for the outgoing job
                    heap_insert(c, id_comparison, current); // put the job in the "complete" queue
                    current = NULL;
                }
                increment_waits(p); // for all "waiting" jobs, increment their "wait" value
            }
            else if(verbose) printf("clock: %2d\tidle\n", i);
        }
    }
    print_results(c, verbose); // print all algorithm analysis results
    return;
}

// prints a title
void print_title(int (*comp_func)(void*, void*))
{
    char *algorithm_name = NULL;
    if(comp_func == &sjf_comparison) {
        algorithm_name = "Shortest Job First";
    } else if(comp_func == &fcfs_comparison) {
        algorithm_name = "First Come First Served";
    } else if(comp_func == &srtf_comparison) {
        algorithm_name = "Shortest Remaining Time First";
    } else if(comp_func == &rr_comparison) {
        algorithm_name = "Round Robin";
    } else if(comp_func == &unix_comparison) {
        algorithm_name = "POSIX Dynamic Priorities";
    } else
        algorithm_name = "Unknown";
    printf("\n*** %s ***\n", algorithm_name);
    return;
}

// print the given job
void print_job(job *j)
{
    printf("id: %3d\tarrive: %4d\tburst: %4d\twaiting: %4d\tstart: %4d\tend: %4d\tpriority: %4d\tservice: %4d\n",
        j->id,
        j->arrive,
        j->burst,
        j->waiting,
        j->start,
        j->end,
        j->priority,
        j->service);
    return;
}

// print all analytical results
void print_results(heap *c, int verbose)
{
    job *current       = NULL;
    int number_of_jobs = c->size,
        sum_waiting    = 0,
        sum_turnaround = 0,
        sum_response   = 0,
        max_end        = 0;
    if(verbose) printf("final job values:\n");
    while((current = heap_extract_max(c, id_comparison))) {
        sum_waiting += current->waiting;
        sum_turnaround += (current->end - current->arrive);
        sum_response += (current->start - current->arrive);
        max_end = max_end > current->end ? max_end : current->end;
        if(verbose) print_job(current);
    }
    printf("final statistics:\n");
    printf(" number of jobs:\t\t%d jobs\n", number_of_jobs);
    printf(" throughput:\t\t\t%3.2f jobs/ms\n", (float)number_of_jobs / (float)max_end);
    printf(" average turnaround time:\t%3.2f ms\n", (float)sum_turnaround / (float)number_of_jobs);
    printf(" average response time:\t\t%3.2f ms\n", (float)sum_response / (float)number_of_jobs);
    printf(" average waiting time:\t\t%3.2f ms\n", (float)sum_waiting / (float)number_of_jobs);
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
    printf(" \t\tValid schedulers are: sjf, fcfs, srtf, rr, unix\n");
    printf(" -v\t\tVerbose mode. Prints an output for each CPU cycle.\n");
    return;
}
