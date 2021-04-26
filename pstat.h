#include "param.h"
struct sched_stats{
    int nprocesses_scheduled;           //num of processes scheduled -- updated in scheduler
    int time_lapsed;
    int nprocesses_completed;           //num of processes completed -- updated in exit
    int ticks[NPROC];                   //ticks taken by each process -- updated every time sched() is called.
    int priority_or_queue[NPROC];
} sched_stats;