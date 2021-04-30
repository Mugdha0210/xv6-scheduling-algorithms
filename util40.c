#include "types.h"
#include "stat.h"
#include "user.h"
#include "date.h"

int main(int argc, char *argv[]) {
  int x, start, now;
  //seconds = (end_ticks - start_ticks) / 100
  //run for 10sec
  int runtime = 1000;

  start = uptime();

  while(1){
    now = uptime();

    if((now - start) >= runtime)
      break;

    x = 0;
    x = x + 123 * 123; 
  }

  printf(1, "util40 calling exit\n");
  exit();
}
