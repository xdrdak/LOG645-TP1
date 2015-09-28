#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "mpi.h"
#include <sys/time.h>

#define MASTER 0
#define MAX_MATRIX_SIZE 8
//Number of cells to send
#define CHUNK_SIZE 4

void printMatrix(unsigned long long arr[][MAX_MATRIX_SIZE]) {
  int i, j;
  for (i = 0; i < MAX_MATRIX_SIZE; i++) {
    for (j = 0; j < MAX_MATRIX_SIZE; j++) {
       printf("%llu  ", arr[i][j]);
    }
    printf("\n");
  }
}

void init_matrix(unsigned long long arr[][MAX_MATRIX_SIZE], int p) {
  int i,j;
  for(i = 0; i < MAX_MATRIX_SIZE; i++) {
    for(j = 0; j < MAX_MATRIX_SIZE; j++) {
      arr[i][j] = p;
    }
  }
}
//Find the x value based on our offset/index
unsigned long long x_o(int o) {
  return o%MAX_MATRIX_SIZE;
}
//Find the y value based on our offset/index
unsigned long long y_o(int o) {
  return o/MAX_MATRIX_SIZE;
}

int main(int argc, char **argv) {

  int err, processors, rank;
  err = MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &processors);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int choice = atoi(argv[1]);
  if ((choice == 1 || choice == 2) && argc == 4) {

    int offset;
    int p = atoi(argv[2]);
    int n = atoi(argv[3]);
    unsigned long long matrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
    unsigned long long chunk[CHUNK_SIZE+1]; //last position contains source

    MPI_Status status;

    if (rank == MASTER) {
      //Timer function : http://stackoverflow.com/questions/361363/how-to-measure-time-in-milliseconds-using-ansi-c
      struct timeval tval_before, tval_after, tval_result;
      gettimeofday(&tval_before, NULL);

      printf("Initializing Matrix\n");
      init_matrix(matrix, p);
      printMatrix(matrix);

      //Dispatch job to every worker;
      int i, chunk_index;

      for(i = 1; i < processors; i++) {
        MPI_Send(&p, 1, MPI_INT, i, i, MPI_COMM_WORLD);
      }

      //Equation 1 reassembly
      for (i = 1; i < processors; i++) {
        //Wait for whatever worker info
        MPI_Recv(&chunk[0], CHUNK_SIZE+1, MPI_LONG_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        offset = (chunk[CHUNK_SIZE] - 1) * CHUNK_SIZE;//translate the rank into a 1D array index
        for (chunk_index = 0; chunk_index < CHUNK_SIZE; chunk_index++) {
          matrix[y_o(offset)][x_o(offset)] = chunk[chunk_index];
          offset++;
        }
      }
      //End of Equation 1 reassembly
      printf("---------\nFinal Matrix\n");
      printMatrix(matrix);
      printf("---------\n");
      gettimeofday(&tval_after, NULL);
      timersub(&tval_after, &tval_before, &tval_result);
      printf("Time elapsed: %ld.%06ld seconds\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
    }

    //Worker process
    if (rank > MASTER) {
      //Wait until we receive something from master
      MPI_Recv(&p, 1, MPI_INT, MASTER, rank, MPI_COMM_WORLD, &status);
      int chunk_index, k;
      unsigned long long prevCellValue = 0;

      //initialize our first chunk based on p val.
      for (k = 0; k < CHUNK_SIZE; k++) {
        chunk[k] = p;
      }
      for (k = 1; k <= n; k++) {
        offset = (rank - 1) * CHUNK_SIZE; //translate the rank into a 1D array index

        if(choice == 2 && (rank-1) % 2  == 1){
          MPI_Recv(&prevCellValue, 1, MPI_LONG_LONG, rank-1, rank, MPI_COMM_WORLD, &status);
        }

        for (chunk_index = 0; chunk_index < CHUNK_SIZE; chunk_index++) {
          usleep(1000); //simulate hardwork
          if(choice == 2){
            if((rank-1) % 2 == 0 && x_o(offset) == 0){
               chunk[chunk_index] =  chunk[chunk_index] + ( y_o(offset) * k);
            }
            else if((rank-1) % 1 == 0 && chunk_index == 0){
              chunk[chunk_index] =  chunk[chunk_index] + prevCellValue * k;
            }
            else {
              chunk[chunk_index] =  chunk[chunk_index] + chunk[chunk_index-1] * k;
            }
          }
          else if(choice == 1){
            chunk[chunk_index] = chunk[chunk_index] + (x_o(offset) + y_o(offset)) * k;
          }
          offset++;
        } //End of calculated matrix "intensive" operations
        //Reset offset and recalculate x and y

        if(choice == 2 && (rank-1) % 2  == 0 ){
          MPI_Send(&chunk[CHUNK_SIZE-1], 1, MPI_LONG_LONG, rank+1, rank+1, MPI_COMM_WORLD);
        }

      } //End of K for loop
      chunk[CHUNK_SIZE] = rank; //set our worker id. Master needs the worker id to fuse array

      MPI_Send(&chunk[0], CHUNK_SIZE+1, MPI_LONG_LONG, MASTER, rank, MPI_COMM_WORLD);
      //End of Equation 1
    }
  }
  else {
    if(rank == MASTER)
      printf("Invalid parameters detected. Please check again!\n");
  }
  err = MPI_Finalize();
  return err;
}
