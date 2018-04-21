#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "fes.h"
#include "idx_LUT.h"

#define STEP_0(i) { \
  if (unlikely(F[ 0 ] == 0)) { \
      solution_buffer[n_solutions_found].int_idx = i; \
      solution_buffer[n_solutions_found].mask = 0x000f; \
      n_solutions_found++; \
   }\
}

#define STEP_1(a,i) { \
  F[ 0 ] ^= F [ a ]; \
  if (unlikely(F[ 0 ] == 0)) { \
      solution_buffer[n_solutions_found].int_idx = i; \
      solution_buffer[n_solutions_found].mask = 0x000f; \
      n_solutions_found++; \
   }\
}

#define STEP_2(a,b,i) { \
  F[ a ] ^= F [ b ]; \
  F[ 0 ] ^= F [ a ]; \
  if (unlikely(F[ 0 ] == 0)) { \
      solution_buffer[n_solutions_found].int_idx = i; \
      solution_buffer[n_solutions_found].mask = 0x000f; \
      n_solutions_found++; \
   }\
}

#define STEP_3(a,b,c,i) { \
  F[ b ] ^= F [ c ]; \
  F[ a ] ^= F [ b ]; \
  F[ 0 ] ^= F [ a ]; \
  if (unlikely(F[ 0 ] == 0)) { \
      solution_buffer[n_solutions_found].int_idx = i; \
      solution_buffer[n_solutions_found].mask = 0x000f; \
      n_solutions_found++; \
   }\
}

#define STEP_4(a,b,c,d,i) { \
  F[ c ] ^= F [ d ]; \
  F[ b ] ^= F [ c ]; \
  F[ a ] ^= F [ b ]; \
  F[ 0 ] ^= F [ a ]; \
  if (unlikely(F[ 0 ] == 0)) { \
      solution_buffer[n_solutions_found].int_idx = i; \
      solution_buffer[n_solutions_found].mask = 0x000f; \
      n_solutions_found++; \
   }\
}

#define STEP_5(a,b,c,d,e,i) { \
  F[ d ] ^= F [ e ]; \
  F[ c ] ^= F [ d ]; \
  F[ b ] ^= F [ c ]; \
  F[ a ] ^= F [ b ]; \
  F[ 0 ] ^= F [ a ]; \
  if (unlikely(F[ 0 ] == 0)) { \
      solution_buffer[n_solutions_found].int_idx = i; \
      solution_buffer[n_solutions_found].mask = 0x000f; \
      n_solutions_found++; \
   }\
}

#define STEP_6(a,b,c,d,e,f,i) { \
  F[ e ] ^= F [ f ]; \
  F[ d ] ^= F [ e ]; \
  F[ c ] ^= F [ d ]; \
  F[ b ] ^= F [ c ]; \
  F[ a ] ^= F [ b ]; \
  F[ 0 ] ^= F [ a ]; \
  if (unlikely(F[ 0 ] == 0)) { \
      solution_buffer[n_solutions_found].int_idx = i; \
      solution_buffer[n_solutions_found].mask = 0x000f; \
      n_solutions_found++; \
   }\
}

#define STEP_7(a,b,c,d,e,f,g,i) { \
  F[ f ] ^= F [ g ]; \
  F[ e ] ^= F [ f ]; \
  F[ d ] ^= F [ e ]; \
  F[ c ] ^= F [ d ]; \
  F[ b ] ^= F [ c ]; \
  F[ a ] ^= F [ b ]; \
  F[ 0 ] ^= F [ a ]; \
  if (unlikely(F[ 0 ] == 0)) { \
      solution_buffer[n_solutions_found].int_idx = i; \
      solution_buffer[n_solutions_found].mask = 0x000f; \
      n_solutions_found++; \
   }\
}


typedef struct {
  uint64_t int_idx;
  uint32_t mask;
} solution_t;

