#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include "gtmpi.h"

typedef struct mcs_node_data{
    int arrivalChildren[4];
    int wakeChildren[2];
    int arrivalParent;
    int wakeParent;
}mcs_node;

static mcs_node* node;

void gtmpi_init(int num_processes){
    node = (mcs_node*) malloc(sizeof(mcs_node));

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int childRank;

    // Create node for arrival tree
    for (int i=0; i<4; ++i){
        childRank = 4 * rank + i + 1;
        if (childRank < num_processes - 1)
            node->arrivalChildren[i] = 4 * rank + i + 1;
        else 
            node->arrivalChildren[i] = -1;
    }

    //Create node for wake up tree
    node->wakeChildren[0] = (2 * rank + 1) < num_processes - 1 ? (2 * rank + 1) : -1;
    node->wakeChildren[1] = (2 * rank + 2) < num_processes - 1 ? (2 * rank + 2) : -1;

    if (rank!=0) {
        node->arrivalParent = (rank - 1) / 4;
        node->wakeParent = (rank - 1) / 2;
    } else {
        node->arrivalParent = -1;
        node->wakeParent = -1;
    }
}

void gtmpi_barrier(){
    int dummy_msg = 0;
    MPI_Status mpi_result;
    // Wait on children
    for (int i=0; i<4; ++i){
        if(node->arrivalChildren[i] != -1)
            MPI_Recv(&dummy_msg, 1, MPI_INT, node->arrivalChildren[i], 0, MPI_COMM_WORLD, &mpi_result);
    }

    // root has no parent
    if (node->arrivalParent != -1){
        //Notify parent of arrival
        MPI_Send(&dummy_msg, 1, MPI_INT, node->arrivalParent, 0, MPI_COMM_WORLD);
        // Wait for parent to wake node
        MPI_Recv(&dummy_msg, 1, MPI_INT, node->wakeParent, 0, MPI_COMM_WORLD, &mpi_result);
    }

    if (node->wakeChildren[0] != -1)
        MPI_Send(&dummy_msg, 1, MPI_INT, node->wakeChildren[0], 0, MPI_COMM_WORLD);

    if (node->wakeChildren[1] != -1)
        MPI_Send(&dummy_msg, 1, MPI_INT, node->wakeChildren[1], 0, MPI_COMM_WORLD);
}

void gtmpi_finalize(){
    free(node);
}
