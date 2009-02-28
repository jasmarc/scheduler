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

# Simulation and Algorithm Analysis of CPU Scheduling Models
#### Jason Marcell
#### Computer Science Department
#### Georgia State University
#### jmarcell1@student.gsu.edu

## Abstract
This paper examines the factors that influence the performance of various CPU scheduling algorithms in order to evaluate each as determined by a set of common performance metrics.

## 1 Introduction
CPU scheduling is the process by which a computer's operating system determines how, in what order, and for how long individual processes in a queue of processes are allowed to access the CPU. Input factors such as the chosen scheduling algorithm, the length of processes, and frequency of processes will have an influence on performance factors such as CPU utilization, average job waiting time, average job response time, and average job turn-around time. Depending on the application, the importance of some factors may weigh more heavily than others. For instance, a system that is designed for heavier human-computer-interaction may require lower average job response time in order to make the system appear more responsive. 

In this paper we will look at the following scheduling algorithms:
* First Come First Served
* Shortest Job First
* Shortest Remaining Time First
* Round Robin
* POSIX Dynamic Priorities Scheduling

We will observe the following output metrics:
* Job Throughput
* CPU Utilization
* Average Turnaround Time
* Average Response Time
* Average Waiting Time

We will also vary our random sample of data by altering certain factors which will be discussed later.

## 1.1 Scheduling Algorithms
## 1.1.1 First Come First Served (FCFS)
Jobs are processed in the order that they arrive [2]. For example, process P0 is the first to arrive at time t0 and no other processes are enqueued or are being serviced. P0 has a burst duration of 3. P0 is immediately serviced and continues to be serviced through to time t3. P1 showed up at time t1, but because the CPU was busy at the time, P1 is not serviced until time t3. Likewise, with any other processes that arrive while the CPU is busy, they are enqueued for later scheduling. Arrival does not guarantee immediate service, however earlier arrival does ensure higher priority for later scheduling. 

## 1.1.2 Shortest Job First (SJF)
Jobs are prioritized by job burst duration [2]. For example, P0 and P1 arrive at the same time t0 and no other processes are enqueued or are being serviced. The CPU chooses the shorter of the two jobs, assume for example that the shorter job was P0, and leaves the other, P1, in the queue. While the CPU is servicing P0, other jobs may show up and join P1 in the queue. The fact that P1 arrived before those jobs has no bearing on their priority in this queue. If one of the jobs that has come along happens to be shorter than P1, it will jump to the front of the line and will be scheduled to receive service before P1. This scheduling algorithm allows for the possibility of starvation. It is possible that P1 may never get a turn at the CPU if a continuous stream of shorter jobs is in fresh supply to cut in front of P1.

## 1.1.3 Shortest Remaining Time First (SRTF)
Jobs are prioritized by job burst duration but allowing for preemption by shorter, newly-arriving processes [2]. For example, P0 and P1 arrive at the same time t0 and no other processes are enqueued or are being serviced. The CPU chooses the shorter of the two jobs, assume for example the shorter job was P0, and leaves the other, P1, in the queue. While the CPU is servicing P0, other jobs may show up and if they happen to be shorter in burst duration than the amount of remaining time left to finish servicing P0 as well as the burst duration of P1, then not only do they get to cut in front of P1, but also P0 is preempted. P0 is then shoved back into the queue unfinished and the shorter job is then serviced immediately. P0 will rise to the front of the queue again when its remaining burst duration is less than the remaining burst duration of any other process.

## 1.1.4 Round Robin (RR)
Jobs are time sliced and interleaved over time, allocating a certain quantum of time to each enqueued process [2]. For example, P0 and P1 arrive at the same time t0 and no other processes are enqueued or are being serviced. The CPU alternates between servicing P0 and P1 over equal quanta of times until both processes are completed. If other processes arrive while P0 and P1 are being serviced, they join the back of the queue and are rotated into the cycle. If the remaining burst duration of a particular process is less than the time chosen for the service quantum, the scheduler simply moves to the next item in the queue.

