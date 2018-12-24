#include <stdio.h>
#include <string.h>
#include <mpi.h>

//-------------------------------------------------------------------
// Usage of MPI_Send(
//      const void *buf,          发送的数据
//      int count,                数据长度
//      MPI_Datatype datatype,    数据类型
//      int dest,                 目标线程
//      int tag,                  数据标签
//      MPI_Comm comm             Communicator
// )
//
// MPI_Recv(
//      const void *buf,
//      int count,
//      MPI_Datatype,
//      int source,
//      int tag,
//      MPI_Comm,
//      MPI_Status,              接受到数据的状态
// )
//-------------------------------------------------------------------


int main(int argc, char* argv[]) {
  int rank, tot, i;
  char msg[128], rev[128];
  MPI_Status state;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &tot);

  printf("The rank: %d, the size: %d\n", rank, tot);
  if(rank == 0) {
    for(i = 1; i < tot; i++) {
      sprintf(msg, "Hello %d, this is zero, I'm your master", i);
      MPI_Send(msg, 128, MPI_CHAR, i, 0, MPI_COMM_WORLD);
    }
    for(i = 1; i < tot; i++) {
      MPI_Recv(rev, 128, MPI_CHAR, i, 0, MPI_COMM_WORLD, &state);
      printf("P%d got: %s\n", rank, rev);
    }

    printf("MPI_Status print:\n");
    printf("count_lo:%d\ncount_hi_and_cancelled:%d\nMPI_SOURCE:%d\nMPI_TAG:%d\nMPI_ERROR:%d\n",
        state.count_lo, state.count_hi_and_cancelled, state.MPI_SOURCE, state.MPI_TAG, state.MPI_ERROR);
  } else {
    MPI_Recv(rev, 128, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &state);
    printf("P%d got: %s\n", rank, rev);
    sprintf(msg, "Hello, zero, this is %d, I'm your slaver", rank);
    MPI_Send(msg, 128, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  }
  MPI_Finalize();
}
