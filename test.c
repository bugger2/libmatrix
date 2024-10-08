#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MATRIX_IMPLEMENTATION
#include "matrix.h"

// used as EXAMPLE_MATRIX(matrix)
// and you will now have a matrix named `matrix` with contents of the following:
// [[0, 1],
//  [2, 3],
//  [4, 5]]
void matrix_print(Matrix* matrix);
void test_init();
void test_identity();
void test_transpose();
void test_dot_matrix();
void test_dot_scalar();
void test_plus_matrix();
void test_minus_matrix();
void test_plus_scalar();
void test_minor();
void test_determinant();
void test_cofactor();
void test_adjugate();
void test_inverse();

int main(void)
{
    test_init();
    test_identity();
    test_transpose();
    test_dot_matrix();
    test_dot_scalar();
    test_plus_matrix();
    test_minus_matrix();
    test_plus_scalar();
    test_minor();
    test_determinant();
    test_cofactor();
    test_adjugate();
    test_inverse();

	return 0;
}

Matrix example_matrix() {
    Matrix ret = matrix_init(3, 2);
    int count = 0;
    for (int i = 0; i < ret.rows; i++) {
        for (int j = 0; j < ret.columns; j++) {
            ret.data[i][j] = count++;
        }
    }

    return ret;
}

void matrix_print(Matrix* matrix)
{
	for(size_t i = 0; i < matrix->rows; i++)
	{
		for(size_t j = 0; j < matrix->columns; j++)
		{
			printf("%f ", matrix->data[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void test_init() {
    printf("Testing test_init... ");

    Matrix matrix = matrix_init(3, 2);

    assert(matrix.rows == 3);
    assert(matrix.columns == 2);

    for (size_t i = 0; i < matrix.rows; i++) {
        for (size_t j = 0; j < matrix.columns; j++) {
            assert(matrix.data[i][j] == 0);
        }
    }

    matrix_free(&matrix);

    printf("done.\n");
}

void test_identity() {
    printf("Testing test_identity... ");

    Matrix matrix = matrix_identity(3);

    assert(matrix.rows == 3);
    assert(matrix.columns == 3);

    for (size_t i = 0; i < matrix.rows; i++) {
        for (size_t j = 0; j < matrix.columns; j++) {
            if (i == j) {
                assert(matrix.data[i][j] == 1);
            } else {
                assert(matrix.data[i][j] == 0);
            }
        }
    }

    matrix_free(&matrix);

    printf("done.\n");
}

void test_transpose() {
    printf("Testing test_transpose... ");

    // creates the following matrix:
    // [[ 0, 1 ],
    //  [ 2, 3 ],
    //  [ 4, 5 ]]
    Matrix matrix = matrix_init(3, 2);
    size_t count = 0;
    for (size_t i = 0; i < matrix.rows; i++) {
        for (size_t j = 0; j < matrix.columns; j++) {
            matrix.data[i][j] = count;
            count++;
        }
    }

    // should produce the following matrix:
    // [[ 0, 2, 4 ],
    //  [ 1, 3, 5 ]]
    Matrix transpose = matrix_transpose(&matrix);
    assert(transpose.rows == matrix.columns);
    assert(transpose.columns == matrix.rows);
    for (size_t i = 0; i < transpose.rows; i++) {
        for (size_t j = 0; j < transpose.columns; j++) {
            assert(transpose.data[i][j] == i + (j*2));
        }
    }

    matrix_free(&matrix);
    matrix_free(&transpose);

    printf("done.\n");
}

void test_dot_matrix() {
    printf("Testing test_dot_matrix... ");
    
    Matrix matrix = example_matrix();
    Matrix other_matrix = example_matrix();
    Matrix transposed = matrix_transpose(&other_matrix);
    matrix_free(&other_matrix);

    // should be this
    // [[1,  3,  5],
    //  [3, 13, 23],
    //  [5, 23, 41]]
    Matrix result = matrix_dot_matrix(&matrix, &transposed);

    /* printf("Matrix:\n"); */
    /* matrix_print(&matrix); */
    /* printf("\n"); */

    /* printf("Transposed:\n"); */
    /* matrix_print(&transposed); */
    /* printf("\n"); */

    /* printf("Result:\n"); */
    /* matrix_print(&result); */
    /* printf("\n"); */
    assert(result.rows == 3);
    assert(result.columns == 3);

    assert(result.data[0][0] == 1);
    assert(result.data[0][1] == 3);
    assert(result.data[0][2] == 5);
    assert(result.data[1][0] == 3);
    assert(result.data[1][1] == 13);
    assert(result.data[1][2] == 23);
    assert(result.data[2][0] == 5);
    assert(result.data[2][1] == 23);
    assert(result.data[2][2] == 41);

    matrix_free(&matrix);
    matrix_free(&transposed);
    matrix_free(&result);

    printf("done.\n");
}

void test_dot_scalar() {
    printf("Testing test_dot_scalar... ");

    Matrix matrix = example_matrix();
    Matrix result = matrix_dot_scalar(&matrix, 2);

    /* printf("Matrix:\n"); */
    /* matrix_print(&matrix); */
    /* printf("\n"); */

    /* printf("Result:\n"); */
    /* matrix_print(&result); */
    /* printf("\n"); */

    assert(result.rows == matrix.rows);
    assert(result.columns == matrix.columns);

    int count = 0;
    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.columns; j++) {
            assert(result.data[i][j] == count * 2);
            count++;
        }
    }

    matrix_free(&matrix);
    matrix_free(&result);

    printf("done.\n");
}

void test_plus_matrix() {
    printf("Testing test_plus_matrix... ");

    Matrix matrix = example_matrix();
    Matrix negative = matrix_dot_scalar(&matrix, -1);

    Matrix result = matrix_plus_matrix(&matrix, &negative);

    assert(result.rows == matrix.rows);
    assert(result.columns == matrix.columns);

    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.columns; j++) {
            assert(result.data[i][j] == 0);
        }
    }

    matrix_free(&matrix);
    matrix_free(&negative);
    matrix_free(&result);

    printf("done.\n");
}

void test_minus_matrix() {
    printf("Testing test_minus_matrix... ");

    Matrix matrix = example_matrix();
    Matrix other = example_matrix();

    Matrix result = matrix_minus_matrix(&matrix, &other);

    assert(result.rows == matrix.rows);
    assert(result.columns == matrix.columns);

    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.columns; j++) {
            assert(result.data[i][j] == 0);
        }
    }

    matrix_free(&matrix);
    matrix_free(&other);
    matrix_free(&result);

    printf("done.\n");
}

