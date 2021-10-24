#include <omp.h>
#include "gtmp.h"
#include <mpi.h>
#include <stdio.h>
#include <stdatomic.h>


#define TRUE 1
#define FALSE 0

int count;
int global_sense;
int thread_total;

void gtmp_init(int num_threads)
{
    thread_total = num_threads;
    count = num_threads;
    global_sense = TRUE; // initialize to true
    omp_set_num_threads(num_threads);
}

static atomic_int round = 1;
static atomic_int threads = 1;
void gtmp_barrier()
{
    int thread_num = omp_get_thread_num();
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    int local_sense = global_sense == 1 ? 0 : 1;
    if (__sync_fetch_and_sub(&count, 1) == 1)
    {
        count = thread_total;
        global_sense = local_sense;
    }
    else
    {
        while (global_sense != local_sense)
            ;
    }

    //printf("%d (T%d): Reached end of barrier (Round: %d)\n", rank, thread_num, round);
    if (threads % thread_total == 0)
        round++;
    threads++;
}

void gtmp_finalize()
{
}
