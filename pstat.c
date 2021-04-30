
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

void util(void);
void util2(void);


int main(int argc, char *argv[]){
  //   int pid = fork();
  //  if ( pid < 0 ) {
  //     printf(1, "%d failed in fork!\n", getpid());
  //   }
  //   else if (pid > 0) {
  //     // parent
  //     //printf(1, "Parent %d creating child %d\n",getpid(), pid);
  //       int qid = fork();
  //       if(qid > 0){
  //           wait();
  //       }
  //       else if(qid == 0){
  //           util2();
  //       }
  //       wait();
        int cpu_burst = getStats(1);
        int duration = getStats(2);
        int proc_comp = getStats(4);
        int tt = getStats(3);
        printf(1, "cpu utilisation: %d/%d\t, throughput: %d/%d\t, turnaround: %d\n", cpu_burst, tt, proc_comp, duration, tt);
    // }
    // else{
    //     util();
    // }
    // util();
    // util2();
    // int i = 0, cpu_burst = 0, tt = 0;

    exit();
}

void util(void){
//     int pid;
//   int z;

//   pid = 0;

//     pid = fork ();
//     if ( pid < 0 ) {
//       printf(1, "%d failed in fork!\n", getpid());
//     }
//     else if (pid > 0) {
//       // parent
//       //printf(1, "Parent %d creating child %d\n",getpid(), pid);
//       wait();
//     }
//     else {
// 	//printf(1,"Child %d created\n",getpid());
//     int x;
int z, x;
	for(z = 0; z < 4000; z++){
	    x += 12; 
    }
  printf(1, "util calling exit\n");
}


void util2(void){
    int x, z;

  for(z = 1; z < 220; z++){
    x = x + 123 * 123; 
  }
  printf(1, "util2 calling exit\n");
}