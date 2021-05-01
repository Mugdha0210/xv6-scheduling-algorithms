#include "types.h"
#include "stat.h"
#include "user.h"
#include "date.h"

int main(int argc, char *argv[]) {
  int x, start, now;

  if(argc != 2){
    printf(1, "Usage: utilTimed <CPU burst time>\n");
    exit();
  }

  //run for argv[1] sec
  //seconds = (end_ticks - start_ticks) / 100
  int runtime = atoi(argv[1]) * 100;

  start = uptime();

  while(1){
    now = uptime();

    if((now - start) >= runtime)
      break;

    x = 0;
    x = x + 123 * 123; 
  }

  exit();
}
