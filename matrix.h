#ifndef _MATRIX_H
#define _MATRIX_H

#include <sys/types.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	double** array;
	size_t rows;
	size_t columns;
} Matrix;

/* Definition */

// initialize a 2d matrix of rows x columns
Matrix matrix_init(size_t rows, size_t columns);

// create a 2d matrix which is the transpose of matrix
Matrix matrix_transpose(Matrix* matrix);

// find the dot product of two matrices
Matrix matrix_times_matrix(Matrix* mat1, Matrix* mat2);

// find the dot product of a matrix and a scalar
Matrix matrix_times_scalar(Matrix* matrix, double scalar);

// get the sum of two matrices
Matrix matrix_plus_matrix(Matrix* mat1, Matrix* mat2);

// get the sum of a matrix and a scalar value
Matrix matrix_plus_scalar(Matrix* matrix, double scalar);

// find the matrix identical to the supplied matrix, but missing the supplied row and column
Matrix matrix_minor(Matrix* matrix, size_t row, size_t column);

// find the determinant of an arbitrarily sized square matrix with laplace expansion
double matrix_determinant(Matrix* matrix);

// calculate the cofactor of the supplied matrix
Matrix matrix_cofactor(Matrix* matrix);

// calculate the transpose of the cofactor of the supplied matrix
Matrix matrix_adjoint(Matrix* matrix);

// get the inverse of a matrix
Matrix matrix_inverse(Matrix* matrix);

// returns the inverse of a matrix
Matrix matrix_inverse(Matrix* matrix);

// free the data contained within matrix, as well as matrix itself
void matrix_free(Matrix* matrix);

/* Implementation */

inline Matrix matrix_init(size_t rows, size_t columns)
{
	Matrix ret = {
		.array = (double**)malloc(rows * sizeof(double*)),
		.rows = rows,
		.columns = columns
	};

	// initialize the columns
	for(size_t i = 0; i < rows; i++)
	{
		ret.array[i] = (double*)malloc(columns * sizeof(double));
	}

	// make all entries equal to 0
	for(size_t i = 0; i < rows; i++)
	{
		for(size_t j = 0; i < columns; j++)
			ret.array[i][j] = 0;
	}

	return ret;
}

inline Matrix matrix_transpose(Matrix* matrix)
{
	Matrix ret = matrix_init(matrix->columns, matrix->rows);

	for(size_t i = 0; i < matrix->rows; i++)
	{
		for(size_t j = 0; j < matrix->columns; j++)
			ret.array[j][i] = matrix->array[i][j];
	}

	return ret;
}

inline Matrix matrix_times_matrix(Matrix* mat1, Matrix* mat2)
{
	// [m x n] * [n x p] = [m x p]. Will fail if n1 != n2.
	// Basically, because of math, the amount of columns in the left matrix must be equal to the amount of rows in the right matrix
	assert(mat1->columns == mat2->rows);

	Matrix ret = matrix_init(mat1->rows, mat2->columns);

	for(size_t i = 0; i < mat1->rows; i++)
	{
		for(size_t j = 0; j < mat2->columns; j++)
		{
			double result = 0;
			for(size_t k = 0; k < mat1->columns /* mat2->rows */; k++)
			{
				result += mat1->array[i][k] * mat2->array[k][j];
			}
			ret.array[i][j] = result;
		}
	}

	return ret;
}

inline Matrix matrix_multiply_scalar(Matrix* matrix, double scalar)
{
	Matrix ret = matrix_init(matrix->rows, matrix->columns);
	for(size_t i = 0; i < matrix->rows; i++)
	{
		for(size_t j = 0; j < matrix->columns; j++)
		{
			ret.array[i][j] *= scalar;
		}
	}

	return ret;
}

inline Matrix matrix_plus_matrix(Matrix* mat1, Matrix* mat2)
{
	// matrices must be of equal dimensions
	assert(mat1->rows == mat2->rows && mat1->columns == mat2->columns);
	
	Matrix ret = matrix_init(mat1->rows, mat1->columns);

	for(size_t i = 0; i < mat1->rows; i++)
	{
		for(size_t j = 0; j < mat2->columns; j++)
		{
			ret.array[i][j] = mat1->array[i][j] + mat2->array[i][j];
		}
	}

	return ret;
}

inline Matrix matrix_plus_scalar(Matrix* matrix, double scalar)
{
	Matrix ret = matrix_init(matrix->rows, matrix->columns);

	for(size_t i = 0; i < matrix->rows; i++)
	{
		for(size_t j = 0; j < matrix->columns; j++)
		{
			matrix->array[i][j] = scalar;
		}
	}

	return ret;
}

inline Matrix matrix_minor(Matrix* matrix, size_t row, size_t column)
{
	Matrix ret = matrix_init(matrix->rows, matrix->columns);

	for(size_t i = 0; i < matrix->rows; i++)
	{
		if(i != row)
		{
			for(size_t j = 0; j < matrix->columns; j++) {
				if(j != column) ret.array[i][j] = matrix->array[i][j];
			}
		}
	}

	return ret;
}

// compute the determinant of a square matrix with laplace expansion
inline double matrix_determinant(Matrix* matrix)
{
	// this probably doesn't make much sense on it's own, and I would be writing
	// more comments than code if I tried to explain it. I suggest reading the
	// following for a good understanding
	// https://en.wikipedia.org/wiki/Laplace_expansion

	// this only works with square matrices
	assert(matrix->rows == matrix->columns);

	double ret = 0;

	if(matrix->rows > 2)
	{
		for(size_t i = 0; i < matrix->columns; i++)
		{
			Matrix temp_determ = matrix_minor(matrix, 0, i);
			ret += powf(-1, i) * matrix->array[0][i] * matrix_determinant(&temp_determ);
			matrix_free(&temp_determ);
		}
	} else {
		ret = matrix->array[0][0] * matrix->array[1][1] - matrix->array[0][1] * matrix->array[1][0];
	}

	return ret;
}

// calculate the cofactor matrix of the supplied matrix
inline Matrix matrix_cofactor(Matrix* matrix)
{
	Matrix ret = matrix_init(matrix->rows, matrix->columns);
		
	for(size_t i = 0; i < matrix->rows; i++)
	{
		for(size_t j = 0; j < matrix->columns; j++)
		{
			Matrix temp_minor = matrix_minor(matrix, i, j);
			ret.array[i][j] = matrix_determinant(&temp_minor);
			matrix_free(&temp_minor);
		}
	}

	return ret;
}

// calculate the transpose of the cofactor of the supplied matrix
inline Matrix matrix_adjoint(Matrix* matrix)
{
	Matrix temp = matrix_cofactor(matrix);
	Matrix ret = matrix_transpose(&temp);
	matrix_free(&temp);
	return ret;
}

// get the inverse of a matrix
inline Matrix matrix_inverse(Matrix* matrix)
{
	// FIXME: this function is not finished

	// this only works with square matrices
	assert(matrix->rows == matrix->columns);

	// inverse = 1/determinant * adjoint
	Matrix adjoint = matrix_adjoint(matrix);
	Matrix ret = matrix_times_scalar(&adjoint, (1/matrix_determinant(matrix)));
	matrix_free(&adjoint);

	return ret;
}

inline void matrix_free(Matrix* matrix)
{
	for(size_t i = 0; i < matrix->rows; i++)
	{
		free(matrix->array[i]);
	}
	free(matrix->array);
	free(matrix);
}

#endif