#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"

struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

struct sched_stats ss;

int c1 = 0, c2 = 0, c3 = 0;
static struct proc *initproc;
// int yieldcount = 0;
int nextpid = 1;
extern void forkret(void);
extern void trapret(void);

static void wakeup1(void *chan);

void
pinit(void)
{
  initlock(&ptable.lock, "ptable");
}

// Must be called with interrupts disabled
int
cpuid() {
  return mycpu()-cpus;
}

// Must be called with interrupts disabled to avoid the caller being
// rescheduled between reading lapicid and running through the loop.
struct cpu*
mycpu(void)
{
  int apicid, i;
  
  if(readeflags()&FL_IF)
    panic("mycpu called with interrupts enabled\n");
  
  apicid = lapicid();
  // APIC IDs are not guaranteed to be contiguous. Maybe we should have
  // a reverse map, or reserve a register to store &cpus[i].
  for (i = 0; i < ncpu; ++i) {
    if (cpus[i].apicid == apicid)
      return &cpus[i];
  }
  panic("unknown apicid\n");
}

// Disable interrupts so that we are not rescheduled
// while reading proc from the cpu structure
struct proc*
myproc(void) {
  struct cpu *c;
  struct proc *p;
  pushcli();
  c = mycpu();
  p = c->proc;
  popcli();
  return p;
}

//PAGEBREAK: 32
// Look in the process table for an UNUSED proc.
// If found, change state to EMBRYO and initialize
// state required to run in the kernel.
// Otherwise return 0.
static struct proc*
allocproc(void)
{
  struct proc *p;
  int i = 0;
  char *sp;
  acquire(&ptable.lock);

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->state == UNUSED)
      goto found;
    i++;
  }

  release(&ptable.lock);
  return 0;

found:
  p->state = EMBRYO;
  p->pid = nextpid++;
  //p->first = 1;
  p->queue_no = 1;  //When a process is first created, it should be placed at the highest priority
  p->proc_no = c1++;

  p->dummy = 0;
  p->end_ticks = 0;
  p->cpu_burst = 0;
  p->wait_for_io = 0;
  p->dummy_wait = 0;
  p->dummy_wait2 = 0;

  release(&ptable.lock);

  // Allocate kernel stack.
  if((p->kstack = kalloc()) == 0){
    p->state = UNUSED;
    return 0;
  }
  sp = p->kstack + KSTACKSIZE;

  // Leave room for trap frame.
  sp -= sizeof *p->tf;
  p->tf = (struct trapframe*)sp;

  // Set up new context to start executing at forkret,
  // which returns to trapret.
  sp -= 4;
  *(uint*)sp = (uint)trapret;

  sp -= sizeof *p->context;
  p->context = (struct context*)sp;
  memset(p->context, 0, sizeof *p->context);
  p->context->eip = (uint)forkret;

  return p;
}

//PAGEBREAK: 32
// Set up first user process.
void
userinit(void)
{
  cprintf("in userinit\n");
  struct proc *p;
  extern char _binary_initcode_start[], _binary_initcode_size[];

  ss.nprocesses_scheduled = 0;
  ss.nprocesses_completed = 0;
  ss.max_CT = 0;


  p = allocproc();
  
  initproc = p;
  if((p->pgdir = setupkvm()) == 0)
    panic("userinit: out of memory?");
  inituvm(p->pgdir, _binary_initcode_start, (int)_binary_initcode_size);
  p->sz = PGSIZE;
  memset(p->tf, 0, sizeof(*p->tf));
  p->tf->cs = (SEG_UCODE << 3) | DPL_USER;
  p->tf->ds = (SEG_UDATA << 3) | DPL_USER;
  p->tf->es = p->tf->ds;
  p->tf->ss = p->tf->ds;
  p->tf->eflags = FL_IF;
  p->tf->esp = PGSIZE;
  p->tf->eip = 0;  // beginning of initcode.S

  safestrcpy(p->name, "initcode", sizeof(p->name));
  p->cwd = namei("/");

  // this assignment to p->state lets other cores
  // run this process. the acquire forces the above
  // writes to be visible, and the lock is also needed
  // because the assignment might not be atomic.
  acquire(&ptable.lock);

  p->state = RUNNABLE;
  cprintf("qinit  called\n");
  
  release(&ptable.lock);
}

