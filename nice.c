#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
  int niceness, pid;
  if (argc < 3)
  {
    printf(2, "Usage: nice <pid> <niceness>\n");
    exit();
  }
  pid = atoi(argv[1]);
  niceness = atoi(argv[2]);
  //niceness in range 0 to 20, and priorities in range 20 to 40.
  if ((niceness < 0) || (niceness > 20))
  {
    printf(2, "Give a value of niceness between 0 and 20.\n");
    exit();
  }
  chpriority(pid, niceness);
  exit();
}
