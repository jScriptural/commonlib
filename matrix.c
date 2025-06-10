/* MATRIX.C
 *
 * Author:Isonguyo John
 * Email: isonguyojohndeveloper@gmail.com
 *
 * Created on 06/04/2025.
 * Modified by: Isonguyo John
 * Last modified: 23/05/2025.
 *
 * C library of functions for working with 2D
 * matrices.
 */

#include "matrix.h"


void matrix_minor(int n, const double m[restrict n][n],double minor[restrict n-1][n-1], int row, int col)
{
  for(int r=0,i=0; r < n; ++r)
  {
    if(r == row-1)
      continue;

    for(int c=0,j=0; c < n; ++c)
    {
      if(c == col-1)
	continue;

      minor[i][j++] = m[r][c];
    }
    ++i;
  }
    
}


void matrix_print(char *tag,int nrows, int ncols, const double matrix[nrows][ncols])
{
  printf("%s: [\n",tag);
  for(int r =0; r < nrows; ++r)
  {
    for(int c =0; c < ncols; ++c)
    {
      if( c == ncols-1)
	printf("%3g\r\n ", matrix[r][c]);
      else
	printf("%3g, ", matrix[r][c]);
    }

  }

  printf("]\r\n\n");

}



double matrix_determinant(int n, double const m[n][n])
{

  if(n == 1)
    return m[0][0];
  
  if(n == 2)
    return m[0][0]*m[1][1] - m[0][1]*m[1][0];


  double det =0;
  double minor[n-1][n-1];
  int r=0;
  for(int i = 0; i < n; ++i)
  {
    matrix_minor(n,m,minor,r+1, i+1);
    if((i+r+2) % 2 == 0)
      det +=  m[r][i] * matrix_determinant(n-1,minor);
    else
      det += m[r][i] * -1 * matrix_determinant(n-1,minor);
  }

  return det;

}



void matrix_transpose(int nrows, int ncols,double const m[restrict nrows][ncols], double t[restrict ncols][nrows])
{
  for(int r=0; r < nrows; ++r)
    for(int c=0; c < ncols; ++c)
      t[c][r] = m[r][c];
}



void matrix_multiply(int r1,int c1,double const m1[restrict r1][c1],int c2,double const m2[restrict c1][c2], double res[restrict r1][c2])
{
  for(int i=0; i < r1; ++i)
  {
    for(int j=0; j < c2; ++j)
    {
      double sum =0;
      for(int k =0; k < c1; ++k)
	sum += m1[i][k]*m2[k][j];

      res[i][j] = sum;
    }
  }
}



void matrix_scalarm(int nr,int nc, double const m[restrict nr][nc],double res[restrict nr][nc], double s)
{

  for(int i = 0; i < nr; ++i)
    for(int j=0; j < nc; ++j)
      res[i][j] = s * m[i][j];
}

void matrix_adjoint(int n, double const m[restrict n][n], double adj[restrict n][n])
{
  if(n == 2)
  {
    adj[0][0] = m[1][1];
    adj[0][1] = -1 * m[0][1];
    adj[1][0] = -1 * m[1][0];
    adj[1][1] = m[0][0];
    return;
  }

  double cof[n][n];
  for(int r=0; r < n; ++r)
  {
    for(int c = 0; c < n; ++c)
    {
      double minor[n-1][n-1];
      matrix_minor(n,m,minor,r+1,c+1);
      if((r+c+2) % 2 == 0) 
	cof[r][c] = matrix_determinant(n-1,minor);
      else
	cof[r][c] = (-1) * matrix_determinant(n-1,minor);
    }
  }
  matrix_transpose(n,n,cof,adj);
}



void matrix_inverse(int n, double const m[restrict n][n], double inv[restrict n][n])
{
  double detm;
  double adj[n][n];


  detm = matrix_determinant(n,m);
  matrix_adjoint(n,m,adj);
  
  for(int r=0; r < n; ++r)
    for(int c=0; c < n; ++c)
      inv[r][c] = adj[r][c] / detm;

}



double vector_dotproduct(int nr,int nc,const double m1[nr][nc], const double m2[nr][nc])
{
  if(nr != 1 && nc != 1)
  {
    errno = EINVAL;
    return 0;
  }

  double sum = 0;
  if(nr == 1)
    for(int j=0; j < nc; ++j)
      sum += m1[0][j] * m2[0][j];
  else
    for(int j=0; j < nr; ++j)
      sum += m1[j][0] * m2[j][0];

  return sum;

}



