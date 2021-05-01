#!/bin/bash
echo " "
echo " "
echo "        ------------        FIRST COME FIRST SERVE        ----------"
echo " "
git checkout fcfs
make clean
make qemu
echo " "
echo " "
echo "        ------------        PRIORITY SCHEDULING        ------------"
echo " "
git checkout priority-scheduling
make clean
make qemu
echo " "
echo " "
echo "        ------------        MULTILEVEL QUEUE SCHEDULING        ------------"
echo " "
git checkout multilevel-queue
make clean
make qemu
echo " "
echo " "
echo "        ------------        MULTILEVEL FEEDBACK QUEUE SCHEDULING        ----------"
echo " "
git checkout multilevel-feedback-queue
make clean
make qemu
echo " "