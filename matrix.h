#ifndef _MATRIX_H
#define _MATRIX_H

#include <sys/types.h>

typedef struct
{
	double** data;
	size_t rows;
	size_t columns;
} Matrix;

// initialize a 2d matrix of rows x columns
Matrix matrix_init(size_t rows, size_t columns);

// create a an n x n indentity matrix
Matrix matrix_identity(size_t n);

// create a 2d matrix which is the transpose of matrix
Matrix matrix_transpose(Matrix* matrix);

// find the dot product of two matrices
Matrix matrix_dot_matrix(Matrix* mat1, Matrix* mat2);

// find the dot product of a matrix and a scalar
Matrix matrix_dot_scalar(Matrix* matrix, double scalar);

// get the sum of two matrices
Matrix matrix_plus_matrix(Matrix* mat1, Matrix* mat2);

// get the difference of two matrices
Matrix matrix_minus_matrix(Matrix* mat1, Matrix* mat2);

// get the sum of a matrix and a scalar value
Matrix matrix_plus_scalar(Matrix* matrix, double scalar);

// find the matrix identical to the supplied matrix, but missing the supplied row and column. ZERO BASED INDEX
Matrix matrix_minor(Matrix* matrix, size_t row, size_t column);

// find the determinant of an arbitrarily sized square matrix with laplace expansion
double matrix_determinant(Matrix* matrix);

// calculate the cofactor of the supplied matrix
Matrix matrix_cofactor(Matrix* matrix);

// calculate the transpose of the cofactor of the supplied matrix
Matrix matrix_adjoint(Matrix* matrix);

// get the inverse of a matrix
Matrix matrix_inverse(Matrix* matrix);

// free the data contained within matrix, as well as matrix itself
void matrix_free(Matrix* matrix);


// ================
//  IMPLEMENTATION
// ================


#ifdef MATRIX_IMPLEMENTATION

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

Matrix matrix_init(size_t rows, size_t columns) {
    Matrix ret = {
        .data = (double**)malloc(rows * sizeof(double*)),
        .rows = rows,
        .columns = columns
    };

    // initialize the columns
    for(size_t i = 0; i < rows; i++) {
        ret.data[i] = (double*)malloc(columns * sizeof(double));
    }

    // make all entries equal to 0
    for(size_t i = 0; i < rows; i++) {
        for(size_t j = 0; j < columns; j++) {
            ret.data[i][j] = 0;
        }
    }

    return ret;
}

Matrix matrix_identity(size_t n) {
    Matrix ret = matrix_init(n, n);
    for(size_t i = 0; i < ret.rows; i++) {
        ret.data[i][i] = 1;
    }
    return ret;
}

Matrix matrix_transpose(Matrix* matrix) {
    Matrix ret = matrix_init(matrix->columns, matrix->rows);

    for(size_t i = 0; i < matrix->rows; i++) {
        for(size_t j = 0; j < matrix->columns; j++) {
            ret.data[j][i] = matrix->data[i][j];
        }
    }

    return ret;
}

Matrix matrix_dot_matrix(Matrix* mat1, Matrix* mat2) {
    // [m x n] * [n x p] = [m x p]. Will fail if n1 != n2.
    // Basically, because of math, the amount of columns in the left matrix must be equal to the amount of rows in the right matrix
    assert(mat1->columns == mat2->rows);

    Matrix ret = matrix_init(mat1->rows, mat2->columns);

    for(size_t i = 0; i < mat1->rows; i++) {
        for(size_t j = 0; j < mat2->columns; j++) {
            double result = 0;
            for(size_t k = 0; k < mat1->columns /* mat2->rows */; k++) {
                result += mat1->data[i][k] * mat2->data[k][j];
            }
            ret.data[i][j] = result;
        }
    }

    return ret;
}

Matrix matrix_dot_scalar(Matrix* matrix, double scalar) {
    Matrix ret = matrix_init(matrix->rows, matrix->columns);
    for(size_t i = 0; i < matrix->rows; i++) {
        for(size_t j = 0; j < matrix->columns; j++)
        {
            ret.data[i][j] = matrix->data[i][j] * scalar;
        }
    }

    return ret;
}

