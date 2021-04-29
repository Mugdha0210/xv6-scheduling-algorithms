#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
  int x, z;

  for(z = 1; z < 220; z++){
    x = x + 123 * 123; 
  }
  printf(1, "util2 calling exit\n");
  exit();
}
