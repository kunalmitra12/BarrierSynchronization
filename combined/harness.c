#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <sys/time.h>   // for gettimeofday()
#include <omp.h>
#include "gtmp.h"
#include "gtmpi.h"

#define SECTOUSEC 1000000

int main(int argc, char** argv)
{
    int num_threads, num_iter=10;
    int num_processes;

    MPI_Init(&argc, &argv);

    num_threads = strtol(argv[2], NULL, 10);
    num_processes = strtol(argv[1], NULL, 10);
    num_iter = strtol(argv[3], NULL, 10);

    double time; // store experiment results
    double threads_sum = 0;

    omp_set_dynamic(0);
    if (omp_get_dynamic())
    printf("Warning: dynamic adjustment of threads has been set\n");

    omp_set_num_threads(num_threads);

    gtmp_init(num_threads);
    gtmpi_init(num_processes);

    long delta_total;
    struct timeval start, end; // to record times
    int thread_num = omp_get_thread_num();
    int i;

    //combined_init(num_threads, num_processes);
    gettimeofday(&start, NULL);
    for(i=0; i < num_iter; ++i)
    {
        #pragma omp parallel shared(num_threads)
        {
            gtmp_barrier();
        }
        gtmpi_barrier();
    }
    gettimeofday(&end, NULL);
    delta_total = (end.tv_sec * SECTOUSEC + end.tv_usec) - (start.tv_sec * SECTOUSEC + start.tv_usec);
    time = delta_total / (double)num_iter;
    gtmpi_finalize();
    MPI_Finalize();
    gtmp_finalize();

    printf("Average Time: %f\n", time);

    return 0;
}