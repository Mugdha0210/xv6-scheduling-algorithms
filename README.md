# XV6 Schduling Algorithms

---

### 📝 Table of Contents

- [Description](#-description)
- [Languages](#languages)
- [How To Use](#how-to-use)
- [Authors](#authors)
- [Acknowledgements](#-acknowledgements)

---

## 🔠 Description

- A computer process time consists of both I/O time and CPU time. To maximise the efficiency of the system, different scheduling algorithms can be implemented to schedule processes on the CPU.

- This project implements 4 CPU-scheduling algoritms in xv6 operating system -

1. First Come First Serve: Schedules according to arrival times of processes. Timer interrupt was disabled.
2. Priority Scheduling (Preemptive): Highest priority process is given greater time quantum. If priorities of two processes match, then schedule according to arrival time.
3. Multilevel Queue (Preemptive): Processes are placed in different queues. Only after completion of processes from top level queue, lower level queued processes are scheduled. Lower queue processes can suffer from starvation.
4. Multilevel Feedback Queue (Preemptive): If a process uses too much CPU time, it is moved to a lower-priority queue.

The time quantum was calculated using timer-interrupt and corresponding ticks. Calculation of ticks-count was adjusted according to timer interrupt duration to achieve uniformity.

Parameters such as turnaround time, throughput, waiting time were calculated.

---

## 💻 Languages

- C
- Assembly language

---

## 🔧 How To Use

- Clone the repo

```bash
cd xv6-scheduling-algorithms
git checkout master
sh script.sh
```

- The automated script will automatically switch branches, start qemu and run the commands to demonstrate all scheduling algorithms.

---

## 🖋️ Authors

- [@Mugdha0210](https://github.com/Mugdha0210)
- [@Urvi2904](https://github.com/Urvi2904)

---

## Acknowledgements

- xv6 repository- https://github.com/mit-pdos/xv6-public

---

NOTE: we have stopped maintaining the x86 version of xv6, and switched
our efforts to the RISC-V version
(https://github.com/mit-pdos/xv6-riscv.git)

xv6 is a re-implementation of Dennis Ritchie's and Ken Thompson's Unix
Version 6 (v6). xv6 loosely follows the structure and style of v6,
but is implemented for a modern x86-based multiprocessor using ANSI C.

ACKNOWLEDGMENTS

xv6 is inspired by John Lions's Commentary on UNIX 6th Edition (Peer
to Peer Communications; ISBN: 1-57398-013-7; 1st edition (June 14,
2000)). See also https://pdos.csail.mit.edu/6.828/, which
provides pointers to on-line resources for v6.

xv6 borrows code from the following sources:
JOS (asm.h, elf.h, mmu.h, bootasm.S, ide.c, console.c, and others)
Plan 9 (entryother.S, mp.h, mp.c, lapic.c)
FreeBSD (ioapic.c)
NetBSD (console.c)

The following people have made contributions: Russ Cox (context switching,
locking), Cliff Frey (MP), Xiao Yu (MP), Nickolai Zeldovich, and Austin
Clements.

We are also grateful for the bug reports and patches contributed by Silas
Boyd-Wickizer, Anton Burtsev, Cody Cutler, Mike CAT, Tej Chajed, eyalz800,
Nelson Elhage, Saar Ettinger, Alice Ferrazzi, Nathaniel Filardo, Peter
Froehlich, Yakir Goaron,Shivam Handa, Bryan Henry, Jim Huang, Alexander
Kapshuk, Anders Kaseorg, kehao95, Wolfgang Keller, Eddie Kohler, Austin
Liew, Imbar Marinescu, Yandong Mao, Matan Shabtay, Hitoshi Mitake, Carmi
Merimovich, Mark Morrissey, mtasm, Joel Nider, Greg Price, Ayan Shafqat,
Eldar Sehayek, Yongming Shen, Cam Tenny, tyfkda, Rafael Ubal, Warren
Toomey, Stephen Tu, Pablo Ventura, Xi Wang, Keiichi Watanabe, Nicolas
Wolovick, wxdao, Grant Wu, Jindong Zhang, Icenowy Zheng, and Zou Chang Wei.

The code in the files that constitute xv6 is
Copyright 2006-2018 Frans Kaashoek, Robert Morris, and Russ Cox.

ERROR REPORTS

We don't process error reports (see note on top of this file).

BUILDING AND RUNNING XV6

To build xv6 on an x86 ELF machine (like Linux or FreeBSD), run
"make". On non-x86 or non-ELF machines (like OS X, even on x86), you
will need to install a cross-compiler gcc suite capable of producing
x86 ELF binaries (see https://pdos.csail.mit.edu/6.828/).
Then run "make TOOLPREFIX=i386-jos-elf-". Now install the QEMU PC
simulator and run "make qemu".
