#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "fes.h"
#include "idx_LUT.h"

pck_vector_t packed_eval_deg_4(LUT_t LUT, int n, pck_vector_t F[], uint64_t i) {
  // first expand the values of the variables from `i`
  pck_vector_t v[n];
  for(int k=0; k<n; k++) {
    v[k] = 0;
    if (i & 0x0001) v[k] = 0xffffffff;
    i = (i >> 1ll);
  }

  pck_vector_t y = F[0];

  for(int idx_0=0; idx_0<n; idx_0++) {
    const pck_vector_t v_0 = v[idx_0];

    // computes the contribution of degree-1 terms
    y ^= F[ idx_1(LUT, idx_0) ] & v_0;

    for(int idx_1=0; idx_1<idx_0; idx_1++) {
      const pck_vector_t v_1 = v_0 & v[idx_1];

      // computes the contribution of degree-2 terms
      y ^= F[ idx_2(LUT, idx_1, idx_0) ] & v_1;

      for(int idx_2=0; idx_2<idx_1; idx_2++) {
        const pck_vector_t v_2 = v_1 & v[idx_2];

        // computes the contribution of degree-3 terms
        y ^= F[ idx_3(LUT, idx_2, idx_1, idx_0) ] & v_2;

        for(int idx_3=0; idx_3<idx_2; idx_3++) {
          const pck_vector_t v_3 = v_2 & v[idx_3];

          // computes the contribution of degree-4 terms
          y ^= F[ idx_4(LUT, idx_3, idx_2, idx_1, idx_0) ] & v_3;


 }}}}

  return y;
}
