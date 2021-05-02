
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[]){
    char *args1[] = { "utilTimed", "10", "10", 0 };
    char *args2[] = { "utilTimed", "10", "10", 0 };
    char *args3[] = { "utilTimed", "10", "10", 0 };

    int i;
    // for(i=0;i<3;i++) // loop will run n times (n=5)
    // {
        if(fork() == 0)
        {
            printf(1, "[son] pid %d from [parent]",getpid());
            exec("utilTimed", args1);
        }
        if(fork() == 0)
        {
            printf(1, "[son] pid %d from [parent]",getpid());
            exec("utilTimed", args2);
        }
        if(fork() == 0)
        {
            printf(1, "[son] pid %d from [parent]",getpid());
            exec("utilTimed", args3);
        }
    // }
    for(i=0;i<3;i++){ // loop will run n times (n=5)
        wait();
    }
    int cpu_burst = getStats(1);
        int duration = getStats(2);
        int proc_comp = getStats(4);
        int tt = getStats(3);
        printf(1, "cpu utilisation: %d/%d\t, 1/throughput: %d/%d\t, turnaround: %d\n", cpu_burst*100, tt, duration,proc_comp, tt);
    halt();
    exit();
}
