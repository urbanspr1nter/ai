#ifndef VEC_H
#define VEC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Vector {
    float* data;
    uint32_t length;
} Vector;

int vec_create(float* data, uint32_t length, Vector** result);
int vec_free(Vector** v);

Vector* vec_add(Vector* a, Vector* b);
Vector* vec_sub(Vector* a, Vector* b);
Vector* vec_mul(Vector* a, Vector* b);
Vector* vec_div(Vector* a, Vector* b);

int vec_dot(Vector* a, Vector* b, float* result);

#endif