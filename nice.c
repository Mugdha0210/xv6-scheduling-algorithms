#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
  int queue_num, pid;
  if (argc < 3)
  {
    printf(2, "Usage: nice <pid> <queue_num>\n");
    exit();
  }
  pid = atoi(argv[1]);
  queue_num = atoi(argv[2]);
  //queue_num in range 0 to 20, and priorities in range 20 to 40.
  if ((queue_num < 0) || (queue_num > 20))
  {
    printf(2, "Give a value of queue_num between 0 and 20.\n");
    exit();
  }
  chpriority(pid, queue_num);
  exit();
}
