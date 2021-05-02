#include "date.h"
// Per-CPU state
struct cpu {
  uchar apicid;                // Local APIC ID
  struct context *scheduler;   // swtch() here to enter scheduler
  struct taskstate ts;         // Used by x86 to find stack for interrupt
  struct segdesc gdt[NSEGS];   // x86 global descriptor table
  volatile uint started;       // Has the CPU started?
  int ncli;                    // Depth of pushcli nesting.
  int intena;                  // Were interrupts enabled before pushcli?
  struct proc *proc;           // The process running on this cpu or null
};

extern struct cpu cpus[NCPU];
extern int ncpu;

//PAGEBREAK: 17
// Saved registers for kernel context switches.
// Don't need to save all the segment registers (%cs, etc),
// because they are constant across kernel contexts.
// Don't need to save %eax, %ecx, %edx, because the
// x86 convention is that the caller has saved them.
// Contexts are stored at the bottom of the stack they
// describe; the stack pointer is the address of the context.
// The layout of the context matches the layout of the stack in swtch.S
// at the "Switch stacks" comment. Switch doesn't save eip explicitly,
// but it is on the stack and allocproc() manipulates it.
struct context {
  uint edi;
  uint esi;
  uint ebx;
  uint ebp;
  uint eip;
};

enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

// Per-process state
struct proc {
  uint sz;                     // Size of process memory (bytes)
  pde_t* pgdir;                // Page table
  char *kstack;                // Bottom of kernel stack for this process
  enum procstate state;        // Process state
  int pid;                     // Process ID
  struct proc *parent;         // Parent process
  struct trapframe *tf;        // Trap frame for current syscall
  struct context *context;     // swtch() here to run process
  void *chan;                  // If non-zero, sleeping on chan
  int killed;                  // If non-zero, have been killed
  struct file *ofile[NOFILE];  // Open files
  struct inode *cwd;           // Current directory
  char name[16];               // Process name (debugging)
  int queue_no;                //1, 2, 3
  int start_ticks;
  int end_ticks;
  int first;
  int yield_count;
  int proc_no;
  int sched_ticks;
  int ready;
  int wait_for_io;
  int cpu_burst;
  int dummy_wait;
  int dummy_wait2;
  int dummy;

};

// Process memory is laid out contiguously, low addresses first:
//   text
//   original data and bss
//   fixed-size stack
//   expandable heap

//3 queues with a maximum of 64 processes each. linked list caused page fault issues, trying queue using an array.
// struct queue{
//   int start;
//   int end;
//   int count;
// };

#define TIME_Q1 10000000
#define TIME_Q2 20000000
#define TIME_Q3 30000000

#define TICKS_LIMIT 30

struct sched_stats{
    int nprocesses_scheduled;           //num of processes scheduled -- updated in scheduler
    //int completion_time;                    //
    int nprocesses_completed;           //num of processes completed -- updated in exit (not sure if needed if no infinite process)
    int cpu_burst[NPROC];               //updated before wait and in exit
    int turnaround[NPROC];              //update in exit
    int min_AT;                         //min arrival time, update in fork() if not the min
    int max_CT;                         //max completion time, update in exit() if not the max
    int min_AT_sec;  
    int max_CT_sec;  

};
