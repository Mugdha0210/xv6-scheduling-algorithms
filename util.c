#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
  int pid;
  int z;

  pid = 0;

    pid = fork ();
    if ( pid < 0 ) {
      printf(1, "%d failed in fork!\n", getpid());
    }
    else if (pid > 0) {
      // parent
      //printf(1, "Parent %d creating child %d\n",getpid(), pid);
      wait();
    }
    else {
	//printf(1,"Child %d created\n",getpid());
  int x;
	for(z = 0; z < 40; z++)
	    x += 12; 
    }
  printf(1, "util calling exit\n");
  exit();
}
