#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
int main(int argc, char *argv[]) {
  int z, x;
  char c;
  int fd = open("README", O_RDONLY);
  if(fd!=-1){
      while(read(fd, &c, 1) != 0){
        x = 1;
    }
  }
  
	for(z = 0; z < 4000; z++){
	    x += 12; 
    }
  printf(1, "util calling exit\n");
  // printf(1, "util calling exit\n");
  exit();
}
