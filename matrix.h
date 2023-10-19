#ifndef _MATRIX_H
#define _MATRIX_H

#include <sys/types.h>

typedef struct
{
	double** array;
	size_t rows;
	size_t columns;
} Matrix;

// initialize a 2d matrix of rows x columns
Matrix matrix_init(size_t rows, size_t columns);

// create a 2d matrix which is the transpose of matrix
Matrix* matrix_transpose(Matrix* matrix);

// find the dot product of two matrices
Matrix* matrix_times_matrix(Matrix* mat1, Matrix* mat2);

// find the dot product of a matrix and a scalar
Matrix* matrix_times_scalar(Matrix* matrix, double scalar);

// get the sum of two matrices
Matrix* matrix_plus_matrix(Matrix* mat1, Matrix* mat2);

// get the sum of a matrix and a scalar value
Matrix* matrix_plus_scalar(Matrix* matrix, double scalar);

// find the matrix identical to the supplied matrix, but missing the supplied row and column
Matrix* matrix_minor(Matrix* matrix, size_t row, size_t column);

// find the determinant of an arbitrarily sized square matrix with laplace expansion
double matrix_determinant(Matrix* matrix);

// calculate the cofactor of the supplied matrix
Matrix* matrix_cofactor(Matrix* matrix);

// calculate the transpose of the cofactor of the supplied matrix
Matrix* matrix_adjoint(Matrix* matrix);

// get the inverse of a matrix
Matrix* matrix_inverse(Matrix* matrix);

// returns the inverse of a matrix
Matrix* matrix_inverse(Matrix* matrix);

// free the data contained within matrix, as well as matrix itself
void matrix_free(Matrix* matrix);

#endif
