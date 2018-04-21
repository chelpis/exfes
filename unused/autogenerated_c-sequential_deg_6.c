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


typedef struct {
  uint64_t int_idx;
  uint32_t mask;
} solution_t;

// generated with L = 7
void exhaustive_ia32_deg_6(LUT_t LUT, int n, pck_vector_t F[], solution_callback_t callback, void* callback_state, int verbose) {
  #define QUIT() { \
    return; \
  }

  uint64_t init_start_time = rdtsc();
  // computes the derivatives required by the enumeration kernel up to degree 6
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



  if (verbose) printf("fes: initialisation = %" PRIu64 " cycles\n", rdtsc()-init_start_time);
  uint64_t enumeration_start_time = rdtsc();
  uint64_t n_solutions_found = 0;
  uint64_t current_solution_index = 0;
  uint64_t pack_of_solution[65536];
  solution_t solution_buffer[192];

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

              // we are now inside the critical part where the hamming weight is known to be >= 6
              // Thus, there are no special cases from now on

              // Because of the last step, the current iteration counter is a multiple of 128 plus one
              // This loop sets it to `rolled_end`, which is a multiple of 128, if possible

            const uint64_t rolled_end = weight_5_start + (1ll << min(7, idx_4));
            for(uint64_t i=1 + weight_5_start; i< rolled_end; i++) {

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
                STEP_6( idx_1(LUT, k_1), idx_2(LUT, k_1, k_2), idx_3(LUT, k_1, k_2, k_3), idx_4(LUT, k_1, k_2, k_3, k_4), idx_5(LUT, k_1, k_2, k_3, k_4, k_5), idx_6(LUT, k_1, k_2, k_3, k_4, k_5, k_6), i );
            }

            CHECK_SOLUTIONS();

            // Here, the number of iterations to perform is (supposedly) sufficiently large
            // We will therefore unroll the loop 128 times

            // unrolled critical section where the hamming weight is >= 6
            for(uint64_t j=128; j<(1ull << idx_4); j+=128) {
                  const uint64_t i = j + weight_5_start;
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
                  const int alpha = LUT[0][k_1];
                  const int beta = LUT[1][k_1]+LUT[0][k_2];
                  const int gamma = LUT[2][k_1]+LUT[1][k_2]+LUT[0][k_3];
                  const int delta = LUT[3][k_1]+LUT[2][k_2]+LUT[1][k_3]+LUT[0][k_4];
                  const int epsilon = LUT[4][k_1]+LUT[3][k_2]+LUT[2][k_3]+LUT[1][k_4]+LUT[0][k_5];
                  const int zeta = LUT[5][k_1]+LUT[4][k_2]+LUT[3][k_3]+LUT[2][k_4]+LUT[1][k_5]+LUT[0][k_6];
                  STEP_6(0 + alpha, 0 + beta, 0 + gamma, 0 + delta, 0 + epsilon, 0 + zeta, i + 0);
                  STEP_6(1, 1 + alpha, 1 + beta, 1 + gamma, 1 + delta, 1 + epsilon, i + 1);
                  STEP_6(2, 2 + alpha, 2 + beta, 2 + gamma, 2 + delta, 2 + epsilon, i + 2);
                  STEP_6(1, 3, 3 + alpha, 3 + beta, 3 + gamma, 3 + delta, i + 3);
                  STEP_6(4, 4 + alpha, 4 + beta, 4 + gamma, 4 + delta, 3 + epsilon, i + 4);
                  STEP_6(1, 5, 5 + alpha, 5 + beta, 5 + gamma, 5 + delta, i + 5);
                  STEP_6(2, 6, 6 + alpha, 6 + beta, 6 + gamma, 6 + delta, i + 6);
                  STEP_6(1, 3, 7, 7 + alpha, 7 + beta, 7 + gamma, i + 7);
                  STEP_6(8, 8 + alpha, 8 + beta, 8 + gamma, 7 + delta, 4 + epsilon, i + 8);
                  STEP_6(1, 9, 9 + alpha, 9 + beta, 9 + gamma, 8 + delta, i + 9);
                  STEP_6(2, 10, 10 + alpha, 10 + beta, 10 + gamma, 9 + delta, i + 10);
                  STEP_6(1, 3, 11, 11 + alpha, 11 + beta, 11 + gamma, i + 11);
                  STEP_6(4, 12, 12 + alpha, 12 + beta, 12 + gamma, 10 + delta, i + 12);
                  STEP_6(1, 5, 13, 13 + alpha, 13 + beta, 13 + gamma, i + 13);
                  STEP_6(2, 6, 14, 14 + alpha, 14 + beta, 14 + gamma, i + 14);
                  STEP_6(1, 3, 7, 15, 15 + alpha, 15 + beta, i + 15);
                  STEP_6(16, 16 + alpha, 16 + beta, 15 + gamma, 11 + delta, 5 + epsilon, i + 16);
                  STEP_6(1, 17, 17 + alpha, 17 + beta, 16 + gamma, 12 + delta, i + 17);
                  STEP_6(2, 18, 18 + alpha, 18 + beta, 17 + gamma, 13 + delta, i + 18);
                  STEP_6(1, 3, 19, 19 + alpha, 19 + beta, 18 + gamma, i + 19);
                  STEP_6(4, 20, 20 + alpha, 20 + beta, 19 + gamma, 14 + delta, i + 20);
                  STEP_6(1, 5, 21, 21 + alpha, 21 + beta, 20 + gamma, i + 21);
                  STEP_6(2, 6, 22, 22 + alpha, 22 + beta, 21 + gamma, i + 22);
                  STEP_6(1, 3, 7, 23, 23 + alpha, 23 + beta, i + 23);
                  STEP_6(8, 24, 24 + alpha, 24 + beta, 22 + gamma, 15 + delta, i + 24);
                  STEP_6(1, 9, 25, 25 + alpha, 25 + beta, 23 + gamma, i + 25);
                  STEP_6(2, 10, 26, 26 + alpha, 26 + beta, 24 + gamma, i + 26);
                  STEP_6(1, 3, 11, 27, 27 + alpha, 27 + beta, i + 27);
                  STEP_6(4, 12, 28, 28 + alpha, 28 + beta, 25 + gamma, i + 28);
                  STEP_6(1, 5, 13, 29, 29 + alpha, 29 + beta, i + 29);
                  STEP_6(2, 6, 14, 30, 30 + alpha, 30 + beta, i + 30);
                  STEP_6(1, 3, 7, 15, 31, 31 + alpha, i + 31);
                  STEP_6(32, 32 + alpha, 31 + beta, 26 + gamma, 16 + delta, 6 + epsilon, i + 32);
                  STEP_6(1, 33, 33 + alpha, 32 + beta, 27 + gamma, 17 + delta, i + 33);
                  STEP_6(2, 34, 34 + alpha, 33 + beta, 28 + gamma, 18 + delta, i + 34);
                  STEP_6(1, 3, 35, 35 + alpha, 34 + beta, 29 + gamma, i + 35);
                  STEP_6(4, 36, 36 + alpha, 35 + beta, 30 + gamma, 19 + delta, i + 36);
                  STEP_6(1, 5, 37, 37 + alpha, 36 + beta, 31 + gamma, i + 37);
                  STEP_6(2, 6, 38, 38 + alpha, 37 + beta, 32 + gamma, i + 38);
                  STEP_6(1, 3, 7, 39, 39 + alpha, 38 + beta, i + 39);
                  STEP_6(8, 40, 40 + alpha, 39 + beta, 33 + gamma, 20 + delta, i + 40);
                  STEP_6(1, 9, 41, 41 + alpha, 40 + beta, 34 + gamma, i + 41);
                  STEP_6(2, 10, 42, 42 + alpha, 41 + beta, 35 + gamma, i + 42);
                  STEP_6(1, 3, 11, 43, 43 + alpha, 42 + beta, i + 43);
                  STEP_6(4, 12, 44, 44 + alpha, 43 + beta, 36 + gamma, i + 44);
                  STEP_6(1, 5, 13, 45, 45 + alpha, 44 + beta, i + 45);
                  STEP_6(2, 6, 14, 46, 46 + alpha, 45 + beta, i + 46);
                  STEP_6(1, 3, 7, 15, 47, 47 + alpha, i + 47);
                  STEP_6(16, 48, 48 + alpha, 46 + beta, 37 + gamma, 21 + delta, i + 48);
                  STEP_6(1, 17, 49, 49 + alpha, 47 + beta, 38 + gamma, i + 49);
                  STEP_6(2, 18, 50, 50 + alpha, 48 + beta, 39 + gamma, i + 50);
                  STEP_6(1, 3, 19, 51, 51 + alpha, 49 + beta, i + 51);
                  STEP_6(4, 20, 52, 52 + alpha, 50 + beta, 40 + gamma, i + 52);
                  STEP_6(1, 5, 21, 53, 53 + alpha, 51 + beta, i + 53);
                  STEP_6(2, 6, 22, 54, 54 + alpha, 52 + beta, i + 54);
                  STEP_6(1, 3, 7, 23, 55, 55 + alpha, i + 55);
                  STEP_6(8, 24, 56, 56 + alpha, 53 + beta, 41 + gamma, i + 56);
                  STEP_6(1, 9, 25, 57, 57 + alpha, 54 + beta, i + 57);
                  STEP_6(2, 10, 26, 58, 58 + alpha, 55 + beta, i + 58);
                  STEP_6(1, 3, 11, 27, 59, 59 + alpha, i + 59);
                  STEP_6(4, 12, 28, 60, 60 + alpha, 56 + beta, i + 60);
                  STEP_6(1, 5, 13, 29, 61, 61 + alpha, i + 61);
                  STEP_6(2, 6, 14, 30, 62, 62 + alpha, i + 62);
                  STEP_6(1, 3, 7, 15, 31, 63, i + 63);
                  STEP_6(64, 63 + alpha, 57 + beta, 42 + gamma, 22 + delta, 7 + epsilon, i + 64);
                  STEP_6(1, 65, 64 + alpha, 58 + beta, 43 + gamma, 23 + delta, i + 65);
                  STEP_6(2, 66, 65 + alpha, 59 + beta, 44 + gamma, 24 + delta, i + 66);
                  STEP_6(1, 3, 67, 66 + alpha, 60 + beta, 45 + gamma, i + 67);
                  STEP_6(4, 68, 67 + alpha, 61 + beta, 46 + gamma, 25 + delta, i + 68);
                  STEP_6(1, 5, 69, 68 + alpha, 62 + beta, 47 + gamma, i + 69);
                  STEP_6(2, 6, 70, 69 + alpha, 63 + beta, 48 + gamma, i + 70);
                  STEP_6(1, 3, 7, 71, 70 + alpha, 64 + beta, i + 71);
                  STEP_6(8, 72, 71 + alpha, 65 + beta, 49 + gamma, 26 + delta, i + 72);
                  STEP_6(1, 9, 73, 72 + alpha, 66 + beta, 50 + gamma, i + 73);
                  STEP_6(2, 10, 74, 73 + alpha, 67 + beta, 51 + gamma, i + 74);
                  STEP_6(1, 3, 11, 75, 74 + alpha, 68 + beta, i + 75);
                  STEP_6(4, 12, 76, 75 + alpha, 69 + beta, 52 + gamma, i + 76);
                  STEP_6(1, 5, 13, 77, 76 + alpha, 70 + beta, i + 77);
                  STEP_6(2, 6, 14, 78, 77 + alpha, 71 + beta, i + 78);
                  STEP_6(1, 3, 7, 15, 79, 78 + alpha, i + 79);
                  STEP_6(16, 80, 79 + alpha, 72 + beta, 53 + gamma, 27 + delta, i + 80);
                  STEP_6(1, 17, 81, 80 + alpha, 73 + beta, 54 + gamma, i + 81);
                  STEP_6(2, 18, 82, 81 + alpha, 74 + beta, 55 + gamma, i + 82);
                  STEP_6(1, 3, 19, 83, 82 + alpha, 75 + beta, i + 83);
                  STEP_6(4, 20, 84, 83 + alpha, 76 + beta, 56 + gamma, i + 84);
                  STEP_6(1, 5, 21, 85, 84 + alpha, 77 + beta, i + 85);
                  STEP_6(2, 6, 22, 86, 85 + alpha, 78 + beta, i + 86);
                  STEP_6(1, 3, 7, 23, 87, 86 + alpha, i + 87);
                  STEP_6(8, 24, 88, 87 + alpha, 79 + beta, 57 + gamma, i + 88);
                  STEP_6(1, 9, 25, 89, 88 + alpha, 80 + beta, i + 89);
                  STEP_6(2, 10, 26, 90, 89 + alpha, 81 + beta, i + 90);
                  STEP_6(1, 3, 11, 27, 91, 90 + alpha, i + 91);
                  STEP_6(4, 12, 28, 92, 91 + alpha, 82 + beta, i + 92);
                  STEP_6(1, 5, 13, 29, 93, 92 + alpha, i + 93);
                  STEP_6(2, 6, 14, 30, 94, 93 + alpha, i + 94);
                  STEP_6(1, 3, 7, 15, 31, 95, i + 95);
                  STEP_6(32, 96, 94 + alpha, 83 + beta, 58 + gamma, 28 + delta, i + 96);
                  STEP_6(1, 33, 97, 95 + alpha, 84 + beta, 59 + gamma, i + 97);
                  STEP_6(2, 34, 98, 96 + alpha, 85 + beta, 60 + gamma, i + 98);
                  STEP_6(1, 3, 35, 99, 97 + alpha, 86 + beta, i + 99);
                  STEP_6(4, 36, 100, 98 + alpha, 87 + beta, 61 + gamma, i + 100);
                  STEP_6(1, 5, 37, 101, 99 + alpha, 88 + beta, i + 101);
                  STEP_6(2, 6, 38, 102, 100 + alpha, 89 + beta, i + 102);
                  STEP_6(1, 3, 7, 39, 103, 101 + alpha, i + 103);
                  STEP_6(8, 40, 104, 102 + alpha, 90 + beta, 62 + gamma, i + 104);
                  STEP_6(1, 9, 41, 105, 103 + alpha, 91 + beta, i + 105);
                  STEP_6(2, 10, 42, 106, 104 + alpha, 92 + beta, i + 106);
                  STEP_6(1, 3, 11, 43, 107, 105 + alpha, i + 107);
                  STEP_6(4, 12, 44, 108, 106 + alpha, 93 + beta, i + 108);
                  STEP_6(1, 5, 13, 45, 109, 107 + alpha, i + 109);
                  STEP_6(2, 6, 14, 46, 110, 108 + alpha, i + 110);
                  STEP_6(1, 3, 7, 15, 47, 111, i + 111);
                  STEP_6(16, 48, 112, 109 + alpha, 94 + beta, 63 + gamma, i + 112);
                  STEP_6(1, 17, 49, 113, 110 + alpha, 95 + beta, i + 113);
                  STEP_6(2, 18, 50, 114, 111 + alpha, 96 + beta, i + 114);
                  STEP_6(1, 3, 19, 51, 115, 112 + alpha, i + 115);
                  STEP_6(4, 20, 52, 116, 113 + alpha, 97 + beta, i + 116);
                  STEP_6(1, 5, 21, 53, 117, 114 + alpha, i + 117);
                  STEP_6(2, 6, 22, 54, 118, 115 + alpha, i + 118);
                  STEP_6(1, 3, 7, 23, 55, 119, i + 119);
                  STEP_6(8, 24, 56, 120, 116 + alpha, 98 + beta, i + 120);
                  STEP_6(1, 9, 25, 57, 121, 117 + alpha, i + 121);
                  STEP_6(2, 10, 26, 58, 122, 118 + alpha, i + 122);
                  STEP_6(1, 3, 11, 27, 59, 123, i + 123);
                  STEP_6(4, 12, 28, 60, 124, 119 + alpha, i + 124);
                  STEP_6(1, 5, 13, 29, 61, 125, i + 125);
                  STEP_6(2, 6, 14, 30, 62, 126, i + 126);
                  STEP_6(1, 3, 7, 15, 31, 63, i + 127);

                  CHECK_SOLUTIONS();
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