// Grow current process's memory by n bytes.
// Return 0 on success, -1 on failure.
int
growproc(int n)
{
  uint sz;
  struct proc *curproc = myproc();

  sz = curproc->sz;
  //cprintf("in growproc -- sz is %d\n", sz);
  if(n > 0){
    if((sz = allocuvm(curproc->pgdir, sz, sz + n)) == 0)
      return -1;
  } else if(n < 0){
    if((sz = deallocuvm(curproc->pgdir, sz, sz + n)) == 0)
      return -1;
  }
  curproc->sz = sz;
  switchuvm(curproc);
  return 0;
}

// Create a new process copying p as the parent.
// Sets up stack to return as if from system call.
// Caller must set state of returned proc to RUNNABLE.
int
fork(void)
{
  int i, pid;
  struct proc *np;
  struct proc *curproc = myproc();

  // Allocate process.
  if((np = allocproc()) == 0){
    return -1;
  }

  // Copy process state from proc.
  if((np->pgdir = copyuvm(curproc->pgdir, curproc->sz)) == 0){
    kfree(np->kstack);
    np->kstack = 0;
    np->state = UNUSED;
    return -1;
  }
  np->sz = curproc->sz;
  np->parent = curproc;
  *np->tf = *curproc->tf;

  // Clear %eax so that fork returns 0 in the child.
  np->tf->eax = 0;

  for(i = 0; i < NOFILE; i++)
    if(curproc->ofile[i])
      np->ofile[i] = filedup(curproc->ofile[i]);
  np->cwd = idup(curproc->cwd);

  safestrcpy(np->name, curproc->name, sizeof(curproc->name));

  pid = np->pid;
  
  acquire(&ptable.lock);
  // if(pid == 1 || pid == 2){
  //   np->queue_no = 1;
  // }
  // else{
  //   // np->queue_no = (pid % 2) + 1;
  //   np->queue_no = 1;
  // }
  np->state = RUNNABLE;
  np->start_ticks = ticks;
  np->yield_count = 0;
  np->queue_no = 1;
  if(np->pid == 3)
    ss.min_AT = np->start_ticks;
  cprintf("PID: %d\tNAME: %s\tQUEUE: %d\tstart ticks: %d\n", np->pid, np->name, np->queue_no, np->start_ticks);
  release(&ptable.lock);

  return pid;
}

// Exit the current process.  Does not return.
// An exited process remains in the zombie state
// until its parent calls wait() to find out it exited.
void
exit(void)
{
  
  struct proc *curproc = myproc();
  cprintf("PID: %d\tNAME: %s\tQUEUE: %d\tend ticks: %d\n", curproc->pid, curproc->name, curproc->queue_no, ticks);
  struct proc *p;
  int fd;

  if(curproc == initproc)
    panic("init exiting");

  // Close all open files.
  for(fd = 0; fd < NOFILE; fd++){
    if(curproc->ofile[fd]){
      fileclose(curproc->ofile[fd]);
      curproc->ofile[fd] = 0;
    }
  }

  ss.nprocesses_completed += 1;
  
  curproc->end_ticks = ticks;
  ss.turnaround[curproc->pid - 1] = (curproc->end_ticks-curproc->start_ticks);
  curproc->cpu_burst = ss.turnaround[curproc->pid - 1] - curproc->wait_for_io - curproc->dummy_wait2;
  ss.cpu_burst[curproc->pid - 1] = curproc -> cpu_burst;
  cprintf("PID %d -- %s -- turnaround : %d\n", curproc->pid, curproc->name, ss.turnaround[curproc->pid - 1]);
  cprintf("PID %d -- %s -- wait for io : %d\n", curproc->pid, curproc->name, curproc->wait_for_io);
  cprintf("PID %d -- %s -- cpu burst : %d\n", curproc->pid, curproc->name, curproc->cpu_burst);

  if(ss.max_CT < curproc->end_ticks)
    ss.max_CT = curproc->end_ticks;

  begin_op();
  iput(curproc->cwd);
  end_op();
  curproc->cwd = 0;

  acquire(&ptable.lock);

  // Parent might be sleeping in wait().
  wakeup1(curproc->parent);

  // Pass abandoned children to init.
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->parent == curproc){
      p->parent = initproc;
      if(p->state == ZOMBIE)
        wakeup1(initproc);
    }
  }

  // Jump into the scheduler, never to return.
  curproc->state = ZOMBIE;
  cprintf("yieldcount = %d\n", curproc->yield_count);
  cprintf("lapsed ticks = %d\n", ticks - curproc->start_ticks);
  sched();
  panic("zombie exit");
}

