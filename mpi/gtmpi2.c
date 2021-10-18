#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include "gtmpi.h"
#include <math.h>

#define WINNER 0
#define LOSER 1

typedef struct tournament_node_data{
    int numRounds;
    int * roles;
    int * opponents;
}tournament_node;

static tournament_node* node;

void gtmpi_init(int num_processes){
    node = (tournament_node*) malloc(sizeof(tournament_node));
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    node->numRounds = ceil(log(num_processes) / log(2));
    node->roles = (int*) malloc(sizeof(int)*node->numRounds);
    node->opponents = (int*) malloc(sizeof(int)*node->numRounds);

    for(int k=1; k<= node->numRounds; ++k){
        if ((rank % (int)pow(2,k) == 0) && ((rank + pow(2,k-1)) < num_processes)) 
        {
            node->roles[k-1] = WINNER;
            node->opponents[k-1] = rank + pow(2, k-1);
        }
        else if ((rank % (int)pow(2,k)) == (pow(2,k-1)))
        {
            node->roles[k-1] = LOSER;
            node->opponents[k-1] = rank - pow(2, k-1);
        } 
        else
        {
            node->roles[k-1] = -1;
            node->opponents[k-1] = -1;
        }
    }
}
static int bRound = 1;
void gtmpi_barrier(){
    int dummy_msg = 0;
    MPI_Status mpi_result;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //Arrivals
    for(int k=0; k < node->numRounds; ++k){
        if (node->roles[k] == LOSER)
            MPI_Send(&dummy_msg, 1, MPI_INT, node->opponents[k], 0, MPI_COMM_WORLD);
        else if (node->roles[k] == WINNER)
            MPI_Recv(&dummy_msg, 1, MPI_INT, node->opponents[k], 0, MPI_COMM_WORLD, &mpi_result);
    }

    //Wake ups
    for(int k=node->numRounds - 1; k > -1 ; --k){
        if (node->roles[k] == WINNER)
            MPI_Send(&dummy_msg, 1, MPI_INT, node->opponents[k], 0, MPI_COMM_WORLD);
        else if (node->roles[k] == LOSER)
            MPI_Recv(&dummy_msg, 1, MPI_INT, node->opponents[k], 0, MPI_COMM_WORLD, &mpi_result);
    }
    printf("%d: Reached end of barrier (Round: %d)\n", rank, bRound);
    bRound++;
}

void gtmpi_finalize(){
    free(node->roles);
    free(node->opponents);
    free(node);
}
