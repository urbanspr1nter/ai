#ifndef MATRIX_H
#define MATRIX_H

#include "vec.h"

typedef struct Matrix {
    Vector** rows;
    uint32_t num_rows;
    uint32_t num_cols;
} Matrix;

int matrix_create(Vector** rows, uint32_t num_rows, uint32_t num_cols, Matrix** result);
int matrix_free(Matrix** m);

Matrix* matrix_add(Matrix* a, Matrix* b);
Matrix* matrix_sub(Matrix* a, Matrix* b);
Matrix* matrix_mul(Matrix* a, Matrix* b);

#endif