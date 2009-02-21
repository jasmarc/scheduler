#include "main.h"

int fcfs_comparison(void *a, void *b)
{
    return (((job*)b)->arrive - ((job*)a)->arrive);
}

int sjf_comparison(void *a, void *b)
{
    return (((job*)b)->burst - ((job*)a)->burst);
}

int main (int argc, char *argv[])
{
    char *subopts, *value;
    int opt,
        sjf = 0,
        fcfs = 0,
        verbose = 0;
    char *filename = NULL;
    
    if(argc == 1) {
        print_usage(argc, argv);
        return 1;
    }
    while ((opt = getopt(argc, argv, "hvi:s:")) != -1)
        switch (opt)
    {
        case 'h':
            print_usage(argc, argv);
            break;
        case 'v':
            verbose = 1;
            break;
        case 'i': 
            filename = optarg;
            break;
        case 's':
            subopts = optarg;
            while (*subopts != '\0')
                switch (getsubopt(&subopts, scheduler_opts, &value))
            {
                case SJF:
                    sjf = 1;
                    break;
                case FCFS:
                    fcfs = 1;
                    break;
                default:
                    printf ("Unknown suboption `%s'\n", value);
                    break;
            }
            break;
        default:
            print_usage(argc, argv);
            abort();
            break;
    }

    if(sjf) {
        heap *h = malloc(sizeof(heap));
        heap *c = malloc(sizeof(heap));
        heap_init(h);
        heap_init(c);
        if(filename)
            read_jobs_from_file(h, sjf_comparison, filename);
        else
            generate_jobs(h, sjf_comparison, 5);
        printf("*** SJF ***\nnumber of jobs: %d\n", h->size);
        print_jobs(h);
        process_jobs(h, sjf_comparison, c, verbose);
        print_results(c);
    }
    
    if(fcfs) {
        heap *h = malloc(sizeof(heap));
        heap *c = malloc(sizeof(heap));
        heap_init(h);
        heap_init(c);
        if(filename)
            read_jobs_from_file(h, fcfs_comparison, filename);
        else
            generate_jobs(h, fcfs_comparison, 5);
        printf("*** FCFS ***\nnumber of jobs: %d\n", h->size);
        print_jobs(h);
        process_jobs(h, fcfs_comparison, c, verbose);
        print_results(c);
    }
    return 0;
}