void test_plus_scalar() {
    printf("Testing test_plus_scalar... ");

    Matrix matrix = example_matrix();
    Matrix result = matrix_plus_scalar(&matrix, 1);

    assert(result.rows == matrix.rows);
    assert(result.columns == matrix.columns);

    int count = 1;
    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.columns; j++) {
            assert(result.data[i][j] == count++);
        }
    }

    matrix_free(&matrix);
    matrix_free(&result);

    printf("done.\n");
}

void test_minor() {
    printf("testing matrix_minor... ");

    Matrix matrix = example_matrix();

    // should result in
    // [[0],
    //  [4]]
    Matrix result = matrix_minor(&matrix, 1, 1);

    assert(result.rows = matrix.rows - 1);
    assert(result.columns = matrix.columns - 1);

    assert(result.data[0][0] == 0);
    assert(result.data[1][0] == 4);

    matrix_free(&matrix);
    matrix_free(&result);

    printf("done.\n");
}

void test_determinant() {
    printf("testing determinant... ");

    Matrix matrix = matrix_init(3, 3);
    double data[3][3] = {
        {1 , 0, 2},
        {3 , 1, 0},
        {-1, 1, 4}
    };
    for (int i = 0; i < matrix.rows; i++) {
        memcpy(matrix.data[i], data[i], 3 * sizeof(double));
    }

    double determinant = matrix_determinant(&matrix);
    assert(determinant == 12);

    matrix_free(&matrix);

    printf("done.\n");
}

void test_cofactor() {
    printf("testing cofactor... ");

    Matrix matrix = matrix_init(3, 3);
    double data[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    for (int i = 0; i < matrix.rows; i++) {
        memcpy(matrix.data[i], data[i], 3 * sizeof(double));
    }

    Matrix result = matrix_cofactor(&matrix);

    double correct[3][3] = {
        {-3, 6, -3},
        {6, -12, 6},
        {-3, 6, -3}
    };

    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.columns; j++) {
            assert(result.data[i][j] == correct[i][j]);
        }
    }

    matrix_free(&matrix);
    matrix_free(&result);

    printf("done.\n");
}

void test_adjugate() {
    printf("testing adjugate... ");

    Matrix matrix = matrix_init(3, 3);
    double data[3][3] = {
        {1, 2, 3},
        {0, 4, 5},
        {1, 0, 6}
    };
    for (int i = 0; i < matrix.rows; i++) {
        memcpy(matrix.data[i], data[i], 3 * sizeof(double));
    }

    Matrix adjugate = matrix_adjugate(&matrix);
    double correct[3][3] = {
        {24, -12, -2},
        {5 ,  3 , -5},
        {-4,  2 ,  4}
    };

    for (int i = 0; i < adjugate.rows; i++) {
        for (int j = 0; j < adjugate.columns; j++) {
            assert(adjugate.data[i][j] == correct[i][j]);
        }
    }

    matrix_free(&adjugate);
    matrix_free(&matrix);

    printf("done.\n");
}

void test_inverse() {
    printf("testing inverse... ");

    Matrix matrix = matrix_init(3, 3);
    double data[3][3] = {
        {3, 0,  2},
        {2, 0, -2},
        {0, 1,  1}
    };
    for (int i = 0; i < matrix.rows; i++) {
        memcpy(matrix.data[i], data[i], 3 * sizeof(double));
    }

    Matrix inverse = matrix_inverse(&matrix);
    Matrix identity = matrix_identity(3);

    // a matrix dotted with its inverse should result in the identity matrix
    Matrix maybe_identity = matrix_dot_matrix(&matrix, &inverse);

    for (int i = 0; i < inverse.rows; i++) {
        for (int j = 0; j < inverse.columns; j++) {
            assert(maybe_identity.data[i][j] == identity.data[i][j]);
        }
    }

    matrix_free(&matrix);
    matrix_free(&inverse);
    matrix_free(&identity);
    matrix_free(&maybe_identity);

    printf("done.\n");
}
