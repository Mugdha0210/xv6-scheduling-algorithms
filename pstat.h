#include "param.h"
struct sched_stats{
    int nprocesses_scheduled;           //num of processes scheduled -- updated in scheduler
    //int completion_time;                    //
    int nprocesses_completed;           //num of processes completed -- updated in exit (not sure if needed if no infinite process)
    int cpu_burst[NPROC];               //updated before wait and in exit
    int total_time[NPROC];              //update in exit
    int min_AT;                         //min arrival time, update in fork() if not the min
    int max_CT;                         //max completion time, update in exit() if not the max
    int priority_or_queue[NPROC];
} sched_stats;