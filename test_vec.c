#include <stdio.h>
#include <math.h>
#include "src/vec.h"

// --- Test Framework ---

int tests_run = 0;
int tests_failed = 0;

#define ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("    [FAIL] %s (at %s:%d)\n", message, __FILE__, __LINE__); \
            return 1; \
        } \
    } while (0)

typedef int (*test_func)();

void run_test(const char* name, test_func func) {
    tests_run++;
    printf("Running %s... ", name);
    if (func() == 0) {
        printf("[PASS]\n");
    } else {
        tests_failed++;
        // Failure message is printed by the ASSERT macro inside the function
    }
}

// --- Helper Functions ---

// Helper to compare floats with epsilon
int float_eq(float a, float b) {
    return fabsf(a - b) < 0.0001f;
}

// --- Test Cases ---

int test_lifecycle() {
    float data[] = {1.0f, 2.0f, 3.0f};
    Vector* v = NULL;

    // Test successful creation
    int status = vec_create(data, 3, &v);
    ASSERT(status == 0, "vec_create should return 0");
    ASSERT(v != NULL, "Vector pointer should not be NULL");
    ASSERT(v->length == 3, "Vector length should be 3");
    ASSERT(float_eq(v->data[0], 1.0f), "Data[0] should be 1.0");

    // Test vec_free
    status = vec_free(&v);
    ASSERT(status == 0, "vec_free should return 0");
    ASSERT(v == NULL, "Vector pointer should be NULL after free");

    // Test NULL inputs to lifecycle
    ASSERT(vec_create(NULL, 3, &v) == -1, "vec_create with NULL data should fail");
    ASSERT(vec_create(data, 0, &v) == -1, "vec_create with 0 length should fail");
    ASSERT(vec_free(NULL) == -1, "vec_free with NULL pointer should fail");
    ASSERT(vec_free(&v) == 0, "vec_free with NULL vector pointer should be no-op");

    return 0;
}

int test_vec_add() {
    float d1[] = {1.0f, 2.0f, 3.0f};
    float d2[] = {4.0f, 5.0f, 6.0f};
    Vector *v1, *v2, *res;

    vec_create(d1, 3, &v1);
    vec_create(d2, 3, &v2);

    res = vec_add(v1, v2);
    ASSERT(res != NULL, "vec_add result should not be NULL");
    ASSERT(res->length == 3, "Result length should be 3");
    ASSERT(float_eq(res->data[0], 5.0f), "1+4 should be 5");
    ASSERT(float_eq(res->data[1], 7.0f), "2+5 should be 7");
    ASSERT(float_eq(res->data[2], 9.0f), "3+6 should be 9");

    // Edge cases
    ASSERT(vec_add(v1, NULL) == NULL, "vec_add with NULL should return NULL");
    
    // Mismatched length
    float d3[] = {1.0f};
    Vector *v3;
    vec_create(d3, 1, &v3);
    ASSERT(vec_add(v1, v3) == NULL, "vec_add with mismatched lengths should return NULL");

    vec_free(&v1);
    vec_free(&v2);
    vec_free(&v3);
    vec_free(&res);
    return 0;
}

int test_vec_sub() {
    float d1[] = {10.0f, 20.0f};
    float d2[] = {3.0f, 5.0f};
    Vector *v1, *v2, *res;

    vec_create(d1, 2, &v1);
    vec_create(d2, 2, &v2);

    res = vec_sub(v1, v2);
    ASSERT(res != NULL, "vec_sub result should not be NULL");
    ASSERT(float_eq(res->data[0], 7.0f), "10-3 should be 7");
    ASSERT(float_eq(res->data[1], 15.0f), "20-5 should be 15");

    vec_free(&v1);
    vec_free(&v2);
    vec_free(&res);
    return 0;
}

int test_vec_mul() {
    float d1[] = {2.0f, 3.0f};
    float d2[] = {4.0f, 5.0f};
    Vector *v1, *v2, *res;

    vec_create(d1, 2, &v1);
    vec_create(d2, 2, &v2);

    res = vec_mul(v1, v2);
    ASSERT(res != NULL, "vec_mul result should not be NULL");
    ASSERT(float_eq(res->data[0], 8.0f), "2*4 should be 8");
    ASSERT(float_eq(res->data[1], 15.0f), "3*5 should be 15");

    vec_free(&v1);
    vec_free(&v2);
    vec_free(&res);
    return 0;
}

int test_vec_div() {
    float d1[] = {10.0f, 20.0f};
    float d2[] = {2.0f, 5.0f};
    float d3[] = {1.0f, 0.0f}; // Contains zero
    Vector *v1, *v2, *v3, *res1, *res2;

    vec_create(d1, 2, &v1);
    vec_create(d2, 2, &v2);
    vec_create(d3, 2, &v3);

    // Happy path
    res1 = vec_div(v1, v2);
    ASSERT(res1 != NULL, "vec_div should succeed");
    ASSERT(float_eq(res1->data[0], 5.0f), "10/2 should be 5");
    ASSERT(float_eq(res1->data[1], 4.0f), "20/5 should be 4");

    // Division by zero
    res2 = vec_div(v1, v3);
    ASSERT(res2 == NULL, "vec_div should return NULL on division by zero");

    // Division by epsilon
    float d4[] = {0.0000001f, 1.0f};
    Vector *v4;
    vec_create(d4, 2, &v4);
    res2 = vec_div(v1, v4);
    ASSERT(res2 == NULL, "vec_div should return NULL on division by epsilon");

    vec_free(&v1);
    vec_free(&v2);
    vec_free(&v3);
    vec_free(&v4);
    vec_free(&res1);
    vec_free(&res2);
    return 0;
}

int test_vec_dot() {
    float d1[] = {1.0f, 2.0f, 3.0f};
    float d2[] = {4.0f, 5.0f, 6.0f};
    Vector *v1, *v2;
    float res_val = 0.0f;

    vec_create(d1, 3, &v1);
    vec_create(d2, 3, &v2);

    // (1*4) + (2*5) + (3*6) = 4 + 10 + 18 = 32
    int status = vec_dot(v1, v2, &res_val);
    ASSERT(status == 0, "vec_dot should return 0");
    ASSERT(float_eq(res_val, 32.0f), "Dot product should be 32");

    // Edge case: NULL result pointer
    ASSERT(vec_dot(v1, v2, NULL) == -1, "vec_dot with NULL result pointer should fail");

    // Edge case: Mismatched lengths
    float d3[] = {1.0f, 2.0f};
    Vector *v3;
    vec_create(d3, 2, &v3);
    ASSERT(vec_dot(v1, v3, &res_val) == -1, "vec_dot with mismatching lengths should fail");

    vec_free(&v1);
    vec_free(&v2);
    vec_free(&v3);
    return 0;
}

// --- Main Runner ---

int main() {
    printf("=== VECTOR LIBRARY TEST SUITE ===\n");

    run_test("vec_lifecycle", test_lifecycle);
    run_test("vec_add", test_vec_add);
    run_test("vec_sub", test_vec_sub);
    run_test("vec_mul", test_vec_mul);
    run_test("vec_div", test_vec_div);
    run_test("vec_dot", test_vec_dot);

    printf("\n=================================\n");
    printf("Tests Run:    %d\n", tests_run);
    printf("Tests Passed: %d\n", tests_run - tests_failed);
    printf("Tests Failed: %d\n", tests_failed);
    printf("=================================\n");

    return (tests_failed == 0) ? 0 : 1;
}
