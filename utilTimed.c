#include "types.h"
#include "stat.h"
#include "user.h"
#include "date.h"
#include "fcntl.h"

int main(int argc, char *argv[]) {
  int x;
  //int fd;
  uint start, cputime, iotime, now;
  //char *buffer = (char*)malloc(sizeof(char)*512);

  if(argc != 3){
    printf(1, "Usage: utilTimed <CPU burst time> <IO burst time>\n");
    exit();
  }

  cputime = atoi(argv[1]);
  iotime = atoi(argv[2]);
  printf(1, "%d\n", cputime);
  printf(1, "%d\n", iotime);

  start = uptime();
  while(1){
    now = uptime();

    if((now - start) >= cputime/2){
      printf(1, "diff 1: %d\n", now-start);
      break;
    }

    x = 0;
    x = x + 123 * 123; 
  }

  start = uptime();
  while(1){
    now = uptime();

    if((now - start) >= iotime){
      printf(1, "diff 2: %d\n", now-start);
      break;
    }

    sleep(0);
    //fd = open("README", O_RDONLY);
    //if(fd == -1){
      //printf(1, "File open failed.\n");
      //exit();
    //}
    //read(fd, buffer, 50);
    //printf(1, "%s\n", buffer);
    //close(fd);
  }

  start = uptime();
  while(1){
    now = uptime();

    if((now - start) >= cputime/2){
      printf(1, "diff 3: %d\n", now-start);
      break;
    }

    x = 0;
    x = x + 123 * 123; 
  }

  exit();
}
