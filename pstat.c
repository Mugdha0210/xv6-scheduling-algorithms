#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[]){
    int i;
 
    char *args1[] = { "utilTimed", "10", "10", "5", 0 };
    char *args2[] = { "utilTimed", "10", "10", "10", 0 };
    char *args3[] = { "utilTimed", "10", "10", "15", 0 };

    // for(i=0;i<3;i++) // loop will run n times (n=5)
    // {
        if(fork() == 0)
        {
            //printf(1, "[son] pid %d from [parent]",getpid());
            chpriority(getpid(), atoi(args1[3]));
            exec("utilTimed", args1);
        }
        if(fork() == 0)
        {
            //printf(1, "[son] pid %d from [parent]",getpid());
            chpriority(getpid(), atoi(args2[3]));
            exec("utilTimed", args2);
        }
        if(fork() == 0)
        {
            //printf(1, "[son] pid %d from [parent]",getpid());
            chpriority(getpid(), atoi(args3[3]));
            exec("utilTimed", args3);
        }
    // }
    for(i=0;i<3;i++){ // loop will run n times (n=5)
        wait();
    }
    int cpu_burst = getStats(1);
        int duration = getStats(2);
        int proc_comp = getStats(4);
        //int tt = getStats(3);
        printf(1, "cpu utilisation: %d\%\t, 1/throughput: %d\t, turnaround: %d\n", cpu_burst*100/duration, duration/proc_comp, duration);
    halt();
    exit();
}

