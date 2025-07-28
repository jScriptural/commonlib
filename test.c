
#include "matrix.h"
#include <errno.h>
#define N 255



int main(void)
{
  const double A[3][3]  = {1,2,3,7,1,5,2,8,2};
  double adjA[3][3] = {0};
  double prod[3][3] = {0};

  matrix_adjoint(3,A,adjA);
  matrix_print("A",3,3,A);
  matrix_print("adjA",3,3,adjA);

  matrix_multiply(3,3,A,3,adjA,prod);
  matrix_print("adjA",3,3,prod);
  return 0;
}