void vector_crossproduct(int nr,int nc, const double m1[nr][nc],const double m2[nr][nc], double res[nr][nc])
{
  if((nr != 1 && nc != 1)||(nr != 3 && nc != 3))
  {
    errno = EINVAL;
    return;
  }


  if(nr == 1 && nc == 3)
  {
    double m[3][3] = { 1,1,1,m1[0][0],m1[0][1],m1[0][2],m2[0][0],m2[0][1],m2[0][2]};
    double minor[2][2];
    for(int i=0; i < 3; ++i)
    {
      matrix_minor(3,m,minor,1,i+1);
      if(i % 2 == 0)
	res[0][i] = matrix_determinant(2,minor);
      else
	res[0][i] = -1 * matrix_determinant(2,minor);
    }
  }
  
  if(nr == 3 && nc == 1)
  {
    double m[3][3] = { 1,1,1,m1[0][0],m1[1][0],m1[2][0],m2[0][0],m2[1][0],m2[2][0]};
    double minor[2][2];
    for(int i=0; i < 3; ++i)
    {
      matrix_minor(3,m,minor,1,i+1);
      if(i % 2 == 0)
	res[i][0] = matrix_determinant(2,minor);
      else
	res[i][0] = -1 * matrix_determinant(2,minor);
    }
  }
}



void matrix_concatc(int nr, int nc1, const double m1[restrict nr][nc1], int nc2,const double m2[restrict nr][nc2], double res[restrict nr][nc1+nc2])
{
  int cols = nc1+nc2;

  for(int i=0; i < nr; ++i)
  {
    for(int j=0; j < cols; ++j)
    {
      if(j < nc1)
	res[i][j] = m1[i][j];
      else
	res[i][j] = m2[i][j-nc1];
    }
  }
}



void matrix_concatr(int nr1,int nc, const double m1[restrict nr1][nc],int nr2, const double m2[restrict nr2][nc],double res[restrict nr1+nr2][nc])
{
  for(int i=0; i < nr1; ++i)
    for(int j=0; j < nc; ++j)
      res[i][j] = m1[i][j];

  for(int i=0; i < nr2; ++i)
    for(int j=0; j < nc; ++j)
      res[i+nr1][j] = m2[i][j];
}



int matrix_isequal(int nr, int nc, const double m1[nr][nc], const double m2[nr][nc])
{
  for(int i=0; i < nr; ++i)
    for(int j=0; j < nc; ++j)
      if(m1[i][j] != m2[i][j])
	return 0;



  return 1;
}



void matrix_add(int nr,int nc, const double m1[nr][nc],const  double m2[nr][nc], double res[restrict nr][nc])
{
  for(int i=0; i < nr; ++i)
    for(int j=0; j < nc; ++j)
      res[i][j] = m1[i][j] + m2[i][j];
}




void matrix_sub(int nr,int nc, const double m1[nr][nc],const  double m2[nr][nc], double res[restrict nr][nc])
{
  for(int i=0; i < nr; ++i)
    for(int j=0; j < nc; ++j)
      res[i][j] = m1[i][j] - m2[i][j];
}


void vector_proj(int nr, int nc, const double m1[nr][nc], const double m2[nr][nc], double res[nr][nc])
{
  double m1dotm2, m2dotm2, c;
  if(nr != 1 && nc != 1)
  {
    errno = EINVAL;
    return;
  }

  //calculate the dot product m1·m2
  m1dotm2 = vector_dotproduct(nr,nc,m1,m2);
  //calculate the dot product m2·m2
  m2dotm2 = vector_dotproduct(nr,nc,m2,m2);

  //c is the component of m1 along m2
  c = m1dotm2/m2dotm2;
  
  //if row vector
  if(nr == 1)
  {
    for(int i=0; i < nc; ++i)
      res[0][i] = c * m2[0][i];
  }
  // if column vector
  else
  {
    for(int i=0; i < nr; ++i)
      res[i][0] = c * m2[i][0];
  }
}

double vector_sqnorm(int nr, int nc, const double matrix[nr][nc])
{
  return vector_dotproduct(nr,nc,matrix,matrix);
}
