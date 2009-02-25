Please run "make clean && make" to compile.
You may then use a2 as described below or you may simply run the "runall.sh" script.

usage:          ./a2 [OPTIONS]
example:        ./a2 -i data.txt -s sjf,fcfs,srtf -v
                ./a2 -n 5 -s sjf
options:
 -h             Print this message.
 -i <file>      Read comma-separated file with arrive,burst
 -n <number>    Number of jobs to generate if not reading from file.
 -s <sched(s)>  Specify scheduler(s) to use.
                Valid schedulers are: sjf, fcfs, srtf, rr, unix
 -v             Verbose mode. Prints an output for each CPU cycle.