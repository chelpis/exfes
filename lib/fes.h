// fes.h provides
//
// macro definitions:
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
//      exhaustive_ia32_deg_2
//      packed_eval_deg_2

#ifndef FES_H_INCLUDED
#define FES_H_INCLUDED

#include <stdint.h>  // uint32_t, uint64_t

#include "exfes_internal.h"  // struct exfes_context
#include "idx_LUT.h"  // idx_lut_t, LUT_t

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
    struct exfes_context *exfes_ctx_ptr;
} wrapper_state_t;

void free_vector(vector_t x);
vector_t init_vector(int n_rows);
pck_vector_t pack(int n, const vector_t v);
uint64_t to_gray(uint64_t i);
pck_vector_t packed_eval(LUT_t LUT, int n, pck_vector_t *F, uint64_t i);
void variables_specialization(LUT_t LUT, int n, int d, pck_vector_t *A, int k, int i);
void exhaustive_ia32_deg_2(LUT_t LUT, int n, pck_vector_t *F, wrapper_state_t *wrapper_state_ptr);
pck_vector_t packed_eval_deg_2(LUT_t LUT, int n, pck_vector_t *F, uint64_t i);

#endif  // FES_H_INCLUDED
