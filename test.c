#include <stdio.h>
#include <assert.h>

#define MATRIX_IMPLEMENTATION
#include "matrix.h"

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

int main(void)
{
    test_init();
    test_identity();
    test_transpose();

	return 0;
}
