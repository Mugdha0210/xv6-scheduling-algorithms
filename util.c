#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
  int pid;
  int ppid = 1; 	//parent is init if failure
  int i;

  pid = fork();
  if (pid < 0)
  {
    printf(1, "%d failed in fork!\n", getpid());
  }
  else if (pid > 0)
  {
    // parent
    ppid = getpid();
    wait();
  }
  else
  {
    printf(1, "Child %d created\n", getpid());
    for(i=0; i < 1000000; i++){
      printf(1, "Child: %d, ", i);
    }
    //printf(1, "\n");
  }
  printf(1, "i: %d, ", i);

  printf(1, "Parent: ");
  time(ppid);

  printf(1, "Child: ");
  time(pid);

  exit();
}
