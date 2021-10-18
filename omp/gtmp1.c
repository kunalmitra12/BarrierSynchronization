#include <omp.h>
#include "gtmp.h"

#define TRUE 1
#define FALSE 0

int count;
int sense;
int threadCount;

void gtmp_init(int num_threads)
{
    threadCount = num_threads;
    count = num_threads;
    sense = true;
    omp_set_num_threads(num_threads);
}

void gtmp_barrier()
{
    int localSense = sense == 1 ? 0 : 1;
    if (__sync_fetch_and_sub(&count, 1) == 1)
    {
        count = threadCount;
        sense = localSense;
    }
    else
    {
        while (sense != localSense)
            ;
    }
}

void gtmp_finalize()
{
}
