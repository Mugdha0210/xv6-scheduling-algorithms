#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

extern int yieldcount;

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_ps(void)
{
  return ps();
}

int sys_chpriority(void)
{
  int pid, pr;
  if (argint(0, &pid) < 0)
    return -1;
  if (argint(1, &pr) < 0)
    return -1;

  return chpriority(pid, pr);
}


int sys_getStats(void)
{
 // struct sched_stats *stats = (struct sched_stats *)malloc(sizeof(stats));
  // if(argptr(0, &stats, sizeof(stats)) < 0)
  //   return -1;
  int n;
  //n = 1: CPU utilisation
  //n = 2: Throughput
  //n = 3: Turnaround time
  if(argint(0, &n) < 0)
    return -1;
  return getStats(n);
}

//Ref: https://medium.com/@silvamatteus/adding-new-system-calls-to-xv6-217b7daefbe1
int
sys_halt(void)
{
  outb(0xf4, 0x00);
  return 0;
}
