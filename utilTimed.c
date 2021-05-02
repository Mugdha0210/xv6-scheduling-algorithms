// //sec = (end - start)/100

// #include "types.h"
// #include "stat.h"
// #include "user.h"
// #include "date.h"
// #include "fcntl.h"

// int main(int argc, char *argv[]) {
//   int x;
//   uint start, cputime, iotime, now;
//   uint priority;

//   if(argc != 4){
//     printf(1, "Usage: utilTimed <CPU burst time> <IO burst time> <Priority>\n");
//     exit();
//   }

//   priority = atoi(argv[3]);

//   // from scheduler, TICR = 10000000 * (20 - priority)
//   // multiplier to find time in ticks = ((10000000 / TICR) * 100)
//   //                                  = 100 / (20 - priority)  

//   cputime = atoi(argv[1]) * (100 / (20 - priority));
//   iotime = atoi(argv[2]) * (100 / (20 - priority));

//   //cputime = atoi(argv[1]) * 100;
//   //iotime = atoi(argv[2]) * 100;

//   //printf(1, "%d\n", cputime);
//   //printf(1, "%d\n", iotime);

//   start = uptime();
//   while(1){
//     now = uptime();

//     if((now - start) >= cputime/2){
//       //printf(1, "diff 1: %d\n", now-start);
//       break;
//     }

//     x = 0;
//     x = x + 123 * 123; 
//   }

//   sleep(iotime);
//   //start = uptime();
//   //while(1){
//     //now = uptime();

//     //if((now - start) >= iotime){
//      // printf(1, "diff 2: %d\n", now-start);
//      // break;
//     //}

//     //fd = open("README", O_RDONLY);
//     //if(fd == -1){
//       //printf(1, "File open failed.\n");
//       //exit();
//     //}
//     //read(fd, buffer, 50);
//     //printf(1, "%s\n", buffer);
//     //close(fd);
//   //}

//   start = uptime();
//   while(1){
//     now = uptime();

//     if((now - start) >= cputime/2){
//       //printf(1, "diff 3: %d\n", now-start);
//       break;
//     }

//     x = 0;
//     x = x + 123 * 123; 
//   }

//   exit();
// }
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

  if(argc != 4){
    printf(1, "Usage: utilTimed <CPU burst time> <IO burst time> <priority>\n");
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