## 1.1.5 POSIX Dynamic Priorities Scheduling (POSIX)
POSIX Dynamic Priorities Scheduling algorithm is another preemptive scheduling algorithm that was designed to counter the starvation scenario presented in the SJF algorithm as well as assign different classes of priorities to different types of processes. Priorities are recalculated for all enqueued processes on a set schedule such that priority = (recent CPU usage/2) + base, where recent CPU usage is how much time the given process has spent at the CPU and base is a tunable, implementation-specific factor [2]. This algorithm ensures that processes that have seen the CPU but were preempted will not be starved out by incoming processes, and thus will eventually be allowed to finish.


## 1.2 Performance metrics
## 1.2.1 Job Throughput
This is simply the number of jobs per unit of time [2]. Throughout this report, time will always be measured in milliseconds.

## 1.2.2 CPU Utilization
This number, given as a percent, is an indication of the amount of time that the CPU has spent doing useful work as opposed to time spent context switching [2]. A context switch occurs when a process is preempted. The system must stop what it is doing, package up the current process's work environment, and move all of this safely to a storage area to be worked on later and to make room for the incoming process. This switching of tasks in and out is pure overhead and should be minimized in an efficient algorithm.

## 1.2.3 Average Turnaround Time
This is the amount of time it takes to service a process from arrival to completion [2].

## 1.2.4 Average Response Time
This is the amount of time it takes from the moment a process arrives to the moment we see the first response from the process, that is the first instant in which the process is being serviced [2].

## 1.2.5 Average Waiting Time
This is the total amount of time a scheduled process spends waiting, including time spent waiting for its first turn at the CPU as well as time spent waiting after being preempted by a higher priority process [2].

## 1.3 Input Data
The burst durations of real life CPU processes have been found to follow an exponential probability distribution such that shorter burst are more likely to occur, whereas longer burst durations are less likely to occur. Likewise interprocess arrival times have been similarly been found to follow an exponential probability distribution function such that shorter time intervals between processes are more likely to occur than larger time intervals between processes [2].

## 2 Experiment
## 2.1 Data Structures and Algorithms
## 2.2.1 Heap
The heart of the scheduler is a heap. A heap is a very simple data structure that is physically implemented as an array but with some special rules that allow for us to extract, very easily and quickly, the next highest priority item. This is known as a priority queue. The heap is a commonly favored data structure to use for implementing priority queues, and schedulers in particular, because the algorithmic performance of each of its operations is favorable. Insertion and extraction each have a running time of O(lg n) [1].

A priority queue, in general, regardless of implementation, is also particularly use for this exercise due to its flexibility. By simply altering the method by which comparisons are made on a pair of nodes, we can completely change the type of scheduler we are implementing. For example, the queue in an FCFS scheduler would sort on arrival time, that of an SJF would sort on burst length. The SRTF is exactly like the SJF but with preemption. The round robin schedulers requires a FIFO queue which, as it turns out, is just a priority queue in which the priority is based on the insert time. The POSIX scheduler is also implemented with a priority queue, however the priority is based upon an externally assigned priority, assigned according to a formula. The formula for priority in the POSIX scheduler is (recent CPU usage/2) + base where recent CPU usage is how much time the given process has spent at the CPU and base is a tunable, implementation-specific factor. For this implementation I have chosen 60 as my base.

## 2.2.2 Comparison Functions
As described in the previous section, the priority queue implemented as a heap is particularly convenient for this application as it allows me to reuse the same code while only altering the way in which nodes are compared within the heap. In order to do this, I made all of the heap routines require the caller to provide a comparison function. The heap routines then uses this user-provided comparison function to determine the priority of nodes. This is similar to the way the qsort function from the C Standard Library (stdlib.h) works. The comparison function takes two arguments, a and b. If a < b, the function returns a negative value. If a > b, the function returns a positive value. If a equals b, the function returns zero. This is most easily accomplished by subtracting b from a.

## 2.2.3 Job Struct
My job data structure is a simple C struct with the following integer values
* id - The id of the job
* arrive - The arrive time
* burst - The remaining burst time
* waiting - The cumulative wait time
* start - The start time. I set this to -1 upon initialization as indication of whether I have touched this. This is so that I only set this once.
* end - The end time
* priority - The priority of the job. This is used by RR and POSIX. The value here is set as in the formula previously described for the POSIX scheduler. And as mentioned before, for RR, this is actually set to the insert time in order to create a FIFO queue.
* service – The total service time

