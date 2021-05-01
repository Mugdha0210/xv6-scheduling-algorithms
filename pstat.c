
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[]){

        int cpu_burst = getStats(1);
        int duration = getStats(2);
        int proc_comp = getStats(4);
        int tt = getStats(3);
        printf(1, "cpu utilisation: %d/%d\t, 1/throughput: %d/%d\t, turnaround: %d\n", cpu_burst*100, tt, duration,proc_comp, tt);

    exit();
}
