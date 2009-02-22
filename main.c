#include "main.h"

int main (int argc, char *argv[])
{
    char *subopts, *value;
    int opt,
        sjf            = 0,
        fcfs           = 0,
        srtf           = 0,
        rr             = 0,
        unix           = 0,
        verbose        = 0,
        number_of_jobs = 0,
        no_scheduler   = 1;
    char *filename     = NULL;

    if(argc == 1) {
        print_usage(argc, argv);
        return 1;
    }
    while ((opt = getopt(argc, argv, "hvn:i:s:")) != -1)
        switch (opt)
    {
        case 'h':
            print_usage(argc, argv);
            break;
        case 'v':
            verbose = 1;
            break;
        case 'n':
            number_of_jobs = strtol(optarg, NULL, 10);
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
                case SRTF:
                    srtf = 1;
                    break;
                case RR:
                    rr = 1;
                    break;
                case UNIX:
                    unix = 1;
                    break;
                default:
                    printf ("unknown scheduler passed in list to -s\n");
                    break;
            }
            no_scheduler = 0;
            break;
        default:
            print_usage(argc, argv);
            abort();
            break;
    }
    if(number_of_jobs > 0 && filename) {
        fprintf(stderr, "Cannot specify both -n and -i parameters.\n");
        print_usage(argc, argv);
        return 1;
    }
    if(number_of_jobs == 0 && filename == NULL) {
        fprintf(stderr, "Must specify either -n or -i parameter.\n");
        print_usage(argc, argv);
        return 1;
    }
    if(no_scheduler) {
        fprintf(stderr, "Must specify a scheduler with -s parameter.\n");
        print_usage(argc, argv);
        return 1;
    }

    if(sjf)
        process_jobs(sjf_comparison, filename, number_of_jobs, verbose);
    if(fcfs)
        process_jobs(fcfs_comparison, filename, number_of_jobs, verbose);
    if(srtf)
        process_jobs(srtf_comparison, filename, number_of_jobs, verbose);
    if(rr)
        process_jobs(rr_comparison, filename, number_of_jobs, verbose);
    if(unix)
        process_jobs(unix_comparison, filename, number_of_jobs, verbose);
    return 0;
}
