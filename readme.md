## Barrier Synchronization

Implementation of Sense Reversal and Dissemination barriers using OpenMP and Tree and Tournament barrier using MPI + PDF report of testing/experiments/analysis.

## Overview

OpenMP allows you to run parallel algorithms on shared-memory multiprocessor/multicore machines. For this assignment you will implement two spin barriers using OpenMP. MPI allows you to run parallel algorithms on distributed memory systems, such as compute clusters or other distributed systems. You will implement two spin barriers using MPI. Finally, you will choose one of your OpenMP barrier implementations and one of your MPI barrier implementations and combine the two in an MPI-OpenMP combined program in order to synchronize between multiple cluster nodes that are each running multiple threads. 

You will run experiments to evaluate the performance of your barrier implementations (information about compute resources for running experiments is in a later section). You will run your OpenMP barriers on an 8-way SMP (symmetric multi-processor) system, and your MPI and MPI-OpenMP combined experiments on a cluster of up to 24 nodes with 12 cores each.

Finally, you will create a write-up that explains what you did, presents your experimental results, and most importantly, analyzes your results to explain the trends and phenomona you see (some hints for analysis are given below).

## Detailed Instructions

These instructions are presented in a sequential order. However, depending on how you decide to divide the work with your project parter, you may choose do so some of these things in parallel. That is okay, so long as everything gets done, and you say who did what in your write-up.

### Part 1: Learn about OpenMP and MPI

The first thing you want to do is learn how to program, compile, and run OpenMP and MPI programs.

#### Setup

Included with this project is a Vagrantfile which provides a VM with the required environment and installs.

Assuming you don't have it installed already, download [the latest version of Vagrant for your platform](https://www.vagrantup.com/downloads). You'll also need the latest version of VirtualBox, which can be found [here](https://www.virtualbox.org/wiki/Downloads).

Start the virtual machine using `vagrant up` and connect to it using `vagrant ssh`. Please note that Vagrant mounts the directory containing the Vagrantfile at `/vagrant`. Once you log into the VM, you can access the project files by changing your current directory via `cd /vagrant`. For more information, use `vagrant --help`.

#### OpenMP
You can compile and run OpenMP programs on any Linux machine that has libomp installed. You can try the example code in the assignment folder (examples/openmp.tar.gz).
Additional informational resources are as follows:
- [OpenMP Website](https://www.topenmp.org//)
- [OpenMP Specification](https://www.openmp.org/specifications/)
- [Introduction to OpenMP (video series)](https://www.openmp.org/uncategorized/tutorial-introduction-to-openmp/)
- [LLNL's OpenMP Tutorial](https://computing.llnl.gov/tutorials/openMP/)

#### MPI
  You can compile and run MPI programs on any Linux machine that has openmpi installed (Note: OpenMPI != OpenMP). Although MPI is normally used for performing computations across different network-connected machines, it will also run on a single machine. This setup can be used for developing and testing your project locally.  You can try running the example code in the assignment folder (examples/mpi.tar.gz), as well as looking at the following informational resources:
- [OpenMPI Website](https://www.open-mpi.org/)

### Part 2: Develop OpenMP Barriers

Implement two spin barriers using OpenMP. You may choose any two spin barriers you like. For example, you could use ones from the MCS paper, anything covered in lecture, or any variation on these you think of. Obviously, your barrier implementations cannot use the built-in OpenMP barrier! However you can optionally use it as a third barrier in your experiments for baseline/control purposes, if you choose.

#### Given to you:
1. The barrier function interfaces are specified in `omp/gtmp.h` in the assignment folder. 
2. The `omp/harness.c` is a rudimentary test harness for you to test your implementation. Feel free to modify the harness to your needs.
3. You may run `make clean; make all` to generate the `omp1` and `omp2` binaries.

#### What you need to do:
Complete the implementations of your 2 barriers in `omp/gtmp1.c` and `omp/gtmp2.c`

### Part 3: Develop MPI Barriers

Implement two spin barriers using MPI. At least one of these implementations must be a tree-based barrier (if you choose to do both as tree-based barriers, that's okay too). You may also opt for one of these implementations to use the same algorithm you chose for one of your OpenMP barriers, but the other one must be different. (However, even if you choose the same algorithm for one of them, you may find that you must implement it very differently when using MPI vs. using OpenMP). Obviously, your barrier implementations cannot use the built-in MPI barrier! However you can optionally use it as a third barrier in your experiements, as a baseline/control, if you choose.

#### Given to you:
1. The barrier function interfaces are specified in `mpi/gtmpi.h` in the assignment folder. 
2. The `mpi/harness.c` is a rudimentary test harness for you to test your implementation. Feel free to modify the harness to your needs.
3. You may run `make clean; make all` to generate the `mpi1` and `mpi2` binaries.

#### What you need to do:
Complete the implementations of your 2 barriers in `mpi/gtmpi1.c` and `mpi/gtmpi2.c`

### Part 4: Develop MPI-OpenMP Combined Barrier

Now choose one of the OpenMP barriers you implemented, and one of the MPI barriers you implemented. Combine them to create a barrier that synchronizes between multiple nodes that are each running multiple threads. You'll also want to be sure to preserve your original code for the two barriers so that you can still run experiments on them separately. You can compare the performance of the combined barrier to your standalone MPI barrier. Note that you will need to run more than one MPI process per node in the standalone configuration to make a comparable configuration to one multithreaded MPI process per node in the combined configuration, so that total number of threads is the same when you compare.

#### Given to you:
You are given a template `combined/Makefile` which generates a binary named `combined` to test the combined barrier. 

#### What you need to do:
Implement the combined barrier along with your own testing harness to generate a binary named "combined". Please provide the appropriate `Makefile`. The invocation for the binary will be as follows:

`mpirun -np <num_proc> ./combined <num_proc> <num_threads> <num_rounds>`

### Part 5: Run Experiments

1. The next step is to do a performance evaluation of your barriers. You may need to modify/write the given test harnesses that runs some OpenMP threads or MPI processes and synchronizes the threads/processes using your barrier implementation. Then your test harness should measure the performance of your barriers in a manner similar to the MCS paper. You should look at the experiments in that paper again and think about how they were conducted.
2. You will measure your OpenMP barriers on a single cluster node, and scale the number of threads from 2 to 8.
3. You will measure your MPI barriers on multiple cluster nodes. You should scale from 2 to 12 MPI processes, one process per node.
4. You will measure your MPI-OpenMP combined barrier on multiple cluster nodes, scaling from 2 to 8 MPI processes running 2 to 12 OpenMP threads per process. 
