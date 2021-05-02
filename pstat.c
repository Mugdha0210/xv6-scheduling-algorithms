
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[]){
    char *args1[] = { "utilTimed", "10", "12", 0 };
    char *args2[] = { "utilTimed", "20", "15", 0 };
    char *args3[] = { "utilTimed", "10", "10", 0 };
    char *args4[] = { "utilTimed", "10", "5", 0 };
    char *args5[] = { "utilTimed", "20", "5", 0 };
    // int p,q;
    int i;
    // for(i=0;i<3;i++) // loop will run n times (n=5)
    // {
        if(fork() == 0)
        {
            // printf(1, "[son] pid %d from [parent]",getpid());
            exec("utilTimed", args1);
        }
        if(fork() == 0)
        {
            // printf(1, "[son] pid %d from [parent]",getpid());
            exec("utilTimed", args2);
        }
        if(fork() == 0)
        {
            // printf(1, "[son] pid %d from [parent]",getpid());
            exec("utilTimed", args3);
        }
        if(fork() == 0)
        {   
            // p = getpid();
            // printf(1, "[son] pid %d from [parent]",getpid());
            // chpriority(p, 1);
            exec("utilTimed", args4);
        }
        if(fork() == 0)
        {   
            // q = getpid();
            // printf(1, "[son] pid %d from [parent]",getpid());
            // chpriority(q, 2);
            exec("utilTimed", args5);
        }
    // }
    for(i=0;i<5;i++){ // loop will run n times (n=5)
        wait();
    }
    int cpu_burst = getStats(1);
        int duration = getStats(2);
        int proc_comp = getStats(4);
        int tt = getStats(3);
        // int dur_sec = getStats(5);
        printf(1, "CPU UTILISATION: %d\% \nTHROUGHPUT (number of processes per second): %d/%d\nTURNAROUND: %d\n", (cpu_burst*100 / tt), proc_comp, duration, duration);
    halt();
    exit();
}
