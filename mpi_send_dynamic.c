#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"


//---------------------------------------------------------------
// 双进程测试，根据MPI_Status动态确定所需要接收的数据的大小
// Usage:
//    MPI_Probe(
//        int source,
//        int tag,
//        MPI_Comm comm,
//        MPI_Status* status)
//    )在实际发送消息之前可以使用该函数查询消息的大小，通过指定
//    communicator, 查询指定源指定标签下的数据量，然后可以使用
//    MPI_Get_count(
//        const MPI_Status *status,
//        MPI_Datatype,
//        int *count
//    )
//    得到需要接受的数据量
//---------------------------------------------------------------

int main (int argc, char **argv) {
  int world_rank;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int num_amount;

  if (world_rank == 0) {
    const int MAX_NUMS = 100;
    int data[MAX_NUMS];

    srand(time(NULL));
    num_amount = (rand() / (float)RAND_MAX) * MAX_NUMS;

    // Send random amount integers to p 1
    MPI_Send(&data, num_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
    printf("Process 0 send %d numbers to p 1\n", num_amount);
  } else if (world_rank == 1) {
    MPI_Status state;

    MPI_Probe(0, 0, MPI_COMM_WORLD, &state);
    MPI_Get_count(&state, MPI_INT, &num_amount);
    int *buff = (int*)malloc(sizeof(int)*num_amount);
    MPI_Recv(buff, num_amount, MPI_INT, 0, 0, MPI_COMM_WORLD, &state);
    printf("Process dynamically received %d numbers from 0\n", num_amount);
    free(buff);
  }

  MPI_Finalize();
}