Matrix matrix_plus_matrix(Matrix* mat1, Matrix* mat2) {
    // matrices must be of equal dimensions
    assert(mat1->rows == mat2->rows && mat1->columns == mat2->columns);
    
    Matrix ret = matrix_init(mat1->rows, mat1->columns);

    for(size_t i = 0; i < mat1->rows; i++) {
        for(size_t j = 0; j < mat2->columns; j++) {
            ret.data[i][j] = mat1->data[i][j] + mat2->data[i][j];
        }
    }

    return ret;
}

Matrix matrix_minus_matrix(Matrix* mat1, Matrix* mat2) {
    Matrix correctedMat2 = matrix_dot_scalar(mat2, -1);
    Matrix ret = matrix_plus_matrix(mat1, &correctedMat2);
    matrix_free(&correctedMat2);
    return ret;
}

Matrix matrix_plus_scalar(Matrix* matrix, double scalar) {
    Matrix ret = matrix_init(matrix->rows, matrix->columns);

    for(size_t i = 0; i < ret.rows; i++) {
        for(size_t j = 0; j < ret.columns; j++) {
            ret.data[i][j] = matrix->data[i][j] + scalar;
        }
    }

    return ret;
}

Matrix matrix_minor(Matrix* matrix, size_t row, size_t column) {
    Matrix ret = matrix_init(matrix->rows-1, matrix->columns-1);

    int hit_removed_row = 0;
    for(size_t i = 0; i < matrix->rows; i++) {
        int hit_removed_column = 0;
        if(i != row) {
            for(size_t j = 0; j < matrix->columns; j++) {
                if(j != column) {
                    ret.data[i-hit_removed_row][j-hit_removed_column] = matrix->data[i][j];   
                }
                else hit_removed_column = 1;
            }
        } else {
            hit_removed_row = 1;
        }
    }

    return ret;
}

// compute the determinant of a square matrix with laplace expansion
double matrix_determinant(Matrix* matrix) {
    // this probably doesn't make much sense on it's own, and I would be writing
    // more comments than code if I tried to explain it. I suggest reading the
    // following for a good understanding
    // https://en.wikipedia.org/wiki/Laplace_expansion

    // this only works with square matrices
    assert(matrix->rows == matrix->columns);

    double ret = 0;

    if(matrix->rows > 2) {
        for(size_t i = 0; i < matrix->columns; i++) {
            Matrix temp_determ = matrix_minor(matrix, 0, i);
            ret += pow(-1, i) * matrix->data[0][i] * matrix_determinant(&temp_determ);
            matrix_free(&temp_determ);
        }
    } else if(matrix->rows == 2) {
        ret = matrix->data[0][0] * matrix->data[1][1] - matrix->data[0][1] * matrix->data[1][0];
    } else {
        ret = matrix->data[0][0];
    }

    printf("ret in matrix_determinant is %f\n", ret);
    return ret;
}

// calculate the cofactor matrix of the supplied matrix
Matrix matrix_cofactor(Matrix* matrix) {
    Matrix ret = matrix_init(matrix->rows, matrix->columns);
        
    for(size_t i = 0; i < matrix->rows; i++) {
        for(size_t j = 0; j < matrix->columns; j++) {
            Matrix temp_minor = matrix_minor(matrix, i, j);
            ret.data[i][j] = matrix_determinant(&temp_minor);
            matrix_free(&temp_minor);
        }
    }

    return ret;
}

// calculate the transpose of the cofactor of the supplied matrix
Matrix matrix_adjoint(Matrix* matrix) {
    Matrix temp = matrix_cofactor(matrix);
    Matrix ret = matrix_transpose(&temp);
    matrix_free(&temp);
    return ret;
}

// get the inverse of a matrix
Matrix matrix_inverse(Matrix* matrix) {
    // FIXME: this function is not finished

    // this only works with square matrices
    assert(matrix->rows == matrix->columns);

    // inverse = 1/determinant * adjoint
    Matrix adjoint = matrix_adjoint(matrix);
    Matrix ret = matrix_dot_scalar(&adjoint, (1/matrix_determinant(matrix)));
    matrix_free(&adjoint);
 
    return ret;
}

void matrix_free(Matrix* matrix) {
    for(size_t i = 0; i < matrix->rows; i++) {
        free(matrix->data[i]);
    }
    free(matrix->data);
}


#endif

#endif