// Wait for a child process to exit and return its pid.
// Return -1 if this process has no children.
int
wait(void)
{
  struct proc *p;
  int havekids, pid;
  struct proc *curproc = myproc();
  
  acquire(&ptable.lock);
  for(;;){
    // Scan through table looking for exited children.
    havekids = 0;
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->parent != curproc)
        continue;
      havekids = 1;
      if(p->state == ZOMBIE){
        // Found one.
        pid = p->pid;
        kfree(p->kstack);
        p->kstack = 0;
        freevm(p->pgdir);
        p->pid = 0;
        p->parent = 0;
        p->name[0] = 0;
        p->killed = 0;
        p->state = UNUSED;
        
        release(&ptable.lock);
        return pid;
      }
    }

    // No point waiting if we don't have any children.
    if(!havekids || curproc->killed){
      release(&ptable.lock);
      return -1;
    }

    // Wait for children to exit.  (See wakeup1 call in proc_exit.)
    // cprintf("%s on cpu, state is %d calling sleep in wait\n", curproc->name, curproc->state);
    sleep(curproc, &ptable.lock);  //DOC: wait-sleep
  }
}

//PAGEBREAK: 42
// Per-CPU process scheduler.
// Each CPU calls scheduler() after setting itself up.
// Scheduler never returns.  It loops, doing:
//  - choose a process to run
//  - swtch to start running that process
//  - eventually that process transfers control
//      via swtch back to the scheduler.
void
scheduler(void)
{
  struct proc *p, *p1;
  struct cpu *c = mycpu();
  c->proc = 0;
  int flag = 0;
  for(;;){
    // Enable interrupts on this processor.
    sti();
    struct proc *highP;

    // Loop over process table looking for process to run.
    acquire(&ptable.lock);
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->state != RUNNABLE)
        continue;

      // Switch to chosen process.  It is the process's job
      // to release ptable.lock and then reacquire it
      // before jumping back to us.
      highP = p;
      // Choose one with highest priority.
      flag = 0;
      for (p1 = ptable.proc; p1 < &ptable.proc[NPROC]; p1++){
        if (p1->state != RUNNABLE){
          if(p1->state == RUNNING){
            if(p1->queue_no < highP ->queue_no){
              flag = 1;
              break;
            }
          }
          continue;
        }
        if (highP->queue_no > p1->queue_no) //larger value, lower priority
          highP = p1;
        if (highP->queue_no == p1->queue_no){
          if(highP->queue_no != 3){
            if(highP->proc_no > p1->proc_no){
              highP = p1;
            }
          }
          else{
            if(p1->yield_count < highP->yield_count){
              highP = p1;
            }
            // cprintf("PID: %d\tNAME: %s\tQUEUE: %d\tyield count: %d\n", highP->pid, highP->name, highP->queue_no, highP->yield_count);
          }
        }
      }
      if(flag == 1){
        continue;
      }
      p = highP;
      // cprintf("PID: %d\tNAME: %s\tQUEUE: %d\trun ticks: %d\n", p->pid, p->name, p->queue_no, ticks);
      c->proc = p;
      if(p->dummy_wait > p->sched_ticks){
        p->dummy_wait2 = p->dummy_wait2 + ticks - p->dummy_wait;
        // cprintf("PID: %d\tNAME: %s\tQUEUE: %d\t ticks %d\n", p->pid, p->name, p->queue_no, ticks);
        // cprintf("PID: %d\tNAME: %s\tQUEUE: %d\t dummy_wait %d\n", p->pid, p->name, p->queue_no, p->dummy_wait);
        // cprintf("PID: %d\tNAME: %s\tQUEUE: %d\t dummy_wait2 %d\n", p->pid, p->name, p->queue_no, p->dummy_wait2);

      }

      switchuvm(p);
      p->state = RUNNING;
      if(p->queue_no == 1)
        modify_TICR(TIME_Q1);
      else if(p->queue_no == 2)
        modify_TICR(TIME_Q2);
      else if(p->queue_no == 3)
        modify_TICR(TIME_Q3);
      
      if(p->first == 1){
        p->first = 0;
        ss.nprocesses_scheduled += 1;
      }
      p->sched_ticks = ticks;
      swtch(&(c->scheduler), p->context);
      switchkvm();

      // Process is done running for now.
      // It should have changed its p->state before coming back.
      c->proc = 0;
    }
    release(&ptable.lock);

  }
}


