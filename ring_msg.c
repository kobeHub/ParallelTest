#include <stdio.h>
#include "mpi.h"

int main (int argc, char **argv) {
  int world_rank, world_size;
  MPI_Status state;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int data;
  if (world_rank != 0) {
    MPI_Recv(&data, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process %d received token %d from process %d\n", world_rank, data, world_rank-1);
    data++;
  } else {
    data = -1;
  }
  MPI_Send(&data, 1, MPI_INT, (world_rank + 1)%world_size, 0, MPI_COMM_WORLD);

  if (world_rank == 0) {
    MPI_Recv(&data, 1, MPI_INT, world_size-1, 0, MPI_COMM_WORLD, &state);
    printf("Process %d received %d from process %d\n", world_rank, data, world_size-1);
    printf("\nThe Status: count_lo:%d, count_hi_and_cancelled:%d, MPI_SOURCE:%d"
          ",MPI_TAG:%d, MPI_ERROR:%d\n", state.count_lo, state.count_hi_and_cancelled,
          state.MPI_SOURCE, state.MPI_TAG, state.MPI_ERROR);
  }

  MPI_Finalize();
}
