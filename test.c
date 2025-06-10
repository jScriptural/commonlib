
#include "matrix.h"
#include <errno.h>
#define N 255



int main(void)
{
  double A[1][3] = {-2/5.0,1/5.0,1};
  double B[1][3]= {-2,1,5};
  double c[1][3] = {0};

  matrix_print("A",1,3,A);
  matrix_print("B",1,3,B);

  vector_crossproduct(1,3,A,B,c);
  
  matrix_print("AхB",1,3,c);


  return 0;
}