// Enter scheduler.  Must hold only ptable.lock
// and have changed proc->state. Saves and restores
// intena because intena is a property of this
// kernel thread, not this CPU. It should
// be proc->intena and proc->ncli, but that would
// break in the few places where a lock is held but
// there's no process.
void
sched(void)
{
  int intena;
  // cprintf("in sched: ");
  struct proc *p = myproc();
  // cprintf("%s\n", p->name);
  if(!holding(&ptable.lock))
    panic("sched ptable.lock");
  if(mycpu()->ncli != 1)
    panic("sched locks");
  if(p->state == RUNNING)
    panic("sched running");
  if(readeflags()&FL_IF)
    panic("sched interruptible");
  intena = mycpu()->intena;
  swtch(&p->context, mycpu()->scheduler);
  mycpu()->intena = intena;
}

// Give up the CPU for one scheduling round.
void
yield(void)
{
  acquire(&ptable.lock);  //DOC: yieldlock
  struct proc *p = myproc();
  p->state = RUNNABLE;
  // cprintf("calling sched from yield\n");
  
  // p->run_ticks = ticks;

  if(p->queue_no == 1){
    if(p->pid != 1 && p->pid != 2){
      p->queue_no = 2;
      p->proc_no = c2++;
    }
    // cprintf("PID: %d\tNAME: %s\tQUEUE: %d\trun ticks: %d\n", p->pid, p->name, p->queue_no, ticks);
  }
  else if(p->queue_no == 2){
    p->queue_no = 3;
    p->proc_no = c3++;
    // cprintf("PID: %d\tNAME: %s\tQUEUE: %d\trun ticks: %d\n", p->pid, p->name, p->queue_no, ticks);
  }
  else{
    if(p->yield_count < TICKS_LIMIT)
      p->yield_count++;
    else{
      p->queue_no = 1;
      p->proc_no = c1++;
      p->yield_count = 0;
      // cprintf("moved to q1 from q3\n");
    }
  }
  // yieldcount++;
  p->dummy_wait = ticks;
  sched();

  
  // if(p->first == 1){
  //cprintf("PID: %d\tNAME: %s\tQUEUE: %d\trun ticks: %d\n", p->pid, p->name, p->queue_no, p->run_ticks);
  // p->first = 0;
  // }
  release(&ptable.lock);
}

// A fork child's very first scheduling by scheduler()
// will swtch here.  "Return" to user space.
void
forkret(void)
{
  static int first = 1;
  // Still holding ptable.lock from scheduler.
  release(&ptable.lock);

  if (first) {
    // Some initialization functions must be run in the context
    // of a regular process (e.g., they call sleep), and thus cannot
    // be run from main().
    first = 0;
    iinit(ROOTDEV);
    initlog(ROOTDEV);
  }

  // Return to "caller", actually trapret (see allocproc).
}

