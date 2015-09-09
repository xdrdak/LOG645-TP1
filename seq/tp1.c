#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_MATRIX_SIZE 8

void print_row(int a[MAX_MATRIX_SIZE])
{
  printf("%d %d %d %d %d %d %d %d \n"
      , a[0]
      , a[1]
      , a[2]
      , a[3]
      , a[4]
      , a[5]
      , a[6]
      , a[7]);
}

void program_one(int p, int n)
{

  int matrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
  int calculated_matrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];

  int i, j, k;
  printf("Program 1 - Initial matrix\n");

  //Initialize the matrix
  for ( i = 0; i < MAX_MATRIX_SIZE; i++ )
  {
    for ( j = 0; j < MAX_MATRIX_SIZE; j++ )
    {
      //Simulate intensive calculation
      //usleep(1000);
      matrix[i][j] = p;
    }

    print_row(matrix[i]);
  }

  printf("--------\n");
  printf("Final Matrix\n");

  for(k = 1; k <= n; k++)
  {
    for ( i = 0; i < MAX_MATRIX_SIZE; i++ )
    {
      for ( j = 0; j < MAX_MATRIX_SIZE; j++ )
      {
        //Simulate intensive calculation
        //usleep(1000);
        calculated_matrix[i][j] = matrix[i][j] + (i + j) * k;
      }

      if(k == n)
      {
        print_row(calculated_matrix[i]);
      }
    } //End of calculated matrix "intensive" operations

    //Store values
    for(i = 0; i < MAX_MATRIX_SIZE; i++)
    {
      memcpy(&matrix[i], &calculated_matrix[i], sizeof(calculated_matrix[0]));
    }

  }


}

void program_two(int p, int n)
{
  int matrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
  int calculated_matrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];

  int i, j, k;
  printf("Program 2 - Initial matrix\n");

  //Initialize the matrix
  for ( i = 0; i < MAX_MATRIX_SIZE; i++ )
  {
    for ( j = 0; j < MAX_MATRIX_SIZE; j++ )
    {
      //Simulate intensive calculation
      //usleep(1000);
      matrix[i][j] = p;
    }

    print_row(matrix[i]);
  }

  printf("---------\n");
  printf("Final Matrix\n");

  for(k = 1; k <= n; k++)
  {
    for ( i = 0; i < MAX_MATRIX_SIZE; i++ )
    {
      for ( j = 0; j < MAX_MATRIX_SIZE; j++ )
      {
        //Simulate intensive calculation
        //usleep(1000);
        if (j == 0)
        {
          calculated_matrix[i][j] = matrix[i][j] + (i * k);
        }
        else
        {
          calculated_matrix[i][j] = matrix[i][j] + calculated_matrix[i][j-1] * k;
        }

      }

      if(k == n)
      {
        print_row(calculated_matrix[i]);
      }
    } //End of calculated matrix "intensive" operations

    //Store values
    for(i = 0; i < MAX_MATRIX_SIZE; i++)
    {
      memcpy(&matrix[i], &calculated_matrix[i], sizeof(calculated_matrix[0]));
    }

  }

}

int main(int argc, char **argv)
{

  if(argc == 4 && *argv[1] == '1')
  {
    program_one(atoi(argv[2]), atoi(argv[3]));
  }
  else if(argc == 4 && *argv[1] == '2')
  {
    program_two(atoi(argv[2]), atoi(argv[3]));
  }
  else
  {
    printf("nope \n");
  }

  return 0;
}
