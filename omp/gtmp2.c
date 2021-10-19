#include <omp.h>
#include "gtmp.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"

#define TRUE 1
#define FALSE 0

typedef struct
{
    int myflags[2][3]; // max val of logP is 3 (P max is 8)
    int *partnerflags[2][3];
} flags;

flags *allnodes;

int P;
int logP;

void gtmp_init(int num_threads)
{
    P = num_threads;
    logP = (int)ceil(log(P) / log(2));
    allnodes = (flags *)malloc(P * sizeof(flags));
    for (int i = 0; i < P; i++)
    {
        for (int r = 0; r < 2; r++)
        {
            for (int k = 0; k < logP; k++)
            {
                (allnodes + i)->myflags[r][k] = FALSE;
                int tmp = (i + (int)pow(2, k)) % P;
                for (int j = 0; j < P; j++)
                {
                    if (j == tmp)
                    {
                        //printf("thread %d to point to thread %d\n", i, j);
                        (allnodes + i)->partnerflags[r][k] = &((allnodes + j)->myflags[r][k]);
                    }
                }
            }
        }
    }
    omp_set_num_threads(num_threads);
}

void gtmp_barrier()
{
    // processor private variables
    int *parity;
    parity = (int *) malloc(sizeof(int));
    *parity = 0;
    int *sense;
    sense = (int *) malloc(sizeof(int));
    *sense = TRUE;

    int tid = omp_get_thread_num();

    flags *localflags = (allnodes + tid);

    for (int instance = 0; instance < logP; instance++)
    {
#pragma omp critical
        {
            *(localflags->partnerflags[*parity][instance]) = *sense;
        }
        while (localflags->myflags[*parity][instance] != *sense)
            /*printf("tid: %d waiting here\n", thread_num)*/;
    }
    if (*parity == 1)
    {
        printf("here\n");
        *sense = !(*sense);
    }
    *parity = 1 - *parity;
}

void gtmp_finalize()
{
    free(allnodes);
}
