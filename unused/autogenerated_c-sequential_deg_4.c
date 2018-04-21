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


typedef struct {
  uint64_t int_idx;
  uint32_t mask;
} solution_t;

// generated with L = 8
void exhaustive_ia32_deg_4(LUT_t LUT, int n, pck_vector_t F[], solution_callback_t callback, void* callback_state, int verbose) {
  #define QUIT() { \
    return; \
  }

  uint64_t init_start_time = rdtsc();
  // computes the derivatives required by the enumeration kernel up to degree 4
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



  if (verbose) printf("fes: initialisation = %" PRIu64 " cycles\n", rdtsc()-init_start_time);
  uint64_t enumeration_start_time = rdtsc();
  uint64_t n_solutions_found = 0;
  uint64_t current_solution_index = 0;
  uint64_t pack_of_solution[65536];
  solution_t solution_buffer[272];

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

          // we are now inside the critical part where the hamming weight is known to be >= 4
          // Thus, there are no special cases from now on

          // Because of the last step, the current iteration counter is a multiple of 256 plus one
          // This loop sets it to `rolled_end`, which is a multiple of 256, if possible

        const uint64_t rolled_end = weight_3_start + (1ll << min(8, idx_2));
        for(uint64_t i=1 + weight_3_start; i< rolled_end; i++) {

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
            STEP_4( idx_1(LUT, k_1), idx_2(LUT, k_1, k_2), idx_3(LUT, k_1, k_2, k_3), idx_4(LUT, k_1, k_2, k_3, k_4), i );
        }

        CHECK_SOLUTIONS();

        // Here, the number of iterations to perform is (supposedly) sufficiently large
        // We will therefore unroll the loop 256 times

        // unrolled critical section where the hamming weight is >= 4
        for(uint64_t j=256; j<(1ull << idx_2); j+=256) {
              const uint64_t i = j + weight_3_start;
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
              const int alpha = LUT[0][k_1];
              const int beta = LUT[1][k_1]+LUT[0][k_2];
              const int gamma = LUT[2][k_1]+LUT[1][k_2]+LUT[0][k_3];
              const int delta = LUT[3][k_1]+LUT[2][k_2]+LUT[1][k_3]+LUT[0][k_4];
              STEP_4(0 + alpha, 0 + beta, 0 + gamma, 0 + delta, i + 0);
              STEP_4(1, 1 + alpha, 1 + beta, 1 + gamma, i + 1);
              STEP_4(2, 2 + alpha, 2 + beta, 2 + gamma, i + 2);
              STEP_4(1, 3, 3 + alpha, 3 + beta, i + 3);
              STEP_4(4, 4 + alpha, 4 + beta, 3 + gamma, i + 4);
              STEP_4(1, 5, 5 + alpha, 5 + beta, i + 5);
              STEP_4(2, 6, 6 + alpha, 6 + beta, i + 6);
              STEP_4(1, 3, 7, 7 + alpha, i + 7);
              STEP_4(8, 8 + alpha, 7 + beta, 4 + gamma, i + 8);
              STEP_4(1, 9, 9 + alpha, 8 + beta, i + 9);
              STEP_4(2, 10, 10 + alpha, 9 + beta, i + 10);
              STEP_4(1, 3, 11, 11 + alpha, i + 11);
              STEP_4(4, 12, 12 + alpha, 10 + beta, i + 12);
              STEP_4(1, 5, 13, 13 + alpha, i + 13);
              STEP_4(2, 6, 14, 14 + alpha, i + 14);
              STEP_4(1, 3, 7, 15, i + 15);
              STEP_4(16, 15 + alpha, 11 + beta, 5 + gamma, i + 16);
              STEP_4(1, 17, 16 + alpha, 12 + beta, i + 17);
              STEP_4(2, 18, 17 + alpha, 13 + beta, i + 18);
              STEP_4(1, 3, 19, 18 + alpha, i + 19);
              STEP_4(4, 20, 19 + alpha, 14 + beta, i + 20);
              STEP_4(1, 5, 21, 20 + alpha, i + 21);
              STEP_4(2, 6, 22, 21 + alpha, i + 22);
              STEP_4(1, 3, 7, 23, i + 23);
              STEP_4(8, 24, 22 + alpha, 15 + beta, i + 24);
              STEP_4(1, 9, 25, 23 + alpha, i + 25);
              STEP_4(2, 10, 26, 24 + alpha, i + 26);
              STEP_4(1, 3, 11, 27, i + 27);
              STEP_4(4, 12, 28, 25 + alpha, i + 28);
              STEP_4(1, 5, 13, 29, i + 29);
              STEP_4(2, 6, 14, 30, i + 30);
              STEP_4(1, 3, 7, 15, i + 31);
              STEP_4(31, 26 + alpha, 16 + beta, 6 + gamma, i + 32);
              STEP_4(1, 32, 27 + alpha, 17 + beta, i + 33);
              STEP_4(2, 33, 28 + alpha, 18 + beta, i + 34);
              STEP_4(1, 3, 34, 29 + alpha, i + 35);
              STEP_4(4, 35, 30 + alpha, 19 + beta, i + 36);
              STEP_4(1, 5, 36, 31 + alpha, i + 37);
              STEP_4(2, 6, 37, 32 + alpha, i + 38);
              STEP_4(1, 3, 7, 38, i + 39);
              STEP_4(8, 39, 33 + alpha, 20 + beta, i + 40);
              STEP_4(1, 9, 40, 34 + alpha, i + 41);
              STEP_4(2, 10, 41, 35 + alpha, i + 42);
              STEP_4(1, 3, 11, 42, i + 43);
              STEP_4(4, 12, 43, 36 + alpha, i + 44);
              STEP_4(1, 5, 13, 44, i + 45);
              STEP_4(2, 6, 14, 45, i + 46);
              STEP_4(1, 3, 7, 15, i + 47);
              STEP_4(16, 46, 37 + alpha, 21 + beta, i + 48);
              STEP_4(1, 17, 47, 38 + alpha, i + 49);
              STEP_4(2, 18, 48, 39 + alpha, i + 50);
              STEP_4(1, 3, 19, 49, i + 51);
              STEP_4(4, 20, 50, 40 + alpha, i + 52);
              STEP_4(1, 5, 21, 51, i + 53);
              STEP_4(2, 6, 22, 52, i + 54);
              STEP_4(1, 3, 7, 23, i + 55);
              STEP_4(8, 24, 53, 41 + alpha, i + 56);
              STEP_4(1, 9, 25, 54, i + 57);
              STEP_4(2, 10, 26, 55, i + 58);
              STEP_4(1, 3, 11, 27, i + 59);
              STEP_4(4, 12, 28, 56, i + 60);
              STEP_4(1, 5, 13, 29, i + 61);
              STEP_4(2, 6, 14, 30, i + 62);
              STEP_4(1, 3, 7, 15, i + 63);
              STEP_4(57, 42 + alpha, 22 + beta, 7 + gamma, i + 64);
              STEP_4(1, 58, 43 + alpha, 23 + beta, i + 65);
              STEP_4(2, 59, 44 + alpha, 24 + beta, i + 66);
              STEP_4(1, 3, 60, 45 + alpha, i + 67);
              STEP_4(4, 61, 46 + alpha, 25 + beta, i + 68);
              STEP_4(1, 5, 62, 47 + alpha, i + 69);
              STEP_4(2, 6, 63, 48 + alpha, i + 70);
              STEP_4(1, 3, 7, 64, i + 71);
              STEP_4(8, 65, 49 + alpha, 26 + beta, i + 72);
              STEP_4(1, 9, 66, 50 + alpha, i + 73);
              STEP_4(2, 10, 67, 51 + alpha, i + 74);
              STEP_4(1, 3, 11, 68, i + 75);
              STEP_4(4, 12, 69, 52 + alpha, i + 76);
              STEP_4(1, 5, 13, 70, i + 77);
              STEP_4(2, 6, 14, 71, i + 78);
              STEP_4(1, 3, 7, 15, i + 79);
              STEP_4(16, 72, 53 + alpha, 27 + beta, i + 80);
              STEP_4(1, 17, 73, 54 + alpha, i + 81);
              STEP_4(2, 18, 74, 55 + alpha, i + 82);
              STEP_4(1, 3, 19, 75, i + 83);
              STEP_4(4, 20, 76, 56 + alpha, i + 84);
              STEP_4(1, 5, 21, 77, i + 85);
              STEP_4(2, 6, 22, 78, i + 86);
              STEP_4(1, 3, 7, 23, i + 87);
              STEP_4(8, 24, 79, 57 + alpha, i + 88);
              STEP_4(1, 9, 25, 80, i + 89);
              STEP_4(2, 10, 26, 81, i + 90);
              STEP_4(1, 3, 11, 27, i + 91);
              STEP_4(4, 12, 28, 82, i + 92);
              STEP_4(1, 5, 13, 29, i + 93);
              STEP_4(2, 6, 14, 30, i + 94);
              STEP_4(1, 3, 7, 15, i + 95);
              STEP_4(31, 83, 58 + alpha, 28 + beta, i + 96);
              STEP_4(1, 32, 84, 59 + alpha, i + 97);
              STEP_4(2, 33, 85, 60 + alpha, i + 98);
              STEP_4(1, 3, 34, 86, i + 99);
              STEP_4(4, 35, 87, 61 + alpha, i + 100);
              STEP_4(1, 5, 36, 88, i + 101);
              STEP_4(2, 6, 37, 89, i + 102);
              STEP_4(1, 3, 7, 38, i + 103);
              STEP_4(8, 39, 90, 62 + alpha, i + 104);
              STEP_4(1, 9, 40, 91, i + 105);
              STEP_4(2, 10, 41, 92, i + 106);
              STEP_4(1, 3, 11, 42, i + 107);
              STEP_4(4, 12, 43, 93, i + 108);
              STEP_4(1, 5, 13, 44, i + 109);
              STEP_4(2, 6, 14, 45, i + 110);
              STEP_4(1, 3, 7, 15, i + 111);
              STEP_4(16, 46, 94, 63 + alpha, i + 112);
              STEP_4(1, 17, 47, 95, i + 113);
              STEP_4(2, 18, 48, 96, i + 114);
              STEP_4(1, 3, 19, 49, i + 115);
              STEP_4(4, 20, 50, 97, i + 116);
              STEP_4(1, 5, 21, 51, i + 117);
              STEP_4(2, 6, 22, 52, i + 118);
              STEP_4(1, 3, 7, 23, i + 119);
              STEP_4(8, 24, 53, 98, i + 120);
              STEP_4(1, 9, 25, 54, i + 121);
              STEP_4(2, 10, 26, 55, i + 122);
              STEP_4(1, 3, 11, 27, i + 123);
              STEP_4(4, 12, 28, 56, i + 124);
              STEP_4(1, 5, 13, 29, i + 125);
              STEP_4(2, 6, 14, 30, i + 126);
              STEP_4(1, 3, 7, 15, i + 127);
              STEP_4(99, 64 + alpha, 29 + beta, 8 + gamma, i + 128);
              STEP_4(1, 100, 65 + alpha, 30 + beta, i + 129);
              STEP_4(2, 101, 66 + alpha, 31 + beta, i + 130);
              STEP_4(1, 3, 102, 67 + alpha, i + 131);
              STEP_4(4, 103, 68 + alpha, 32 + beta, i + 132);
              STEP_4(1, 5, 104, 69 + alpha, i + 133);
              STEP_4(2, 6, 105, 70 + alpha, i + 134);
              STEP_4(1, 3, 7, 106, i + 135);
              STEP_4(8, 107, 71 + alpha, 33 + beta, i + 136);
              STEP_4(1, 9, 108, 72 + alpha, i + 137);
              STEP_4(2, 10, 109, 73 + alpha, i + 138);
              STEP_4(1, 3, 11, 110, i + 139);
              STEP_4(4, 12, 111, 74 + alpha, i + 140);
              STEP_4(1, 5, 13, 112, i + 141);
              STEP_4(2, 6, 14, 113, i + 142);
              STEP_4(1, 3, 7, 15, i + 143);
              STEP_4(16, 114, 75 + alpha, 34 + beta, i + 144);
              STEP_4(1, 17, 115, 76 + alpha, i + 145);
              STEP_4(2, 18, 116, 77 + alpha, i + 146);
              STEP_4(1, 3, 19, 117, i + 147);
              STEP_4(4, 20, 118, 78 + alpha, i + 148);
              STEP_4(1, 5, 21, 119, i + 149);
              STEP_4(2, 6, 22, 120, i + 150);
              STEP_4(1, 3, 7, 23, i + 151);
              STEP_4(8, 24, 121, 79 + alpha, i + 152);
              STEP_4(1, 9, 25, 122, i + 153);
              STEP_4(2, 10, 26, 123, i + 154);
              STEP_4(1, 3, 11, 27, i + 155);
              STEP_4(4, 12, 28, 124, i + 156);
              STEP_4(1, 5, 13, 29, i + 157);
              STEP_4(2, 6, 14, 30, i + 158);
              STEP_4(1, 3, 7, 15, i + 159);
              STEP_4(31, 125, 80 + alpha, 35 + beta, i + 160);
              STEP_4(1, 32, 126, 81 + alpha, i + 161);
              STEP_4(2, 33, 127, 82 + alpha, i + 162);
              STEP_4(1, 3, 34, 128, i + 163);
              STEP_4(4, 35, 129, 83 + alpha, i + 164);
              STEP_4(1, 5, 36, 130, i + 165);
              STEP_4(2, 6, 37, 131, i + 166);
              STEP_4(1, 3, 7, 38, i + 167);
              STEP_4(8, 39, 132, 84 + alpha, i + 168);
              STEP_4(1, 9, 40, 133, i + 169);
              STEP_4(2, 10, 41, 134, i + 170);
              STEP_4(1, 3, 11, 42, i + 171);
              STEP_4(4, 12, 43, 135, i + 172);
              STEP_4(1, 5, 13, 44, i + 173);
              STEP_4(2, 6, 14, 45, i + 174);
              STEP_4(1, 3, 7, 15, i + 175);
              STEP_4(16, 46, 136, 85 + alpha, i + 176);
              STEP_4(1, 17, 47, 137, i + 177);
              STEP_4(2, 18, 48, 138, i + 178);
              STEP_4(1, 3, 19, 49, i + 179);
              STEP_4(4, 20, 50, 139, i + 180);
              STEP_4(1, 5, 21, 51, i + 181);
              STEP_4(2, 6, 22, 52, i + 182);
              STEP_4(1, 3, 7, 23, i + 183);
              STEP_4(8, 24, 53, 140, i + 184);
              STEP_4(1, 9, 25, 54, i + 185);
              STEP_4(2, 10, 26, 55, i + 186);
              STEP_4(1, 3, 11, 27, i + 187);
              STEP_4(4, 12, 28, 56, i + 188);
              STEP_4(1, 5, 13, 29, i + 189);
              STEP_4(2, 6, 14, 30, i + 190);
              STEP_4(1, 3, 7, 15, i + 191);
              STEP_4(57, 141, 86 + alpha, 36 + beta, i + 192);
              STEP_4(1, 58, 142, 87 + alpha, i + 193);
              STEP_4(2, 59, 143, 88 + alpha, i + 194);
              STEP_4(1, 3, 60, 144, i + 195);
              STEP_4(4, 61, 145, 89 + alpha, i + 196);
              STEP_4(1, 5, 62, 146, i + 197);
              STEP_4(2, 6, 63, 147, i + 198);
              STEP_4(1, 3, 7, 64, i + 199);
              STEP_4(8, 65, 148, 90 + alpha, i + 200);
              STEP_4(1, 9, 66, 149, i + 201);
              STEP_4(2, 10, 67, 150, i + 202);
              STEP_4(1, 3, 11, 68, i + 203);
              STEP_4(4, 12, 69, 151, i + 204);
              STEP_4(1, 5, 13, 70, i + 205);
              STEP_4(2, 6, 14, 71, i + 206);
              STEP_4(1, 3, 7, 15, i + 207);
              STEP_4(16, 72, 152, 91 + alpha, i + 208);
              STEP_4(1, 17, 73, 153, i + 209);
              STEP_4(2, 18, 74, 154, i + 210);
              STEP_4(1, 3, 19, 75, i + 211);
              STEP_4(4, 20, 76, 155, i + 212);
              STEP_4(1, 5, 21, 77, i + 213);
              STEP_4(2, 6, 22, 78, i + 214);
              STEP_4(1, 3, 7, 23, i + 215);
              STEP_4(8, 24, 79, 156, i + 216);
              STEP_4(1, 9, 25, 80, i + 217);
              STEP_4(2, 10, 26, 81, i + 218);
              STEP_4(1, 3, 11, 27, i + 219);
              STEP_4(4, 12, 28, 82, i + 220);
              STEP_4(1, 5, 13, 29, i + 221);
              STEP_4(2, 6, 14, 30, i + 222);
              STEP_4(1, 3, 7, 15, i + 223);
              STEP_4(31, 83, 157, 92 + alpha, i + 224);
              STEP_4(1, 32, 84, 158, i + 225);
              STEP_4(2, 33, 85, 159, i + 226);
              STEP_4(1, 3, 34, 86, i + 227);
              STEP_4(4, 35, 87, 160, i + 228);
              STEP_4(1, 5, 36, 88, i + 229);
              STEP_4(2, 6, 37, 89, i + 230);
              STEP_4(1, 3, 7, 38, i + 231);
              STEP_4(8, 39, 90, 161, i + 232);
              STEP_4(1, 9, 40, 91, i + 233);
              STEP_4(2, 10, 41, 92, i + 234);
              STEP_4(1, 3, 11, 42, i + 235);
              STEP_4(4, 12, 43, 93, i + 236);
              STEP_4(1, 5, 13, 44, i + 237);
              STEP_4(2, 6, 14, 45, i + 238);
              STEP_4(1, 3, 7, 15, i + 239);
              STEP_4(16, 46, 94, 162, i + 240);
              STEP_4(1, 17, 47, 95, i + 241);
              STEP_4(2, 18, 48, 96, i + 242);
              STEP_4(1, 3, 19, 49, i + 243);
              STEP_4(4, 20, 50, 97, i + 244);
              STEP_4(1, 5, 21, 51, i + 245);
              STEP_4(2, 6, 22, 52, i + 246);
              STEP_4(1, 3, 7, 23, i + 247);
              STEP_4(8, 24, 53, 98, i + 248);
              STEP_4(1, 9, 25, 54, i + 249);
              STEP_4(2, 10, 26, 55, i + 250);
              STEP_4(1, 3, 11, 27, i + 251);
              STEP_4(4, 12, 28, 56, i + 252);
              STEP_4(1, 5, 13, 29, i + 253);
              STEP_4(2, 6, 14, 30, i + 254);
              STEP_4(1, 3, 7, 15, i + 255);

              CHECK_SOLUTIONS();
          }

      }
    }
  }
  FLUSH_SOLUTIONS();
  uint64_t end_time = rdtsc();
 if (verbose) printf("fes: enumeration+check = %" PRIu64 " cycles\n", end_time - enumeration_start_time);
  QUIT();
}
