#include <stdio.h>
#include <mpi.h>

#define Limit 15

int main(int argc, char *argv[]) {
  int world_rank, size;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  int pingpang_count = 0;
  int partner_rank = (world_rank + 1) % 2;

  while (pingpang_count < Limit) {
    if (world_rank == pingpang_count % 2) {
      // The first player to increase the count
      pingpang_count++;
      MPI_Send(&pingpang_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
      printf("Player%d increased the pingpang to %d, and send to Player%d\n",
          world_rank, pingpang_count, partner_rank);
    } else {
      MPI_Recv(&pingpang_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD,
          MPI_STATUS_IGNORE);
      printf("Player%d received the pingpang of %d, from Player%d\n",
         world_rank,  pingpang_count, partner_rank);
    }
  }
}
