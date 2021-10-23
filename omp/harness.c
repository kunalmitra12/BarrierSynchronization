#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "gtmp.h"
#include <sys/time.h>

#define SECTOUSEC 1000000

int main(int argc, char **argv)
{
  int num_threads, num_iter = 100000;

  if (argc < 2)
  {
    fprintf(stderr, "Usage: ./harness [NUM_THREADS]\n");
    exit(EXIT_FAILURE);
  }
  num_threads = strtol(argv[1], NULL, 10);

  double thread_times[num_threads]; // store experiment results
  double threads_sum = 0;

  omp_set_dynamic(0);
  if (omp_get_dynamic())
    printf("Warning: dynamic adjustment of threads has been set\n");

  omp_set_num_threads(num_threads);

  gtmp_init(num_threads);

#pragma omp parallel shared(num_threads)
  {
    long delta_total;
    struct timeval start, end; // to record times
    int thread_num = omp_get_thread_num();
    int i;
    gettimeofday(&start, NULL);
    for (i = 0; i < num_iter; i++)
    {
      //printf("Hello world from thread %d of %d\n", thread_num, num_threads);
      gtmp_barrier();
      //printf("Hello world from thread %d of %d after barrier\n", thread_num, num_threads);
    }
    gettimeofday(&end, NULL);
    delta_total = (end.tv_sec * SECTOUSEC + end.tv_usec) - (start.tv_sec * SECTOUSEC + start.tv_usec);
    thread_times[thread_num] = delta_total / (double)num_iter;
  }

  gtmp_finalize();

  for (int i = 0; i < num_threads; i++)
  {
    threads_sum += thread_times[i];
  }

  double avg = threads_sum / (double)num_threads;
  printf("Average Time: %f\n", avg);

  return 0;
}
