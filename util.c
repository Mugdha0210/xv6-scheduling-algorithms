#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
  int pid;
  int k;
  int x, z;

  x = 0;
  pid = 0;

  for ( k = 0; k < 5; k++ ) {
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
	for(z = 0; z < 4000000000; z++)
	    x = x + 12345 * 12345; 
    }
  }
  exit();
}
