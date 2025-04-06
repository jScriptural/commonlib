
#include "matrix.h"
#include <errno.h>
#define N 255



int main(void)
{
  double A[4][4] = {[0][0]=3, [1][1]=5, [2][2]=-3,[3][3]=4};
  double B[4][4] = {1,2,3,4,5,6,7,8,9,8,5,2,1,4,0,-24};
  double C[4][4]={0};

  matrix_print("A",4,4,A);
  matrix_print("B",4,4,B);


  errno=0;
  matrix_sub(4,4,B,A,C);
  if(errno == 0)
    matrix_print("B-A",4,4,C);
  else 
    perror("product");

  return 0;
}


