#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
  // int pid;
  //int ppid = 1; 	//parent is init if failure
  int i,x;
  // pid = fork();
  // if (pid < 0)
  // {
  //   printf(1, "%d failed in fork!\n", getpid());
  // }
  // else if (pid > 0)
  // {
  //   // parent
  //   pid = getpid();
  //   wait();
  // }
  // else
  // {
    // printf(1, "Child %d created\n", getpid());
    for(i=0; i < 2200000000; i++){
      // printf(1, "Child: %d, ", i);
      // printf(1, "i is %d\n", i);
      if(i == (2200000000 - 1) ){
        printf(1, "i is :%d\n", i);
      }
       x = x + 3.14*89.64;
       x = 12345 * 12345;
    }
    //printf(1, "\n");
  // }
  // printf(1, "i: %d, ", i);

  // printf(1, "Parent: ");
  // time(ppid);

  // printf(1, "Child: ");
  // time(pid);
  printf(1, "util calling exit\n");
  exit();
}
