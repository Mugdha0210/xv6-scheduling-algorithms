#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
  int x, z;

  for(z = 1; z < 30000; z++){
    x = 0;
    x = x + 12345 * 12345; 
    if(z == (30000 - 1))
        printf(1, "z is %d\n", z);
  }
    printf(1, "util calling exit\n");
  exit();
}