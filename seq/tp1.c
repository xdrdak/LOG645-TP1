#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#define MAX_MATRIX_SIZE 8

void printMatrix(int arr[][MAX_MATRIX_SIZE]) {
  int i, j;
  for(i = 0; i < MAX_MATRIX_SIZE; i++) {
    for(j = 0; j < MAX_MATRIX_SIZE; j++) {
       printf("%d  ", arr[i][j]);
    }
    printf("\n");
  }
}

void init_matrix(int arr[][MAX_MATRIX_SIZE], int p) {
  int i,j;
  for(i = 0; i < MAX_MATRIX_SIZE; i++) {
    for(j = 0; j < MAX_MATRIX_SIZE; j++) {
      arr[i][j] = p;
    }
  }
}

int formula_1(int arr[][MAX_MATRIX_SIZE], int i, int j, int k) {
  return arr[i][j] + (i + j) * k;
}

int formula_2(int arr[][MAX_MATRIX_SIZE], int i, int j, int k) {
  if (j == 0) {
    return arr[i][j] + (i * k);
  }
  else {
    return arr[i][j] + arr[i][j-1] * k;
  }
}

int main(int argc, char **argv) {
  int choice = atoi(argv[1]);

  if ((choice == 1 || choice == 2) && argc == 4) {
    //Timer function : http://stackoverflow.com/questions/361363/how-to-measure-time-in-milliseconds-using-ansi-c
    struct timeval tval_before, tval_after, tval_result;
    gettimeofday(&tval_before, NULL);

    int p = atoi(argv[2]);
    int n = atoi(argv[3]);
    int matrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];

    printf("Initializing Matrix\n");
    init_matrix(matrix, p);
    printMatrix(matrix);

    printf("---------\nFinal Matrix\n");
    int i, j, k;

    for(k = 1; k <= n; k++) {
      for(i = 0; i < MAX_MATRIX_SIZE; i++) {
        for(j = 0; j < MAX_MATRIX_SIZE; j++) {
          usleep(1000); //simulate hardwork
          if (choice == 1) {
            matrix[i][j] = formula_1(matrix, i, j, k);
          }
          else if(choice == 2) {
            matrix[i][j] = formula_2(matrix, i, j, k);
          }
        }
      } //End of calculated matrix "intensive" operations
    } //End of K for

    printMatrix(matrix);
    printf("---------\n");

    gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);
    printf("Time elapsed: %ld.%06ld seconds\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
    return 0;
  }
  else {
    printf("Invalid parameters detected. Please check again!\n");
    return 1;
  }

  return 0;
}
