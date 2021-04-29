#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
  int z, x;
	for(z = 0; z < 4000; z++){
	    x += 12; 
    }
  printf(1, "util calling exit\n");
  // printf(1, "util calling exit\n");
  exit();
}
