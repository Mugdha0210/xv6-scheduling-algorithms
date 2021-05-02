#include "types.h"
#include "stat.h"
#include "user.h"
#include "date.h"
#include "fcntl.h"

int main(int argc, char *argv[]) {
  int x;
  // int i;
  // int fd;
  uint start, cputime, now, iotime;
  // char *buffer = (char*)malloc(sizeof(char)*512);

  if(argc != 3){
    printf(1, "Usage: utilTimed <CPU burst time> <IO burst time>\n");
    exit();
  }

  cputime = atoi(argv[1])*100;
  iotime = atoi(argv[2])*100;
  // printf(1, "%d\n", cputime);
  // printf(1, "%d\n", iotime);

  start = uptime();
  while(1){
    now = uptime();

    if((now - start) >= cputime/2){
      // printf(1, "diff 1: %d\n", now-start);
      break;
    }

    x = 0;
    x = 123 * 123; 
    // for(i=0; i<4000000; i++){
    //     // for(j = 0; j < 400; j++){
    //         x = 0;
    //         x += 123456 * 12356 + 31182;
    //     // }
    // }
  }

  sleep(iotime);

  start = uptime();
  while(1){
    now = uptime();

    if((now - start) >= cputime/2){
      // printf(1, "diff 3: %d\n", now-start);
      break;
    }

            x = 0;
            x += 123 * 123;

  }

  exit();
}
