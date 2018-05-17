#ifndef FES_COMMON
#define FES_COMMON

#include <stdlib.h>
#include <stdint.h>

#include "idx_LUT.h"
#include "fes_interface.h"

typedef int*  vector_t;
typedef vector_t* matrix_t;
typedef uint32_t pck_vector_t;

#define SIMD_CHUNK_SIZE 9
#define IA32_CHUNK_SIZE 9
#define enumerated_degree_bound 10


#define min(x,y) (((x) > (y)) ? (y) : (x)) 

typedef struct {
  matrix_t quad;
  vector_t lin;
  int con;
} quadratic_form;

typedef struct {
  int n;
  int degree;
  int n_batches;
  pck_vector_t **G;
  idx_lut_t * testing_LUT;
  
  solution_callback_t callback;
  void *callback_state;
} wrapper_state_t;

typedef quadratic_form* system_t;

void free_vector(vector_t x);
vector_t init_vector(int n_rows);
pck_vector_t pack(int n, const vector_t v);

uint64_t to_gray(uint64_t i);

#define unlikely(x)     __builtin_expect(!!(x), 0)

pck_vector_t packed_eval(LUT_t LUT, int n, int d, pck_vector_t *F, uint64_t i);
void variables_specialization(LUT_t LUT, int n, int d, pck_vector_t* A, int k, int i);
system_t generate_random_system(int n, int u);
void free_system(int n, int n_eqs, system_t f);

#include "all_functions.h"


#endif
