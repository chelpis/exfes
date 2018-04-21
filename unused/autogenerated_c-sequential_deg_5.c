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


typedef struct {
  uint64_t int_idx;
  uint32_t mask;
} solution_t;

// generated with L = 8
void exhaustive_ia32_deg_5(LUT_t LUT, int n, pck_vector_t F[], solution_callback_t callback, void* callback_state, int verbose) {
  #define QUIT() { \
    return; \
  }

  uint64_t init_start_time = rdtsc();
  // computes the derivatives required by the enumeration kernel up to degree 5
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



  if (verbose) printf("fes: initialisation = %" PRIu64 " cycles\n", rdtsc()-init_start_time);
  uint64_t enumeration_start_time = rdtsc();
  uint64_t n_solutions_found = 0;
  uint64_t current_solution_index = 0;
  uint64_t pack_of_solution[65536];
  solution_t solution_buffer[288];

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

            // we are now inside the critical part where the hamming weight is known to be >= 5
            // Thus, there are no special cases from now on

            // Because of the last step, the current iteration counter is a multiple of 256 plus one
            // This loop sets it to `rolled_end`, which is a multiple of 256, if possible

          const uint64_t rolled_end = weight_4_start + (1ll << min(8, idx_3));
          for(uint64_t i=1 + weight_4_start; i< rolled_end; i++) {

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
              STEP_5( idx_1(LUT, k_1), idx_2(LUT, k_1, k_2), idx_3(LUT, k_1, k_2, k_3), idx_4(LUT, k_1, k_2, k_3, k_4), idx_5(LUT, k_1, k_2, k_3, k_4, k_5), i );
          }

          CHECK_SOLUTIONS();

          // Here, the number of iterations to perform is (supposedly) sufficiently large
          // We will therefore unroll the loop 256 times

          // unrolled critical section where the hamming weight is >= 5
          for(uint64_t j=256; j<(1ull << idx_3); j+=256) {
                const uint64_t i = j + weight_4_start;
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
                const int alpha = LUT[0][k_1];
                const int beta = LUT[1][k_1]+LUT[0][k_2];
                const int gamma = LUT[2][k_1]+LUT[1][k_2]+LUT[0][k_3];
                const int delta = LUT[3][k_1]+LUT[2][k_2]+LUT[1][k_3]+LUT[0][k_4];
                const int epsilon = LUT[4][k_1]+LUT[3][k_2]+LUT[2][k_3]+LUT[1][k_4]+LUT[0][k_5];
                STEP_5(0 + alpha, 0 + beta, 0 + gamma, 0 + delta, 0 + epsilon, i + 0);
                STEP_5(1, 1 + alpha, 1 + beta, 1 + gamma, 1 + delta, i + 1);
                STEP_5(2, 2 + alpha, 2 + beta, 2 + gamma, 2 + delta, i + 2);
                STEP_5(1, 3, 3 + alpha, 3 + beta, 3 + gamma, i + 3);
                STEP_5(4, 4 + alpha, 4 + beta, 4 + gamma, 3 + delta, i + 4);
                STEP_5(1, 5, 5 + alpha, 5 + beta, 5 + gamma, i + 5);
                STEP_5(2, 6, 6 + alpha, 6 + beta, 6 + gamma, i + 6);
                STEP_5(1, 3, 7, 7 + alpha, 7 + beta, i + 7);
                STEP_5(8, 8 + alpha, 8 + beta, 7 + gamma, 4 + delta, i + 8);
                STEP_5(1, 9, 9 + alpha, 9 + beta, 8 + gamma, i + 9);
                STEP_5(2, 10, 10 + alpha, 10 + beta, 9 + gamma, i + 10);
                STEP_5(1, 3, 11, 11 + alpha, 11 + beta, i + 11);
                STEP_5(4, 12, 12 + alpha, 12 + beta, 10 + gamma, i + 12);
                STEP_5(1, 5, 13, 13 + alpha, 13 + beta, i + 13);
                STEP_5(2, 6, 14, 14 + alpha, 14 + beta, i + 14);
                STEP_5(1, 3, 7, 15, 15 + alpha, i + 15);
                STEP_5(16, 16 + alpha, 15 + beta, 11 + gamma, 5 + delta, i + 16);
                STEP_5(1, 17, 17 + alpha, 16 + beta, 12 + gamma, i + 17);
                STEP_5(2, 18, 18 + alpha, 17 + beta, 13 + gamma, i + 18);
                STEP_5(1, 3, 19, 19 + alpha, 18 + beta, i + 19);
                STEP_5(4, 20, 20 + alpha, 19 + beta, 14 + gamma, i + 20);
                STEP_5(1, 5, 21, 21 + alpha, 20 + beta, i + 21);
                STEP_5(2, 6, 22, 22 + alpha, 21 + beta, i + 22);
                STEP_5(1, 3, 7, 23, 23 + alpha, i + 23);
                STEP_5(8, 24, 24 + alpha, 22 + beta, 15 + gamma, i + 24);
                STEP_5(1, 9, 25, 25 + alpha, 23 + beta, i + 25);
                STEP_5(2, 10, 26, 26 + alpha, 24 + beta, i + 26);
                STEP_5(1, 3, 11, 27, 27 + alpha, i + 27);
                STEP_5(4, 12, 28, 28 + alpha, 25 + beta, i + 28);
                STEP_5(1, 5, 13, 29, 29 + alpha, i + 29);
                STEP_5(2, 6, 14, 30, 30 + alpha, i + 30);
                STEP_5(1, 3, 7, 15, 31, i + 31);
                STEP_5(32, 31 + alpha, 26 + beta, 16 + gamma, 6 + delta, i + 32);
                STEP_5(1, 33, 32 + alpha, 27 + beta, 17 + gamma, i + 33);
                STEP_5(2, 34, 33 + alpha, 28 + beta, 18 + gamma, i + 34);
                STEP_5(1, 3, 35, 34 + alpha, 29 + beta, i + 35);
                STEP_5(4, 36, 35 + alpha, 30 + beta, 19 + gamma, i + 36);
                STEP_5(1, 5, 37, 36 + alpha, 31 + beta, i + 37);
                STEP_5(2, 6, 38, 37 + alpha, 32 + beta, i + 38);
                STEP_5(1, 3, 7, 39, 38 + alpha, i + 39);
                STEP_5(8, 40, 39 + alpha, 33 + beta, 20 + gamma, i + 40);
                STEP_5(1, 9, 41, 40 + alpha, 34 + beta, i + 41);
                STEP_5(2, 10, 42, 41 + alpha, 35 + beta, i + 42);
                STEP_5(1, 3, 11, 43, 42 + alpha, i + 43);
                STEP_5(4, 12, 44, 43 + alpha, 36 + beta, i + 44);
                STEP_5(1, 5, 13, 45, 44 + alpha, i + 45);
                STEP_5(2, 6, 14, 46, 45 + alpha, i + 46);
                STEP_5(1, 3, 7, 15, 47, i + 47);
                STEP_5(16, 48, 46 + alpha, 37 + beta, 21 + gamma, i + 48);
                STEP_5(1, 17, 49, 47 + alpha, 38 + beta, i + 49);
                STEP_5(2, 18, 50, 48 + alpha, 39 + beta, i + 50);
                STEP_5(1, 3, 19, 51, 49 + alpha, i + 51);
                STEP_5(4, 20, 52, 50 + alpha, 40 + beta, i + 52);
                STEP_5(1, 5, 21, 53, 51 + alpha, i + 53);
                STEP_5(2, 6, 22, 54, 52 + alpha, i + 54);
                STEP_5(1, 3, 7, 23, 55, i + 55);
                STEP_5(8, 24, 56, 53 + alpha, 41 + beta, i + 56);
                STEP_5(1, 9, 25, 57, 54 + alpha, i + 57);
                STEP_5(2, 10, 26, 58, 55 + alpha, i + 58);
                STEP_5(1, 3, 11, 27, 59, i + 59);
                STEP_5(4, 12, 28, 60, 56 + alpha, i + 60);
                STEP_5(1, 5, 13, 29, 61, i + 61);
                STEP_5(2, 6, 14, 30, 62, i + 62);
                STEP_5(1, 3, 7, 15, 31, i + 63);
                STEP_5(63, 57 + alpha, 42 + beta, 22 + gamma, 7 + delta, i + 64);
                STEP_5(1, 64, 58 + alpha, 43 + beta, 23 + gamma, i + 65);
                STEP_5(2, 65, 59 + alpha, 44 + beta, 24 + gamma, i + 66);
                STEP_5(1, 3, 66, 60 + alpha, 45 + beta, i + 67);
                STEP_5(4, 67, 61 + alpha, 46 + beta, 25 + gamma, i + 68);
                STEP_5(1, 5, 68, 62 + alpha, 47 + beta, i + 69);
                STEP_5(2, 6, 69, 63 + alpha, 48 + beta, i + 70);
                STEP_5(1, 3, 7, 70, 64 + alpha, i + 71);
                STEP_5(8, 71, 65 + alpha, 49 + beta, 26 + gamma, i + 72);
                STEP_5(1, 9, 72, 66 + alpha, 50 + beta, i + 73);
                STEP_5(2, 10, 73, 67 + alpha, 51 + beta, i + 74);
                STEP_5(1, 3, 11, 74, 68 + alpha, i + 75);
                STEP_5(4, 12, 75, 69 + alpha, 52 + beta, i + 76);
                STEP_5(1, 5, 13, 76, 70 + alpha, i + 77);
                STEP_5(2, 6, 14, 77, 71 + alpha, i + 78);
                STEP_5(1, 3, 7, 15, 78, i + 79);
                STEP_5(16, 79, 72 + alpha, 53 + beta, 27 + gamma, i + 80);
                STEP_5(1, 17, 80, 73 + alpha, 54 + beta, i + 81);
                STEP_5(2, 18, 81, 74 + alpha, 55 + beta, i + 82);
                STEP_5(1, 3, 19, 82, 75 + alpha, i + 83);
                STEP_5(4, 20, 83, 76 + alpha, 56 + beta, i + 84);
                STEP_5(1, 5, 21, 84, 77 + alpha, i + 85);
                STEP_5(2, 6, 22, 85, 78 + alpha, i + 86);
                STEP_5(1, 3, 7, 23, 86, i + 87);
                STEP_5(8, 24, 87, 79 + alpha, 57 + beta, i + 88);
                STEP_5(1, 9, 25, 88, 80 + alpha, i + 89);
                STEP_5(2, 10, 26, 89, 81 + alpha, i + 90);
                STEP_5(1, 3, 11, 27, 90, i + 91);
                STEP_5(4, 12, 28, 91, 82 + alpha, i + 92);
                STEP_5(1, 5, 13, 29, 92, i + 93);
                STEP_5(2, 6, 14, 30, 93, i + 94);
                STEP_5(1, 3, 7, 15, 31, i + 95);
                STEP_5(32, 94, 83 + alpha, 58 + beta, 28 + gamma, i + 96);
                STEP_5(1, 33, 95, 84 + alpha, 59 + beta, i + 97);
                STEP_5(2, 34, 96, 85 + alpha, 60 + beta, i + 98);
                STEP_5(1, 3, 35, 97, 86 + alpha, i + 99);
                STEP_5(4, 36, 98, 87 + alpha, 61 + beta, i + 100);
                STEP_5(1, 5, 37, 99, 88 + alpha, i + 101);
                STEP_5(2, 6, 38, 100, 89 + alpha, i + 102);
                STEP_5(1, 3, 7, 39, 101, i + 103);
                STEP_5(8, 40, 102, 90 + alpha, 62 + beta, i + 104);
                STEP_5(1, 9, 41, 103, 91 + alpha, i + 105);
                STEP_5(2, 10, 42, 104, 92 + alpha, i + 106);
                STEP_5(1, 3, 11, 43, 105, i + 107);
                STEP_5(4, 12, 44, 106, 93 + alpha, i + 108);
                STEP_5(1, 5, 13, 45, 107, i + 109);
                STEP_5(2, 6, 14, 46, 108, i + 110);
                STEP_5(1, 3, 7, 15, 47, i + 111);
                STEP_5(16, 48, 109, 94 + alpha, 63 + beta, i + 112);
                STEP_5(1, 17, 49, 110, 95 + alpha, i + 113);
                STEP_5(2, 18, 50, 111, 96 + alpha, i + 114);
                STEP_5(1, 3, 19, 51, 112, i + 115);
                STEP_5(4, 20, 52, 113, 97 + alpha, i + 116);
                STEP_5(1, 5, 21, 53, 114, i + 117);
                STEP_5(2, 6, 22, 54, 115, i + 118);
                STEP_5(1, 3, 7, 23, 55, i + 119);
                STEP_5(8, 24, 56, 116, 98 + alpha, i + 120);
                STEP_5(1, 9, 25, 57, 117, i + 121);
                STEP_5(2, 10, 26, 58, 118, i + 122);
                STEP_5(1, 3, 11, 27, 59, i + 123);
                STEP_5(4, 12, 28, 60, 119, i + 124);
                STEP_5(1, 5, 13, 29, 61, i + 125);
                STEP_5(2, 6, 14, 30, 62, i + 126);
                STEP_5(1, 3, 7, 15, 31, i + 127);
                STEP_5(120, 99 + alpha, 64 + beta, 29 + gamma, 8 + delta, i + 128);
                STEP_5(1, 121, 100 + alpha, 65 + beta, 30 + gamma, i + 129);
                STEP_5(2, 122, 101 + alpha, 66 + beta, 31 + gamma, i + 130);
                STEP_5(1, 3, 123, 102 + alpha, 67 + beta, i + 131);
                STEP_5(4, 124, 103 + alpha, 68 + beta, 32 + gamma, i + 132);
                STEP_5(1, 5, 125, 104 + alpha, 69 + beta, i + 133);
                STEP_5(2, 6, 126, 105 + alpha, 70 + beta, i + 134);
                STEP_5(1, 3, 7, 127, 106 + alpha, i + 135);
                STEP_5(8, 128, 107 + alpha, 71 + beta, 33 + gamma, i + 136);
                STEP_5(1, 9, 129, 108 + alpha, 72 + beta, i + 137);
                STEP_5(2, 10, 130, 109 + alpha, 73 + beta, i + 138);
                STEP_5(1, 3, 11, 131, 110 + alpha, i + 139);
                STEP_5(4, 12, 132, 111 + alpha, 74 + beta, i + 140);
                STEP_5(1, 5, 13, 133, 112 + alpha, i + 141);
                STEP_5(2, 6, 14, 134, 113 + alpha, i + 142);
                STEP_5(1, 3, 7, 15, 135, i + 143);
                STEP_5(16, 136, 114 + alpha, 75 + beta, 34 + gamma, i + 144);
                STEP_5(1, 17, 137, 115 + alpha, 76 + beta, i + 145);
                STEP_5(2, 18, 138, 116 + alpha, 77 + beta, i + 146);
                STEP_5(1, 3, 19, 139, 117 + alpha, i + 147);
                STEP_5(4, 20, 140, 118 + alpha, 78 + beta, i + 148);
                STEP_5(1, 5, 21, 141, 119 + alpha, i + 149);
                STEP_5(2, 6, 22, 142, 120 + alpha, i + 150);
                STEP_5(1, 3, 7, 23, 143, i + 151);
                STEP_5(8, 24, 144, 121 + alpha, 79 + beta, i + 152);
                STEP_5(1, 9, 25, 145, 122 + alpha, i + 153);
                STEP_5(2, 10, 26, 146, 123 + alpha, i + 154);
                STEP_5(1, 3, 11, 27, 147, i + 155);
                STEP_5(4, 12, 28, 148, 124 + alpha, i + 156);
                STEP_5(1, 5, 13, 29, 149, i + 157);
                STEP_5(2, 6, 14, 30, 150, i + 158);
                STEP_5(1, 3, 7, 15, 31, i + 159);
                STEP_5(32, 151, 125 + alpha, 80 + beta, 35 + gamma, i + 160);
                STEP_5(1, 33, 152, 126 + alpha, 81 + beta, i + 161);
                STEP_5(2, 34, 153, 127 + alpha, 82 + beta, i + 162);
                STEP_5(1, 3, 35, 154, 128 + alpha, i + 163);
                STEP_5(4, 36, 155, 129 + alpha, 83 + beta, i + 164);
                STEP_5(1, 5, 37, 156, 130 + alpha, i + 165);
                STEP_5(2, 6, 38, 157, 131 + alpha, i + 166);
                STEP_5(1, 3, 7, 39, 158, i + 167);
                STEP_5(8, 40, 159, 132 + alpha, 84 + beta, i + 168);
                STEP_5(1, 9, 41, 160, 133 + alpha, i + 169);
                STEP_5(2, 10, 42, 161, 134 + alpha, i + 170);
                STEP_5(1, 3, 11, 43, 162, i + 171);
                STEP_5(4, 12, 44, 163, 135 + alpha, i + 172);
                STEP_5(1, 5, 13, 45, 164, i + 173);
                STEP_5(2, 6, 14, 46, 165, i + 174);
                STEP_5(1, 3, 7, 15, 47, i + 175);
                STEP_5(16, 48, 166, 136 + alpha, 85 + beta, i + 176);
                STEP_5(1, 17, 49, 167, 137 + alpha, i + 177);
                STEP_5(2, 18, 50, 168, 138 + alpha, i + 178);
                STEP_5(1, 3, 19, 51, 169, i + 179);
                STEP_5(4, 20, 52, 170, 139 + alpha, i + 180);
                STEP_5(1, 5, 21, 53, 171, i + 181);
                STEP_5(2, 6, 22, 54, 172, i + 182);
                STEP_5(1, 3, 7, 23, 55, i + 183);
                STEP_5(8, 24, 56, 173, 140 + alpha, i + 184);
                STEP_5(1, 9, 25, 57, 174, i + 185);
                STEP_5(2, 10, 26, 58, 175, i + 186);
                STEP_5(1, 3, 11, 27, 59, i + 187);
                STEP_5(4, 12, 28, 60, 176, i + 188);
                STEP_5(1, 5, 13, 29, 61, i + 189);
                STEP_5(2, 6, 14, 30, 62, i + 190);
                STEP_5(1, 3, 7, 15, 31, i + 191);
                STEP_5(63, 177, 141 + alpha, 86 + beta, 36 + gamma, i + 192);
                STEP_5(1, 64, 178, 142 + alpha, 87 + beta, i + 193);
                STEP_5(2, 65, 179, 143 + alpha, 88 + beta, i + 194);
                STEP_5(1, 3, 66, 180, 144 + alpha, i + 195);
                STEP_5(4, 67, 181, 145 + alpha, 89 + beta, i + 196);
                STEP_5(1, 5, 68, 182, 146 + alpha, i + 197);
                STEP_5(2, 6, 69, 183, 147 + alpha, i + 198);
                STEP_5(1, 3, 7, 70, 184, i + 199);
                STEP_5(8, 71, 185, 148 + alpha, 90 + beta, i + 200);
                STEP_5(1, 9, 72, 186, 149 + alpha, i + 201);
                STEP_5(2, 10, 73, 187, 150 + alpha, i + 202);
                STEP_5(1, 3, 11, 74, 188, i + 203);
                STEP_5(4, 12, 75, 189, 151 + alpha, i + 204);
                STEP_5(1, 5, 13, 76, 190, i + 205);
                STEP_5(2, 6, 14, 77, 191, i + 206);
                STEP_5(1, 3, 7, 15, 78, i + 207);
                STEP_5(16, 79, 192, 152 + alpha, 91 + beta, i + 208);
                STEP_5(1, 17, 80, 193, 153 + alpha, i + 209);
                STEP_5(2, 18, 81, 194, 154 + alpha, i + 210);
                STEP_5(1, 3, 19, 82, 195, i + 211);
                STEP_5(4, 20, 83, 196, 155 + alpha, i + 212);
                STEP_5(1, 5, 21, 84, 197, i + 213);
                STEP_5(2, 6, 22, 85, 198, i + 214);
                STEP_5(1, 3, 7, 23, 86, i + 215);
                STEP_5(8, 24, 87, 199, 156 + alpha, i + 216);
                STEP_5(1, 9, 25, 88, 200, i + 217);
                STEP_5(2, 10, 26, 89, 201, i + 218);
                STEP_5(1, 3, 11, 27, 90, i + 219);
                STEP_5(4, 12, 28, 91, 202, i + 220);
                STEP_5(1, 5, 13, 29, 92, i + 221);
                STEP_5(2, 6, 14, 30, 93, i + 222);
                STEP_5(1, 3, 7, 15, 31, i + 223);
                STEP_5(32, 94, 203, 157 + alpha, 92 + beta, i + 224);
                STEP_5(1, 33, 95, 204, 158 + alpha, i + 225);
                STEP_5(2, 34, 96, 205, 159 + alpha, i + 226);
                STEP_5(1, 3, 35, 97, 206, i + 227);
                STEP_5(4, 36, 98, 207, 160 + alpha, i + 228);
                STEP_5(1, 5, 37, 99, 208, i + 229);
                STEP_5(2, 6, 38, 100, 209, i + 230);
                STEP_5(1, 3, 7, 39, 101, i + 231);
                STEP_5(8, 40, 102, 210, 161 + alpha, i + 232);
                STEP_5(1, 9, 41, 103, 211, i + 233);
                STEP_5(2, 10, 42, 104, 212, i + 234);
                STEP_5(1, 3, 11, 43, 105, i + 235);
                STEP_5(4, 12, 44, 106, 213, i + 236);
                STEP_5(1, 5, 13, 45, 107, i + 237);
                STEP_5(2, 6, 14, 46, 108, i + 238);
                STEP_5(1, 3, 7, 15, 47, i + 239);
                STEP_5(16, 48, 109, 214, 162 + alpha, i + 240);
                STEP_5(1, 17, 49, 110, 215, i + 241);
                STEP_5(2, 18, 50, 111, 216, i + 242);
                STEP_5(1, 3, 19, 51, 112, i + 243);
                STEP_5(4, 20, 52, 113, 217, i + 244);
                STEP_5(1, 5, 21, 53, 114, i + 245);
                STEP_5(2, 6, 22, 54, 115, i + 246);
                STEP_5(1, 3, 7, 23, 55, i + 247);
                STEP_5(8, 24, 56, 116, 218, i + 248);
                STEP_5(1, 9, 25, 57, 117, i + 249);
                STEP_5(2, 10, 26, 58, 118, i + 250);
                STEP_5(1, 3, 11, 27, 59, i + 251);
                STEP_5(4, 12, 28, 60, 119, i + 252);
                STEP_5(1, 5, 13, 29, 61, i + 253);
                STEP_5(2, 6, 14, 30, 62, i + 254);
                STEP_5(1, 3, 7, 15, 31, i + 255);

                CHECK_SOLUTIONS();
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