// Atomically release lock and sleep on chan.
// Reacquires lock when awakened.
void
sleep(void *chan, struct spinlock *lk)
{
  struct proc *p = myproc();
  
  if(p == 0)
    panic("sleep");

  if(lk == 0)
    panic("sleep without lk");

  // Must acquire ptable.lock in order to
  // change p->state and then call sched.
  // Once we hold ptable.lock, we can be
  // guaranteed that we won't miss any wakeup
  // (wakeup runs with ptable.lock locked),
  // so it's okay to release lk.
  if(lk != &ptable.lock){  //DOC: sleeplock0
    acquire(&ptable.lock);  //DOC: sleeplock1
    release(lk);
  }
  // Go to sleep.
  p->chan = chan;
  p->state = SLEEPING;

  p->dummy = ticks;
  sched();

  // Tidy up.
  p->chan = 0;

  // Reacquire original lock.
  if(lk != &ptable.lock){  //DOC: sleeplock2
    release(&ptable.lock);
    acquire(lk);
  }
}

//PAGEBREAK!
// Wake up all processes sleeping on chan.
// The ptable lock must be held.
static void
wakeup1(void *chan)
{
  struct proc *p;

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    if(p->state == SLEEPING && p->chan == chan){
      p->state = RUNNABLE;
      p->wait_for_io += (ticks - p->dummy);
    }
      
}

// Wake up all processes sleeping on chan.
void
wakeup(void *chan)
{
  acquire(&ptable.lock);
  wakeup1(chan);
  release(&ptable.lock);
}

// Kill the process with the given pid.
// Process won't exit until it returns
// to user space (see trap in trap.c).
int
kill(int pid)
{
  struct proc *p;

  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->pid == pid){
      p->killed = 1;
      // Wake process from sleep if necessary.
      if(p->state == SLEEPING){
        p->state = RUNNABLE;
        // if((p->priority) >= 0 && (p->priority) <= 6)
        //   enqueue(&q1, p);
        // if((p->priority) >= 7 && (p->priority) <= 13)
        //   enqueue(&q2, p);
        // else if((p->priority) >= 14 && (p->priority) <= 20)
        //   enqueue(&q3, p);
      }
      release(&ptable.lock);
      return 0;
    }
  }
  release(&ptable.lock);
  return -1;
}

//PAGEBREAK: 36
// Print a process listing to console.  For debugging.
// Runs when user types ^P on console.
// No lock to avoid wedging a stuck machine further.
void
procdump(void)
{
  static char *states[] = {
  [UNUSED]    "unused",
  [EMBRYO]    "embryo",
  [SLEEPING]  "sleep ",
  [RUNNABLE]  "runble",
  [RUNNING]   "run   ",
  [ZOMBIE]    "zombie"
  };
  int i;
  struct proc *p;
  char *state;
  uint pc[10];

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->state == UNUSED)
      continue;
    if(p->state >= 0 && p->state < NELEM(states) && states[p->state])
      state = states[p->state];
    else
      state = "???";
    cprintf("%d %s %s", p->pid, state, p->name);
    if(p->state == SLEEPING){
      getcallerpcs((uint*)p->context->ebp+2, pc);
      for(i=0; i<10 && pc[i] != 0; i++)
        cprintf(" %p", pc[i]);
    }
    cprintf("\n");
  }
}

int getStats(int n){
  //n = 1: CPU utilisation
  //n = 2: Throughput
  //n = 3: Turnaround time

  //n = 1: CPU burst
  //n = 2: duration
  //n = 3: Turnaround time
  //n = 4: nprocesscomp
  int i = 0, cpu_burst = 0, tt = 0;
  if(n == 1){
    for(i = 0; i < NPROC; i++){
        cpu_burst += ss.cpu_burst[i];
        // cprintf("i: %d\tcpu_burst: %d\n",i, ss.cpu_burst[i]);
    }
    cprintf("cpu_burst is %d\n", cpu_burst);
    return (cpu_burst);
  }
  else if(n == 2){
    return (ss.max_CT - ss.min_AT);
  }
  else if(n == 3){
    for(i = 0; i < NPROC; i++){
      tt += ss.turnaround[i];
      // cprintf("i: %d\ttt: %d\n",i, ss.turnaround[i]);
    }
    cprintf("tt is %d\n", tt);
    return tt;
  }
  else if(n == 4){
    return ss.nprocesses_completed;
  }
  return -1;
}
