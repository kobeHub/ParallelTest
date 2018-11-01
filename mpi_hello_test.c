#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
  int rank, tot, i;
  char msg[128], rev[128];
  MPI_Status state;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &tot);

  if(rank == 0) {
    for(i = 0; i < tot; i++) {
      sprintf(msg, "Hello %d, this is zero, I'm your master", i);
      MPI_Send(msg, 128, MPI_CHAR, i, 0, MPI_COMM_WORLD);
    }
    for(i = 1; i < tot; i++) {
      MPI_Recv(rev, 128, MPI_CHAR, i, 0, MPI_COMM_WORLD, &state);
      printf("P%d got: %s\n", rank, rev);
    }
  } else {
    MPI_Recv(rev, 128, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &state);
    printf("P%d got: %s\n", rank, rev);
    sprintf(msg, "Hello, zero, this is %d, I'm your slaver", rank);
    MPI_Send(msg, 128, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  }
  MPI_Finalize();
}
