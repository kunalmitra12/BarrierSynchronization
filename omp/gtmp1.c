#include <omp.h>
#include "gtmp.h"

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

void gtmp_barrier()
{
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
}

void gtmp_finalize()
{
}
