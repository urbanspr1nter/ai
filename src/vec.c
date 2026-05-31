#include "vec.h"

int vec_create(float* data, uint32_t length, Vector** result) {
    if (result == NULL) {
        return -1;
    }

    // Ensure the caller's pointer is NULL if we fail early
    *result = NULL;

    if (data == NULL) {
        return -1;
    }

    if (length == 0) {
        return -1;
    }

    Vector* v = (Vector*)malloc(sizeof(Vector));
    if (v == NULL) {
        fprintf(stderr, "Couldn't allocate memory for vector.\n");
        return -1;
    }

    v->data = (float*)malloc(sizeof(float) * length);
    if (v->data == NULL) {
        // free the allocated Vector v before return -1.
        free(v);

        fprintf(stderr, "Couldn't allocate memory for vector data.\n");
        return -1;
    }

    v->length = length;

    // copy over the floats
    for (uint32_t i = 0; i < v->length; i++) {
        (v->data)[i] = data[i];
    }

    *result = v;

    return 0;
}

int vec_free(Vector** v) {
    if (v == NULL) {
        return -1;
    }

    if (*v == NULL) {
        return 0;
    }

    (*v)->length = 0;

    if ((*v)->data != NULL) {
        free((*v)->data);
    }

    free((*v));
    *v = NULL;

    return 0;
}

Vector* vec_add(Vector* a, Vector* b) {
    if (a == NULL || b == NULL) {
        return NULL;
    }

    if (a->length == 0 || b->length == 0) {
        return NULL;
    }

    if (a->length != b->length) {
        return NULL;
    }

    uint32_t len = a->length;

    float *data = (float*)malloc(sizeof(float) * len);
    if (data == NULL) {
        fprintf(stderr, "Cannot allocate enough memory for vector data.\n");
        return NULL;
    }

    for (uint32_t i = 0; i < len; i++) {
        data[i] = a->data[i] + b->data[i];
    }

    Vector* result;

    int result_code = vec_create(data, len, &result);
    
    // Free data as the vec_create will already have created a copy of it.
    free(data);

    if (result_code == -1) {
        fprintf(stderr, "Cannot create a Vector from add.\n");
        return NULL;
    }



    return result;
}

Vector* vec_sub(Vector* a, Vector* b) {
    if (a == NULL || b == NULL) {
        return NULL;
    }

    if (a->length == 0 || b->length == 0) {
        return NULL;
    }

    if (a->length != b->length) {
        return NULL;
    }

    uint32_t len = a->length;

    float *data = (float*)malloc(sizeof(float) * len);
    if (data == NULL) {
        fprintf(stderr, "Cannot allocate enough memory for vector data.\n");
        return NULL;
    }

    for (uint32_t i = 0; i < len; i++) {
        data[i] = a->data[i] - b->data[i];
    }

    Vector* result;

    int result_code = vec_create(data, len, &result);
    
    // Free data as the vec_create will already have created a copy of it.
    free(data);

    if (result_code == -1) {
        fprintf(stderr, "Cannot create a Vector from subtract.\n");
        return NULL;
    }

    return result;
}

Vector* vec_mul(Vector* a, Vector* b) {
    if (a == NULL || b == NULL) {
        return NULL;
    }

    if (a->length == 0 || b->length == 0) {
        return NULL;
    }

    if (a->length != b->length) {
        return NULL;
    }

    uint32_t len = a->length;

    float *data = (float*)malloc(sizeof(float) * len);
    if (data == NULL) {
        fprintf(stderr, "Cannot allocate enough memory for vector data.\n");
        return NULL;
    }

    for (uint32_t i = 0; i < len; i++) {
        data[i] = a->data[i] * b->data[i];
    }

    Vector* result;

    int result_code = vec_create(data, len, &result);
    
    // Free data as the vec_create will already have created a copy of it.
    free(data);

    if (result_code == -1) {
        fprintf(stderr, "Cannot create a Vector from multiply.\n");
        return NULL;
    }

    return result;
}

Vector* vec_div(Vector* a, Vector* b) {
    if (a == NULL || b == NULL) {
        return NULL;
    }

    if (a->length == 0 || b->length == 0) {
        return NULL;
    }

    if (a->length != b->length) {
        return NULL;
    }

    uint32_t len = a->length;

    float *data = (float*)malloc(sizeof(float) * len);
    if (data == NULL) {
        fprintf(stderr, "Cannot allocate enough memory for vector data.\n");
        return NULL;
    }

    float EPSILON = 0.00001f;

    for (uint32_t i = 0; i < len; i++) {
        if (b->data[i] < EPSILON && b->data[i] > -EPSILON) {
            free(data);

            fprintf(stderr, "A division by zero detected. Exiting early.\n");
            return NULL;
        }

        data[i] = a->data[i] / b->data[i];
    }

    Vector* result;

    int result_code = vec_create(data, len, &result);
    
    // Free data as the vec_create will already have created a copy of it.
    free(data);

    if (result_code == -1) {
        fprintf(stderr, "Cannot create a Vector from divide.\n");
        return NULL;
    }

    return result;
}

int vec_dot(Vector* a, Vector* b, float* result) {
    if (a == NULL || b == NULL) {
        fprintf(stderr, "Can't do a dot product on null vectors.\n");
        return -1;
    }

    if (result == NULL) {
        fprintf(stderr, "Result pointer cannot be NULL.\n");
        return -1;
    }
    
    if (a->length != b->length) {
        fprintf(stderr, "Cannot do a dot product on unequal vector lengths.\n");
        return -1;
    }

    uint32_t len = a->length;

    if (len == 0) {
        *result = 0.0f;
        return 0;
    }
    
    // dot product
    float dot_product = 0.0;
    for (uint32_t i = 0; i < len; i++) {
        dot_product += (a->data[i] * b->data[i]);
    }

    // dot product will be stored here.
    *result = dot_product;

    return 0;
}