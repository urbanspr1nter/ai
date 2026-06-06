#include "matrix.h"

int matrix_create(Vector** rows, uint32_t num_rows, uint32_t num_cols, Matrix** result) {
    if (result == NULL) {
        return -1;
    }

    *result = NULL;

    if (rows == NULL || *rows == NULL) {
        return -1;
    }

    if (num_rows == 0 && num_cols > 0) {
        fprintf(stderr, "Can't have columns without any rows. Otherwise use vector instead.\n");
        return -1;
    } 
    
    if (num_rows > 0 && num_cols == 0) {
        fprintf(stderr, "If you want an empty matrix, specify 0 columns too.\n");
        return -1;
    }

    Vector** data = (Vector**)malloc(sizeof(Vector*) * num_rows);
    if (data == NULL) {
        fprintf(stderr, "Couldn't allocate memory for matrix.\n");
        return -1;
    }

    // i is the index to the current row of vecs
    uint32_t i;
    for (i = 0; i < num_rows; i++) {
        if (rows[i] == NULL) {
            fprintf(stderr, "num_rows exceed that of the true number of vectors passed in.\n");

            goto cleanup;
        }

        if (rows[i]->length != num_cols) {
            fprintf(stderr, "There is a mismatch of columns in vector compared to matrix columns.\n");

            goto cleanup;
        }

        int create_result = vec_create(rows[i]->data, rows[i]->length, &data[i]);

        if (create_result == -1) {
            fprintf(stderr, "Couldn't allocate vector to matrix at index %d\n", i);
            
            goto cleanup;
        }
    }

    Matrix *m = (Matrix*)malloc(sizeof(Matrix));
    if (m == NULL) {
        fprintf(stderr, "Couldn't allocate memory for a matrix.\n");

        goto cleanup;
    }

    m->rows = data;
    m->num_rows = num_rows;
    m->num_cols = num_cols;

    *result = m;

    return 0;

cleanup:
    for (uint32_t j = 0; j < i; j++) {
        vec_free(&data[j]);
    }
    free(data);

    return -1;
}