// generated with L = 7
void exhaustive_ia32_deg_7(LUT_t LUT, int n, pck_vector_t F[], solution_callback_t callback, void* callback_state, int verbose) {
  #define QUIT() { \
    return; \
  }

  uint64_t init_start_time = rdtsc();
  // computes the derivatives required by the enumeration kernel up to degree 7
  // this is done in-place, meaning that if "F" described the coefficients of the
  // polynomials before, then afterwards, they describe the derivatives

  // here, degree-1 terms are affected by degree-2 terms
  for(int i0=1; i0<n; i0++) {
      if (i0  != 0 ) F[ idx_1(LUT, i0) ] ^= F[ idx_2(LUT, i0-1, i0) ];
  }

  // here, degree-2 terms are affected by degree-3 terms
  for(int i1=2; i1<n; i1++) {
      for(int i0=0; i0<i1; i0++) {
        if (i0  != 0 ) F[ idx_2(LUT, i0, i1) ] ^= F[ idx_3(LUT, i0-1, i0, i1) ];
        if (i1-1 > i0) F[ idx_2(LUT, i0, i1) ] ^= F[ idx_3(LUT, i0, i1-1, i1) ];
  }}

  // here, degree-2 terms are affected by degree-4 terms
  for(int i1=2; i1<n; i1++) {
      for(int i0=0; i0<i1; i0++) {
        if (i0  != 0  && i1-1 > i0) F[ idx_2(LUT, i0, i1) ] ^= F[ idx_4(LUT, i0-1, i0, i1-1, i1) ];
  }}

  // here, degree-3 terms are affected by degree-4 terms
  for(int i2=3; i2<n; i2++) {
      for(int i1=1; i1<i2; i1++) {
        for(int i0=0; i0<i1; i0++) {
          if (i0  != 0 ) F[ idx_3(LUT, i0, i1, i2) ] ^= F[ idx_4(LUT, i0-1, i0, i1, i2) ];
          if (i1-1 > i0) F[ idx_3(LUT, i0, i1, i2) ] ^= F[ idx_4(LUT, i0, i1-1, i1, i2) ];
          if (i2-1 > i1) F[ idx_3(LUT, i0, i1, i2) ] ^= F[ idx_4(LUT, i0, i1, i2-1, i2) ];
  }}}

  // here, degree-3 terms are affected by degree-5 terms
  for(int i2=3; i2<n; i2++) {
      for(int i1=1; i1<i2; i1++) {
        for(int i0=0; i0<i1; i0++) {
          if (i0  != 0  && i1-1 > i0) F[ idx_3(LUT, i0, i1, i2) ] ^= F[ idx_5(LUT, i0-1, i0, i1-1, i1, i2) ];
          if (i0  != 0  && i2-1 > i1) F[ idx_3(LUT, i0, i1, i2) ] ^= F[ idx_5(LUT, i0-1, i0, i1, i2-1, i2) ];
          if (i1-1 > i0 && i2-1 > i1) F[ idx_3(LUT, i0, i1, i2) ] ^= F[ idx_5(LUT, i0, i1-1, i1, i2-1, i2) ];
  }}}

  // here, degree-3 terms are affected by degree-6 terms
  for(int i2=3; i2<n; i2++) {
      for(int i1=1; i1<i2; i1++) {
        for(int i0=0; i0<i1; i0++) {
          if (i0  != 0  && i1-1 > i0 && i2-1 > i1) F[ idx_3(LUT, i0, i1, i2) ] ^= F[ idx_6(LUT, i0-1, i0, i1-1, i1, i2-1, i2) ];
  }}}

  // here, degree-4 terms are affected by degree-5 terms
  for(int i3=4; i3<n; i3++) {
      for(int i2=2; i2<i3; i2++) {
        for(int i1=1; i1<i2; i1++) {
          for(int i0=0; i0<i1; i0++) {
            if (i0  != 0 ) F[ idx_4(LUT, i0, i1, i2, i3) ] ^= F[ idx_5(LUT, i0-1, i0, i1, i2, i3) ];
            if (i1-1 > i0) F[ idx_4(LUT, i0, i1, i2, i3) ] ^= F[ idx_5(LUT, i0, i1-1, i1, i2, i3) ];
            if (i2-1 > i1) F[ idx_4(LUT, i0, i1, i2, i3) ] ^= F[ idx_5(LUT, i0, i1, i2-1, i2, i3) ];
            if (i3-1 > i2) F[ idx_4(LUT, i0, i1, i2, i3) ] ^= F[ idx_5(LUT, i0, i1, i2, i3-1, i3) ];
  }}}}

  // here, degree-4 terms are affected by degree-6 terms
  for(int i3=4; i3<n; i3++) {
      for(int i2=2; i2<i3; i2++) {
        for(int i1=1; i1<i2; i1++) {
          for(int i0=0; i0<i1; i0++) {
            if (i0  != 0  && i1-1 > i0) F[ idx_4(LUT, i0, i1, i2, i3) ] ^= F[ idx_6(LUT, i0-1, i0, i1-1, i1, i2, i3) ];
            if (i0  != 0  && i2-1 > i1) F[ idx_4(LUT, i0, i1, i2, i3) ] ^= F[ idx_6(LUT, i0-1, i0, i1, i2-1, i2, i3) ];
            if (i0  != 0  && i3-1 > i2) F[ idx_4(LUT, i0, i1, i2, i3) ] ^= F[ idx_6(LUT, i0-1, i0, i1, i2, i3-1, i3) ];
            if (i1-1 > i0 && i2-1 > i1) F[ idx_4(LUT, i0, i1, i2, i3) ] ^= F[ idx_6(LUT, i0, i1-1, i1, i2-1, i2, i3) ];
            if (i1-1 > i0 && i3-1 > i2) F[ idx_4(LUT, i0, i1, i2, i3) ] ^= F[ idx_6(LUT, i0, i1-1, i1, i2, i3-1, i3) ];
            if (i2-1 > i1 && i3-1 > i2) F[ idx_4(LUT, i0, i1, i2, i3) ] ^= F[ idx_6(LUT, i0, i1, i2-1, i2, i3-1, i3) ];
  }}}}

  // here, degree-4 terms are affected by degree-7 terms
  for(int i3=4; i3<n; i3++) {
      for(int i2=2; i2<i3; i2++) {
        for(int i1=1; i1<i2; i1++) {
          for(int i0=0; i0<i1; i0++) {
            if (i0  != 0  && i1-1 > i0 && i2-1 > i1) F[ idx_4(LUT, i0, i1, i2, i3) ] ^= F[ idx_7(LUT, i0-1, i0, i1-1, i1, i2-1, i2, i3) ];
            if (i0  != 0  && i1-1 > i0 && i3-1 > i2) F[ idx_4(LUT, i0, i1, i2, i3) ] ^= F[ idx_7(LUT, i0-1, i0, i1-1, i1, i2, i3-1, i3) ];
            if (i0  != 0  && i2-1 > i1 && i3-1 > i2) F[ idx_4(LUT, i0, i1, i2, i3) ] ^= F[ idx_7(LUT, i0-1, i0, i1, i2-1, i2, i3-1, i3) ];
            if (i1-1 > i0 && i2-1 > i1 && i3-1 > i2) F[ idx_4(LUT, i0, i1, i2, i3) ] ^= F[ idx_7(LUT, i0, i1-1, i1, i2-1, i2, i3-1, i3) ];
  }}}}

  // here, degree-5 terms are affected by degree-6 terms
  for(int i4=5; i4<n; i4++) {
      for(int i3=3; i3<i4; i3++) {
        for(int i2=2; i2<i3; i2++) {
          for(int i1=1; i1<i2; i1++) {
            for(int i0=0; i0<i1; i0++) {
              if (i0  != 0 ) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_6(LUT, i0-1, i0, i1, i2, i3, i4) ];
              if (i1-1 > i0) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_6(LUT, i0, i1-1, i1, i2, i3, i4) ];
              if (i2-1 > i1) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_6(LUT, i0, i1, i2-1, i2, i3, i4) ];
              if (i3-1 > i2) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_6(LUT, i0, i1, i2, i3-1, i3, i4) ];
              if (i4-1 > i3) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_6(LUT, i0, i1, i2, i3, i4-1, i4) ];
  }}}}}

  // here, degree-5 terms are affected by degree-7 terms
  for(int i4=5; i4<n; i4++) {
      for(int i3=3; i3<i4; i3++) {
        for(int i2=2; i2<i3; i2++) {
          for(int i1=1; i1<i2; i1++) {
            for(int i0=0; i0<i1; i0++) {
              if (i0  != 0  && i1-1 > i0) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_7(LUT, i0-1, i0, i1-1, i1, i2, i3, i4) ];
              if (i0  != 0  && i2-1 > i1) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_7(LUT, i0-1, i0, i1, i2-1, i2, i3, i4) ];
              if (i0  != 0  && i3-1 > i2) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_7(LUT, i0-1, i0, i1, i2, i3-1, i3, i4) ];
              if (i0  != 0  && i4-1 > i3) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_7(LUT, i0-1, i0, i1, i2, i3, i4-1, i4) ];
              if (i1-1 > i0 && i2-1 > i1) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_7(LUT, i0, i1-1, i1, i2-1, i2, i3, i4) ];
              if (i1-1 > i0 && i3-1 > i2) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_7(LUT, i0, i1-1, i1, i2, i3-1, i3, i4) ];
              if (i1-1 > i0 && i4-1 > i3) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_7(LUT, i0, i1-1, i1, i2, i3, i4-1, i4) ];
              if (i2-1 > i1 && i3-1 > i2) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_7(LUT, i0, i1, i2-1, i2, i3-1, i3, i4) ];
              if (i2-1 > i1 && i4-1 > i3) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_7(LUT, i0, i1, i2-1, i2, i3, i4-1, i4) ];
              if (i3-1 > i2 && i4-1 > i3) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_7(LUT, i0, i1, i2, i3-1, i3, i4-1, i4) ];
  }}}}}

  // here, degree-6 terms are affected by degree-7 terms
  for(int i5=6; i5<n; i5++) {
      for(int i4=4; i4<i5; i4++) {
        for(int i3=3; i3<i4; i3++) {
          for(int i2=2; i2<i3; i2++) {
            for(int i1=1; i1<i2; i1++) {
              for(int i0=0; i0<i1; i0++) {
                if (i0  != 0 ) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_7(LUT, i0-1, i0, i1, i2, i3, i4, i5) ];
                if (i1-1 > i0) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_7(LUT, i0, i1-1, i1, i2, i3, i4, i5) ];
                if (i2-1 > i1) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_7(LUT, i0, i1, i2-1, i2, i3, i4, i5) ];
                if (i3-1 > i2) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_7(LUT, i0, i1, i2, i3-1, i3, i4, i5) ];
                if (i4-1 > i3) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_7(LUT, i0, i1, i2, i3, i4-1, i4, i5) ];
                if (i5-1 > i4) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_7(LUT, i0, i1, i2, i3, i4, i5-1, i5) ];
  }}}}}}



  if (verbose) printf("fes: initialisation = %" PRIu64 " cycles\n", rdtsc()-init_start_time);
  uint64_t enumeration_start_time = rdtsc();
  uint64_t n_solutions_found = 0;
  uint64_t current_solution_index = 0;
  uint64_t pack_of_solution[65536];
  solution_t solution_buffer[256];

  #define FLUSH_SOLUTIONS() { \
    if ((*callback)(callback_state, current_solution_index, pack_of_solution)) \
      QUIT(); \
    }

  #define PUSH_SOLUTION(current_solution) { \
    pack_of_solution[current_solution_index] = current_solution; \
    current_solution_index++; \
    if (current_solution_index == 65536){ \
      FLUSH_SOLUTIONS(); \
      current_solution_index = 0; \
    } \
  }

  #define CHECK_SOLUTIONS() { \
    for(uint64_t i=0; i<n_solutions_found; i++){ \
      if (solution_buffer[i].mask & 0xffff) \
        PUSH_SOLUTION(to_gray(solution_buffer[i].int_idx) + 0*(1ll << (n-0))); \
    } \
    n_solutions_found = 0; \
  }

  // special case for i=0
  const uint64_t weight_0_start = 0;
  STEP_0(0);

  // from now on, hamming weight is >= 1
  for(int idx_0=0; idx_0<n    ; idx_0++) {

    // special case when i has hamming weight exactly 1
    const uint64_t weight_1_start = weight_0_start + (1ll << idx_0);
      STEP_1( idx_1(LUT, idx_0), weight_1_start );

    for(int idx_1=0; idx_1<idx_0; idx_1++) {

      // special case when i has hamming weight exactly 2
      const uint64_t weight_2_start = weight_1_start + (1ll << idx_1);
        STEP_2( idx_1(LUT, idx_1), idx_2(LUT, idx_1, idx_0), weight_2_start );

      for(int idx_2=0; idx_2<idx_1; idx_2++) {

        // special case when i has hamming weight exactly 3
        const uint64_t weight_3_start = weight_2_start + (1ll << idx_2);
          STEP_3( idx_1(LUT, idx_2), idx_2(LUT, idx_2, idx_1), idx_3(LUT, idx_2, idx_1, idx_0), weight_3_start );

        for(int idx_3=0; idx_3<idx_2; idx_3++) {

          // special case when i has hamming weight exactly 4
          const uint64_t weight_4_start = weight_3_start + (1ll << idx_3);
            STEP_4( idx_1(LUT, idx_3), idx_2(LUT, idx_3, idx_2), idx_3(LUT, idx_3, idx_2, idx_1), idx_4(LUT, idx_3, idx_2, idx_1, idx_0), weight_4_start );

          for(int idx_4=0; idx_4<idx_3; idx_4++) {

            // special case when i has hamming weight exactly 5
            const uint64_t weight_5_start = weight_4_start + (1ll << idx_4);
              STEP_5( idx_1(LUT, idx_4), idx_2(LUT, idx_4, idx_3), idx_3(LUT, idx_4, idx_3, idx_2), idx_4(LUT, idx_4, idx_3, idx_2, idx_1), idx_5(LUT, idx_4, idx_3, idx_2, idx_1, idx_0), weight_5_start );

            for(int idx_5=0; idx_5<idx_4; idx_5++) {

              // special case when i has hamming weight exactly 6
              const uint64_t weight_6_start = weight_5_start + (1ll << idx_5);
                STEP_6( idx_1(LUT, idx_5), idx_2(LUT, idx_5, idx_4), idx_3(LUT, idx_5, idx_4, idx_3), idx_4(LUT, idx_5, idx_4, idx_3, idx_2), idx_5(LUT, idx_5, idx_4, idx_3, idx_2, idx_1), idx_6(LUT, idx_5, idx_4, idx_3, idx_2, idx_1, idx_0), weight_6_start );

                // we are now inside the critical part where the hamming weight is known to be >= 7
                // Thus, there are no special cases from now on

                // Because of the last step, the current iteration counter is a multiple of 128 plus one
                // This loop sets it to `rolled_end`, which is a multiple of 128, if possible

              const uint64_t rolled_end = weight_6_start + (1ll << min(7, idx_5));
              for(uint64_t i=1 + weight_6_start; i< rolled_end; i++) {

                    int pos = 0;
                    uint64_t _i = i;
                    while ((_i & 0x0001) == 0) { _i >>= 1; pos++; }
                    const int k_1 = pos;
                    _i >>= 1; pos++;
                    while ((_i & 0x0001) == 0) { _i >>= 1; pos++; }
                    const int k_2 = pos;
                    _i >>= 1; pos++;
                    while ((_i & 0x0001) == 0) { _i >>= 1; pos++; }
                    const int k_3 = pos;
                    _i >>= 1; pos++;
                    while ((_i & 0x0001) == 0) { _i >>= 1; pos++; }
                    const int k_4 = pos;
                    _i >>= 1; pos++;
                    while ((_i & 0x0001) == 0) { _i >>= 1; pos++; }
                    const int k_5 = pos;
                    _i >>= 1; pos++;
                    while ((_i & 0x0001) == 0) { _i >>= 1; pos++; }
                    const int k_6 = pos;
                    _i >>= 1; pos++;
                    while ((_i & 0x0001) == 0) { _i >>= 1; pos++; }
                    const int k_7 = pos;
                  STEP_7( idx_1(LUT, k_1), idx_2(LUT, k_1, k_2), idx_3(LUT, k_1, k_2, k_3), idx_4(LUT, k_1, k_2, k_3, k_4), idx_5(LUT, k_1, k_2, k_3, k_4, k_5), idx_6(LUT, k_1, k_2, k_3, k_4, k_5, k_6), idx_7(LUT, k_1, k_2, k_3, k_4, k_5, k_6, k_7), i );
              }

              CHECK_SOLUTIONS();

              // Here, the number of iterations to perform is (supposedly) sufficiently large
              // We will therefore unroll the loop 128 times

              // unrolled critical section where the hamming weight is >= 7
              for(uint64_t j=128; j<(1ull << idx_5); j+=128) {
                    const uint64_t i = j + weight_6_start;
                    int pos = 0;
                    uint64_t _i = i;
                    while ((_i & 0x0001) == 0) { _i >>= 1; pos++; }
                    const int k_1 = pos;
                    _i >>= 1; pos++;
                    while ((_i & 0x0001) == 0) { _i >>= 1; pos++; }
                    const int k_2 = pos;
                    _i >>= 1; pos++;
                    while ((_i & 0x0001) == 0) { _i >>= 1; pos++; }
                    const int k_3 = pos;
                    _i >>= 1; pos++;
                    while ((_i & 0x0001) == 0) { _i >>= 1; pos++; }
                    const int k_4 = pos;
                    _i >>= 1; pos++;
                    while ((_i & 0x0001) == 0) { _i >>= 1; pos++; }
                    const int k_5 = pos;
                    _i >>= 1; pos++;
                    while ((_i & 0x0001) == 0) { _i >>= 1; pos++; }
                    const int k_6 = pos;
                    _i >>= 1; pos++;
                    while ((_i & 0x0001) == 0) { _i >>= 1; pos++; }
                    const int k_7 = pos;
                    const int alpha = LUT[0][k_1];
                    const int beta = LUT[1][k_1]+LUT[0][k_2];
                    const int gamma = LUT[2][k_1]+LUT[1][k_2]+LUT[0][k_3];
                    const int delta = LUT[3][k_1]+LUT[2][k_2]+LUT[1][k_3]+LUT[0][k_4];
                    const int epsilon = LUT[4][k_1]+LUT[3][k_2]+LUT[2][k_3]+LUT[1][k_4]+LUT[0][k_5];
                    const int zeta = LUT[5][k_1]+LUT[4][k_2]+LUT[3][k_3]+LUT[2][k_4]+LUT[1][k_5]+LUT[0][k_6];
                    const int eta = LUT[6][k_1]+LUT[5][k_2]+LUT[4][k_3]+LUT[3][k_4]+LUT[2][k_5]+LUT[1][k_6]+LUT[0][k_7];
                    STEP_7(0 + alpha, 0 + beta, 0 + gamma, 0 + delta, 0 + epsilon, 0 + zeta, 0 + eta, i + 0);
                    STEP_7(1, 1 + alpha, 1 + beta, 1 + gamma, 1 + delta, 1 + epsilon, 1 + zeta, i + 1);
                    STEP_7(2, 2 + alpha, 2 + beta, 2 + gamma, 2 + delta, 2 + epsilon, 2 + zeta, i + 2);
                    STEP_7(1, 3, 3 + alpha, 3 + beta, 3 + gamma, 3 + delta, 3 + epsilon, i + 3);
                    STEP_7(4, 4 + alpha, 4 + beta, 4 + gamma, 4 + delta, 4 + epsilon, 3 + zeta, i + 4);
                    STEP_7(1, 5, 5 + alpha, 5 + beta, 5 + gamma, 5 + delta, 5 + epsilon, i + 5);
                    STEP_7(2, 6, 6 + alpha, 6 + beta, 6 + gamma, 6 + delta, 6 + epsilon, i + 6);
                    STEP_7(1, 3, 7, 7 + alpha, 7 + beta, 7 + gamma, 7 + delta, i + 7);
                    STEP_7(8, 8 + alpha, 8 + beta, 8 + gamma, 8 + delta, 7 + epsilon, 4 + zeta, i + 8);
                    STEP_7(1, 9, 9 + alpha, 9 + beta, 9 + gamma, 9 + delta, 8 + epsilon, i + 9);
                    STEP_7(2, 10, 10 + alpha, 10 + beta, 10 + gamma, 10 + delta, 9 + epsilon, i + 10);
                    STEP_7(1, 3, 11, 11 + alpha, 11 + beta, 11 + gamma, 11 + delta, i + 11);
                    STEP_7(4, 12, 12 + alpha, 12 + beta, 12 + gamma, 12 + delta, 10 + epsilon, i + 12);
                    STEP_7(1, 5, 13, 13 + alpha, 13 + beta, 13 + gamma, 13 + delta, i + 13);
                    STEP_7(2, 6, 14, 14 + alpha, 14 + beta, 14 + gamma, 14 + delta, i + 14);
                    STEP_7(1, 3, 7, 15, 15 + alpha, 15 + beta, 15 + gamma, i + 15);
                    STEP_7(16, 16 + alpha, 16 + beta, 16 + gamma, 15 + delta, 11 + epsilon, 5 + zeta, i + 16);
                    STEP_7(1, 17, 17 + alpha, 17 + beta, 17 + gamma, 16 + delta, 12 + epsilon, i + 17);
                    STEP_7(2, 18, 18 + alpha, 18 + beta, 18 + gamma, 17 + delta, 13 + epsilon, i + 18);
                    STEP_7(1, 3, 19, 19 + alpha, 19 + beta, 19 + gamma, 18 + delta, i + 19);
                    STEP_7(4, 20, 20 + alpha, 20 + beta, 20 + gamma, 19 + delta, 14 + epsilon, i + 20);
                    STEP_7(1, 5, 21, 21 + alpha, 21 + beta, 21 + gamma, 20 + delta, i + 21);
                    STEP_7(2, 6, 22, 22 + alpha, 22 + beta, 22 + gamma, 21 + delta, i + 22);
                    STEP_7(1, 3, 7, 23, 23 + alpha, 23 + beta, 23 + gamma, i + 23);
                    STEP_7(8, 24, 24 + alpha, 24 + beta, 24 + gamma, 22 + delta, 15 + epsilon, i + 24);
                    STEP_7(1, 9, 25, 25 + alpha, 25 + beta, 25 + gamma, 23 + delta, i + 25);
                    STEP_7(2, 10, 26, 26 + alpha, 26 + beta, 26 + gamma, 24 + delta, i + 26);
                    STEP_7(1, 3, 11, 27, 27 + alpha, 27 + beta, 27 + gamma, i + 27);
                    STEP_7(4, 12, 28, 28 + alpha, 28 + beta, 28 + gamma, 25 + delta, i + 28);
                    STEP_7(1, 5, 13, 29, 29 + alpha, 29 + beta, 29 + gamma, i + 29);
                    STEP_7(2, 6, 14, 30, 30 + alpha, 30 + beta, 30 + gamma, i + 30);
                    STEP_7(1, 3, 7, 15, 31, 31 + alpha, 31 + beta, i + 31);
                    STEP_7(32, 32 + alpha, 32 + beta, 31 + gamma, 26 + delta, 16 + epsilon, 6 + zeta, i + 32);
                    STEP_7(1, 33, 33 + alpha, 33 + beta, 32 + gamma, 27 + delta, 17 + epsilon, i + 33);
                    STEP_7(2, 34, 34 + alpha, 34 + beta, 33 + gamma, 28 + delta, 18 + epsilon, i + 34);
                    STEP_7(1, 3, 35, 35 + alpha, 35 + beta, 34 + gamma, 29 + delta, i + 35);
                    STEP_7(4, 36, 36 + alpha, 36 + beta, 35 + gamma, 30 + delta, 19 + epsilon, i + 36);
                    STEP_7(1, 5, 37, 37 + alpha, 37 + beta, 36 + gamma, 31 + delta, i + 37);
                    STEP_7(2, 6, 38, 38 + alpha, 38 + beta, 37 + gamma, 32 + delta, i + 38);
                    STEP_7(1, 3, 7, 39, 39 + alpha, 39 + beta, 38 + gamma, i + 39);
                    STEP_7(8, 40, 40 + alpha, 40 + beta, 39 + gamma, 33 + delta, 20 + epsilon, i + 40);
                    STEP_7(1, 9, 41, 41 + alpha, 41 + beta, 40 + gamma, 34 + delta, i + 41);
                    STEP_7(2, 10, 42, 42 + alpha, 42 + beta, 41 + gamma, 35 + delta, i + 42);
                    STEP_7(1, 3, 11, 43, 43 + alpha, 43 + beta, 42 + gamma, i + 43);
                    STEP_7(4, 12, 44, 44 + alpha, 44 + beta, 43 + gamma, 36 + delta, i + 44);
                    STEP_7(1, 5, 13, 45, 45 + alpha, 45 + beta, 44 + gamma, i + 45);
                    STEP_7(2, 6, 14, 46, 46 + alpha, 46 + beta, 45 + gamma, i + 46);
                    STEP_7(1, 3, 7, 15, 47, 47 + alpha, 47 + beta, i + 47);
                    STEP_7(16, 48, 48 + alpha, 48 + beta, 46 + gamma, 37 + delta, 21 + epsilon, i + 48);
                    STEP_7(1, 17, 49, 49 + alpha, 49 + beta, 47 + gamma, 38 + delta, i + 49);
                    STEP_7(2, 18, 50, 50 + alpha, 50 + beta, 48 + gamma, 39 + delta, i + 50);
                    STEP_7(1, 3, 19, 51, 51 + alpha, 51 + beta, 49 + gamma, i + 51);
                    STEP_7(4, 20, 52, 52 + alpha, 52 + beta, 50 + gamma, 40 + delta, i + 52);
                    STEP_7(1, 5, 21, 53, 53 + alpha, 53 + beta, 51 + gamma, i + 53);
                    STEP_7(2, 6, 22, 54, 54 + alpha, 54 + beta, 52 + gamma, i + 54);
                    STEP_7(1, 3, 7, 23, 55, 55 + alpha, 55 + beta, i + 55);
                    STEP_7(8, 24, 56, 56 + alpha, 56 + beta, 53 + gamma, 41 + delta, i + 56);
                    STEP_7(1, 9, 25, 57, 57 + alpha, 57 + beta, 54 + gamma, i + 57);
                    STEP_7(2, 10, 26, 58, 58 + alpha, 58 + beta, 55 + gamma, i + 58);
                    STEP_7(1, 3, 11, 27, 59, 59 + alpha, 59 + beta, i + 59);
                    STEP_7(4, 12, 28, 60, 60 + alpha, 60 + beta, 56 + gamma, i + 60);
                    STEP_7(1, 5, 13, 29, 61, 61 + alpha, 61 + beta, i + 61);
                    STEP_7(2, 6, 14, 30, 62, 62 + alpha, 62 + beta, i + 62);
                    STEP_7(1, 3, 7, 15, 31, 63, 63 + alpha, i + 63);
                    STEP_7(64, 64 + alpha, 63 + beta, 57 + gamma, 42 + delta, 22 + epsilon, 7 + zeta, i + 64);
                    STEP_7(1, 65, 65 + alpha, 64 + beta, 58 + gamma, 43 + delta, 23 + epsilon, i + 65);
                    STEP_7(2, 66, 66 + alpha, 65 + beta, 59 + gamma, 44 + delta, 24 + epsilon, i + 66);
                    STEP_7(1, 3, 67, 67 + alpha, 66 + beta, 60 + gamma, 45 + delta, i + 67);
                    STEP_7(4, 68, 68 + alpha, 67 + beta, 61 + gamma, 46 + delta, 25 + epsilon, i + 68);
                    STEP_7(1, 5, 69, 69 + alpha, 68 + beta, 62 + gamma, 47 + delta, i + 69);
                    STEP_7(2, 6, 70, 70 + alpha, 69 + beta, 63 + gamma, 48 + delta, i + 70);
                    STEP_7(1, 3, 7, 71, 71 + alpha, 70 + beta, 64 + gamma, i + 71);
                    STEP_7(8, 72, 72 + alpha, 71 + beta, 65 + gamma, 49 + delta, 26 + epsilon, i + 72);
                    STEP_7(1, 9, 73, 73 + alpha, 72 + beta, 66 + gamma, 50 + delta, i + 73);
                    STEP_7(2, 10, 74, 74 + alpha, 73 + beta, 67 + gamma, 51 + delta, i + 74);
                    STEP_7(1, 3, 11, 75, 75 + alpha, 74 + beta, 68 + gamma, i + 75);
                    STEP_7(4, 12, 76, 76 + alpha, 75 + beta, 69 + gamma, 52 + delta, i + 76);
                    STEP_7(1, 5, 13, 77, 77 + alpha, 76 + beta, 70 + gamma, i + 77);
                    STEP_7(2, 6, 14, 78, 78 + alpha, 77 + beta, 71 + gamma, i + 78);
                    STEP_7(1, 3, 7, 15, 79, 79 + alpha, 78 + beta, i + 79);
                    STEP_7(16, 80, 80 + alpha, 79 + beta, 72 + gamma, 53 + delta, 27 + epsilon, i + 80);
                    STEP_7(1, 17, 81, 81 + alpha, 80 + beta, 73 + gamma, 54 + delta, i + 81);
                    STEP_7(2, 18, 82, 82 + alpha, 81 + beta, 74 + gamma, 55 + delta, i + 82);
                    STEP_7(1, 3, 19, 83, 83 + alpha, 82 + beta, 75 + gamma, i + 83);
                    STEP_7(4, 20, 84, 84 + alpha, 83 + beta, 76 + gamma, 56 + delta, i + 84);
                    STEP_7(1, 5, 21, 85, 85 + alpha, 84 + beta, 77 + gamma, i + 85);
                    STEP_7(2, 6, 22, 86, 86 + alpha, 85 + beta, 78 + gamma, i + 86);
                    STEP_7(1, 3, 7, 23, 87, 87 + alpha, 86 + beta, i + 87);
                    STEP_7(8, 24, 88, 88 + alpha, 87 + beta, 79 + gamma, 57 + delta, i + 88);
                    STEP_7(1, 9, 25, 89, 89 + alpha, 88 + beta, 80 + gamma, i + 89);
                    STEP_7(2, 10, 26, 90, 90 + alpha, 89 + beta, 81 + gamma, i + 90);
                    STEP_7(1, 3, 11, 27, 91, 91 + alpha, 90 + beta, i + 91);
                    STEP_7(4, 12, 28, 92, 92 + alpha, 91 + beta, 82 + gamma, i + 92);
                    STEP_7(1, 5, 13, 29, 93, 93 + alpha, 92 + beta, i + 93);
                    STEP_7(2, 6, 14, 30, 94, 94 + alpha, 93 + beta, i + 94);
                    STEP_7(1, 3, 7, 15, 31, 95, 95 + alpha, i + 95);
                    STEP_7(32, 96, 96 + alpha, 94 + beta, 83 + gamma, 58 + delta, 28 + epsilon, i + 96);
                    STEP_7(1, 33, 97, 97 + alpha, 95 + beta, 84 + gamma, 59 + delta, i + 97);
                    STEP_7(2, 34, 98, 98 + alpha, 96 + beta, 85 + gamma, 60 + delta, i + 98);
                    STEP_7(1, 3, 35, 99, 99 + alpha, 97 + beta, 86 + gamma, i + 99);
                    STEP_7(4, 36, 100, 100 + alpha, 98 + beta, 87 + gamma, 61 + delta, i + 100);
                    STEP_7(1, 5, 37, 101, 101 + alpha, 99 + beta, 88 + gamma, i + 101);
                    STEP_7(2, 6, 38, 102, 102 + alpha, 100 + beta, 89 + gamma, i + 102);
                    STEP_7(1, 3, 7, 39, 103, 103 + alpha, 101 + beta, i + 103);
                    STEP_7(8, 40, 104, 104 + alpha, 102 + beta, 90 + gamma, 62 + delta, i + 104);
                    STEP_7(1, 9, 41, 105, 105 + alpha, 103 + beta, 91 + gamma, i + 105);
                    STEP_7(2, 10, 42, 106, 106 + alpha, 104 + beta, 92 + gamma, i + 106);
                    STEP_7(1, 3, 11, 43, 107, 107 + alpha, 105 + beta, i + 107);
                    STEP_7(4, 12, 44, 108, 108 + alpha, 106 + beta, 93 + gamma, i + 108);
                    STEP_7(1, 5, 13, 45, 109, 109 + alpha, 107 + beta, i + 109);
                    STEP_7(2, 6, 14, 46, 110, 110 + alpha, 108 + beta, i + 110);
                    STEP_7(1, 3, 7, 15, 47, 111, 111 + alpha, i + 111);
                    STEP_7(16, 48, 112, 112 + alpha, 109 + beta, 94 + gamma, 63 + delta, i + 112);
                    STEP_7(1, 17, 49, 113, 113 + alpha, 110 + beta, 95 + gamma, i + 113);
                    STEP_7(2, 18, 50, 114, 114 + alpha, 111 + beta, 96 + gamma, i + 114);
                    STEP_7(1, 3, 19, 51, 115, 115 + alpha, 112 + beta, i + 115);
                    STEP_7(4, 20, 52, 116, 116 + alpha, 113 + beta, 97 + gamma, i + 116);
                    STEP_7(1, 5, 21, 53, 117, 117 + alpha, 114 + beta, i + 117);
                    STEP_7(2, 6, 22, 54, 118, 118 + alpha, 115 + beta, i + 118);
                    STEP_7(1, 3, 7, 23, 55, 119, 119 + alpha, i + 119);
                    STEP_7(8, 24, 56, 120, 120 + alpha, 116 + beta, 98 + gamma, i + 120);
                    STEP_7(1, 9, 25, 57, 121, 121 + alpha, 117 + beta, i + 121);
                    STEP_7(2, 10, 26, 58, 122, 122 + alpha, 118 + beta, i + 122);
                    STEP_7(1, 3, 11, 27, 59, 123, 123 + alpha, i + 123);
                    STEP_7(4, 12, 28, 60, 124, 124 + alpha, 119 + beta, i + 124);
                    STEP_7(1, 5, 13, 29, 61, 125, 125 + alpha, i + 125);
                    STEP_7(2, 6, 14, 30, 62, 126, 126 + alpha, i + 126);
                    STEP_7(1, 3, 7, 15, 31, 63, 127, i + 127);

                    CHECK_SOLUTIONS();
                }

            }
          }
        }
      }
    }
  }
  FLUSH_SOLUTIONS();
  uint64_t end_time = rdtsc();
 if (verbose) printf("fes: enumeration+check = %" PRIu64 " cycles\n", end_time - enumeration_start_time);
  QUIT();
}