## 2.2.4 Preemption
The SRTF, RR, and POSIX schedulers all exhibit preemption. This was implemented by reinserting the job currently being serviced back into the heap after every clock tick in order to reevaluate the entire heap to see if another job with higher priority should take the place of the current job.

## 2.2.5 Generating Data
In order to simulate real-life data, we need random numbers that exhibit exponential distribution. For this, I used GNU Octave, an open source mathematics software tool similar to Wolfram's MATLAB. Octave has an exprnd function which accepts a lambda value as well as two scalar values for row and column sizes and generates a set of random data according to an exponential probability distribution function with the given lambda value. I created a matrix with 500 rows and 2 columns. The first column represents interprocess arrival time. The second column represents burst duration. I ensured that burst duration was a minimum of length 2.  After generating this data in Octave, this data is fed into the scheduler simulator via a command line option. The scheduler accepts the name of a file which is expected to contain data in a CSV format, fields separated by commas, records separated by newlines.

## 3 Results
## 3.1 Random Data Samples
Figures 3.1. through 3.3. show histogram plots of the three sets of random data that were run through the schedulers.


Equation 1.


Equation 2.

The random data were generated to exhibit an exponential distribution. The data sets varied by their beta value as shown in Equation 1. for the exponential probability distribution function. The data were generated using GNU Octave, using the exprnd function which expects the probability distribution function to be in the form as shown in Equation 2. Thus, the lambda value is just the reciprocal of the desired beta [3]. Each set had 500 jobs.

![Figure 3.1.](../raw/c4753f86377715ee4b72da2283a582d406990ec6/data/i1.png "Figure 3.1.")

Figure 3.2.




Figure 3.3.

## 3.1 Random Data Samples
Figures 3.5 through 3.8 show job throughput, CPU utilization, average turnaround time, average response time, and average waiting time by scheduling algorithm and beta value.











## 4 Conclusion
Algorithm choice had no effect on job throughput, however throughput was shown to increase with higher beta values. This suggests that machines with more frequent short burst processes have more difficulty getting jobs completed whereas systems that receive less frequent arrivals and a greater mix of long burst duration processes are able to get more processes completed.

The SJF algorithm was shown to keep the CPU busiest with a max of 94% average utilization. In general, utilization dropped with beta value increase.

The SJF and SRTF algorithms had the minimal average turnaround times of approximately 200ms each. In general turnaround time increased with beta value.

The POSIX scheduler has the shortest average response time averaging around a millisecond or even less in some cases. Next was RR which averaged approximately 70ms or so. SJF and SRTF each averaged closed to 200ms. The FCFS algorithm however had the worst response time, averaging around 600ms.

The SJF and SRTF algorithms had the best wait times at around 200ms. Next were the FCFS and POSIX schedulers with approximately 700 and 1,000ms, respectively. The RR algorithm was the worst for wait time with approximately 1.4 seconds.

In conclusion, if responsiveness is most important, such as in a system that a human will be interacting with primarily, POSIX scheduling would provide maximum responsiveness and the FCFS would definitely not be recommended. The RR algorithm had a terrible average wait time compared to the other algorithms. The SJF and SRTF performed very well in most cases, if not average, compared to the other algorithms. As always, choice of scheduling algorithm is specific to the needs of the application, but as demonstrated differences in performance will result, and, thus, care should be taken that the appropriate algorithm is chosen for the task at hand.

## 5. References
[1] T. H. Cormen, C. E. Leiserson, R. L. Rivest, and C. Stein. Introduction to Algorithms. MIT Press, Second Edition, 2001.

[2] A. Silberschatz and P. B. Galvin. Operating Systems Concepts. Addison Wesley, Reading, MA, Seventh Edition, 2004.

[3] Weisstein, Eric W. "Exponential Distribution." From MathWorld--A Wolfram Web Resource. http://mathworld.wolfram.com/ExponentialDistribution.html
