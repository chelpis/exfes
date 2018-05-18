// fes.h provides
//
// macro definitions:
//      SIMD_CHUNK_SIZE
//      min
//      unlikely
//
// types:
//      vector_t
//      matrix_t
//      pck_vector_t
//      quadratic_form
//      wrapper_state_t
//      system_t
//
// function declarations:
//      free_vector
//      init_vector
//      pack
//      to_gray
//      packed_eval
//      variables_specialization
//      generate_random_system
//      free_system
//      exhaustive_ia32_deg_2
//      packed_eval_deg_2

#ifndef FES_H
#define FES_H

#include <stdint.h>  // uint32_t, uint64_t

#include "fes_interface.h"  // solution_callback_t
#include "idx_LUT.h"  // idx_lut_t, LUT_t

#define SIMD_CHUNK_SIZE 9
#define min(x, y) (((x) > (y)) ? (y) : (x))
#define unlikely(x) __builtin_expect(!!(x), 0)

typedef int *vector_t;
typedef vector_t *matrix_t;
typedef uint32_t pck_vector_t;

typedef struct {
    matrix_t quad;
    vector_t lin;
    int con;
} quadratic_form;

typedef quadratic_form *system_t;

typedef struct {
    int n;
    int degree;
    int n_batches;
    pck_vector_t **G;
    idx_lut_t *testing_LUT;
    solution_callback_t callback;
    void *callback_state;
} wrapper_state_t;

void free_vector(vector_t x);
vector_t init_vector(int n_rows);
pck_vector_t pack(int n, const vector_t v);
uint64_t to_gray(uint64_t i);
pck_vector_t packed_eval(LUT_t LUT, int n, int d, pck_vector_t *F, uint64_t i);
void variables_specialization(LUT_t LUT, int n, int d, pck_vector_t *A, int k, int i);
system_t generate_random_system(int n, int u);
void free_system(int n, int n_eqs, system_t f);
void exhaustive_ia32_deg_2(LUT_t LUT, int n, pck_vector_t *F, solution_callback_t callback, void *callback_state, int verbose);
pck_vector_t packed_eval_deg_2(LUT_t LUT, int n, pck_vector_t *F, uint64_t i);

#endif
