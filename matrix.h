/* MATRIX.H
 *
 * Author: Isonguyo John <isongjohn014@gmail.com>
 * Created on 06/04/2025.
 *
 * Modified by: Isonguyo John
 * Last modified: 06/04/2025.
 *
 *
 * C library of functions for working with 2D matrices.
 */


#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <errno.h>


/*[matrix_isequal]-
 *
 *
 */
int matrix_isequal(int nrows, int ncols, const double matrix_1[nrows][ncols], const double matrix_2[nrows][ncols]);

/*[matrix_concatc]-
 *
 *
 */
void matrix_concatr(int nrows_1,int ncols, const double matrix_1[restrict nrows_1][ncols],int nrows_2, const double matrix_2[restrict nrows_2][ncols],double res[restrict nrows_1+nrows_2][ncols]);

/*[matrix_concatc]-
 *
 *
 */
void matrix_concatc(int nrows, int ncols_1, const double matrix_1[restrict nrows][ncols_1], int ncols_2,const double matrix_2[restrict nrows][ncols_2], double res[restrict nrows][ncols_1+ncols_2]);

/*[matrix_crossproduct]-
 *
 *
 */
void matrix_crossproduct(int nrows,int ncols, const double matrix_1[nrows][ncols],const double matrix_2[nrows][ncols], double result[restrict nrows][ncols]);

/*[matrix_dotproduct]-
 *
 *
 *
 */
double matrix_dotproduct(int nrows,int ncols,const double matrix_1[nrows][ncols], const  double matrix_2[nrows][ncols]);

/*[matrix_inverse]-
 *
 *
 *
 */
void matrix_inverse(int order, const double matrix[restrict order][order], double inverse[restrict order][order]);

/*[matrix_adjoint]-
 *
 *
 *
 */
void matrix_adjoint(int order,const double matrix[restrict order][order], double adjoint[restrict order][order]);

/*[matrix_scalarm]-
 *
 *
 *
 */
void matrix_scalarm(int nrows,int ncols, const double matrix[restrict nrows][ncols],double result[restrict nrows][ncols],double scalar);

/*[matrix_multiply]-
 *
 *
 *
 */
void matrix_multiply(int nrows_1,int ncols_1,double const matrix_1[restrict nrows_1][ncols_1],int ncols_2, double const matrix_2[restrict ncols_1][ncols_2], double result[restrict nrows_1][ncols_2]);	

/*[matrix_transpose]-
 *
 *
 *
 */
void matrix_transpose(int nrows, int ncols,double const matrix[restrict nrows][ncols], double transpose[restrict ncols][nrows]);

/*[matrix_determinant]-
 *
 *
 *
 */
double matrix_determinant(int order, double const matrix[order][order]);

/*[matrix_minor]-
 *
 *
 *
 */
void matrix_minor(int order, const double matrix[restrict order][order],double minor[restrict order-1][order-1], int row, int col);

/*[matrix_print]-
 *
 *
 *
 */
void matrix_print(char *tag,int nrows, int ncols,const double matrix[nrows][ncols]);

/*[matrix_add]-
 *
 *
 *
 */
void matrix_add(int nrows,int ncols, const double matrix_1[nrows][ncols],const  double matrix_2[nrows][ncols], double result[restrict nrows][ncols]);

/*[matrix_sub]-
 *
 *
 *
 */
void matrix_sub(int nrows,int ncols, const double matrix_1[nrows][ncols],const  double matrix_2[nrows][ncols], double result[restrict nrows][ncols]);


#endif
