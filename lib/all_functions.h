#ifndef ALL_FUNCTIONS_H
#define ALL_FUNCTIONS_H

#include "fes.h"
#include <stdint.h>

void exhaustive_ia32_deg_2(LUT_t LUT, int n, pck_vector_t F[],
                           solution_callback_t callback, void *callback_state,
                           int verbose);
pck_vector_t packed_eval_deg_2(LUT_t LUT, int n, pck_vector_t F[], uint64_t i);

#endif
