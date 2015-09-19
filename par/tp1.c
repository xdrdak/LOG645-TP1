#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "mpi.h"
#include <sys/time.h>

#define MASTER 0
#define MAX_MATRIX_SIZE 8

void printMatrix(int arr[][MAX_MATRIX_SIZE])
{
  int i, j;
  for(i = 0; i < MAX_MATRIX_SIZE; i++)
  {
    for(j = 0; j < MAX_MATRIX_SIZE; j++)
    {
       printf("%d  ", arr[i][j]);
    }
    printf("\n");
  }
}

void init_matrix(int arr[][MAX_MATRIX_SIZE], int p)
{
  int i,j;
  for(i = 0; i < MAX_MATRIX_SIZE; i++)
  {
    for(j = 0; j < MAX_MATRIX_SIZE; j++)
    {
      arr[i][j] = p;
    }
  }
}
int x_o(int o)
{
  return o%MAX_MATRIX_SIZE;
}

int y_o(int o)
{
  return o/MAX_MATRIX_SIZE;
}

int main(int argc, char **argv)
{

  int err, processors, rank;
  err = MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &processors);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int choice = atoi(argv[1]);
  if ((choice == 1 || choice == 2) && argc == 4)
  {

    int offset;
    int p = atoi(argv[2]);
    int n = atoi(argv[3]);
    int matrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
    int chunk[5]; //5th position contains source

    MPI_Status status;

    if (rank == MASTER)
    {
      struct timeval tval_before, tval_after, tval_result;
      gettimeofday(&tval_before, NULL);

      printf("Initializing Matrix\n");
      init_matrix(matrix, p);
      printMatrix(matrix);

      //Dispatch job to every worker;
      int i, chunk_index;

      for(i = 1; i < processors; i++)
      {
        MPI_Send(&p, 5, MPI_INT, i, i, MPI_COMM_WORLD);
      }

      //Wait for workers to complete tasks
      for(i = 1; i < processors; i++)
      {
        MPI_Recv(&chunk[0], 5, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        offset = (chunk[4] - 1)*4;
        for(chunk_index = 0; chunk_index < 4; chunk_index++)
        {
          matrix[x_o(offset)][y_o(offset)] = chunk[chunk_index];
          offset++;
        }
      }
      printf("---------\n");
      printf("Final Matrix\n");
      printMatrix(matrix);

      gettimeofday(&tval_after, NULL);
      timersub(&tval_after, &tval_before, &tval_result);
      printf("Time elapsed: %ld.%06ld seconds\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
    }

    if (rank > MASTER)
    {
      //Wait until we receive something from master
      MPI_Recv(&p, MAX_MATRIX_SIZE^2, MPI_INT, MASTER, rank, MPI_COMM_WORLD, &status);
      int chunk_index, k;
      for(k = 0; k < 4; k++)
      {
        chunk[k] = p;
      }
      for(k = 1; k <= n; k++)
      {
        offset = (rank - 1)*4;

        for(chunk_index = 0; chunk_index < 4; chunk_index++)
        {
          usleep(1000);
          chunk[chunk_index] = chunk[chunk_index] + (x_o(offset) + y_o(offset)) * k;
          offset++;
        } //End of calculated matrix "intensive" operations
        //Reset offset and recalculate x and y

      } //End of K for
      chunk[4] = rank;
      MPI_Send(&chunk[0], 5, MPI_INT, MASTER, rank, MPI_COMM_WORLD);
    }
  }
  else
  {
    if(rank == MASTER)
      printf("Invalid parameters detected. Please check again!\n");
  }
  err = MPI_Finalize();
  return err;
}
