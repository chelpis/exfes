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


typedef struct {
  uint64_t int_idx;
  uint32_t mask;
} solution_t;

// generated with L = 9
void exhaustive_ia32_deg_3(LUT_t LUT, int n, pck_vector_t F[], solution_callback_t callback, void* callback_state, int verbose) {
  #define QUIT() { \
    return; \
  }

  uint64_t init_start_time = rdtsc();
  // computes the derivatives required by the enumeration kernel up to degree 3
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



  if (verbose) printf("fes: initialisation = %" PRIu64 " cycles\n", rdtsc()-init_start_time);
  uint64_t enumeration_start_time = rdtsc();
  uint64_t n_solutions_found = 0;
  uint64_t current_solution_index = 0;
  uint64_t pack_of_solution[65536];
  solution_t solution_buffer[520];

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

        // we are now inside the critical part where the hamming weight is known to be >= 3
        // Thus, there are no special cases from now on

        // Because of the last step, the current iteration counter is a multiple of 512 plus one
        // This loop sets it to `rolled_end`, which is a multiple of 512, if possible

      const uint64_t rolled_end = weight_2_start + (1ll << min(9, idx_1));
      for(uint64_t i=1 + weight_2_start; i< rolled_end; i++) {

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
          STEP_3( idx_1(LUT, k_1), idx_2(LUT, k_1, k_2), idx_3(LUT, k_1, k_2, k_3), i );
      }

      CHECK_SOLUTIONS();

      // Here, the number of iterations to perform is (supposedly) sufficiently large
      // We will therefore unroll the loop 512 times

      // unrolled critical section where the hamming weight is >= 3
      for(uint64_t j=512; j<(1ull << idx_1); j+=512) {
            const uint64_t i = j + weight_2_start;
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
            const int alpha = LUT[0][k_1];
            const int beta = LUT[1][k_1]+LUT[0][k_2];
            const int gamma = LUT[2][k_1]+LUT[1][k_2]+LUT[0][k_3];
            STEP_3(0 + alpha, 0 + beta, 0 + gamma, i + 0);
            STEP_3(1, 1 + alpha, 1 + beta, i + 1);
            STEP_3(2, 2 + alpha, 2 + beta, i + 2);
            STEP_3(1, 3, 3 + alpha, i + 3);
            STEP_3(4, 4 + alpha, 3 + beta, i + 4);
            STEP_3(1, 5, 5 + alpha, i + 5);
            STEP_3(2, 6, 6 + alpha, i + 6);
            STEP_3(1, 3, 7, i + 7);
            STEP_3(8, 7 + alpha, 4 + beta, i + 8);
            STEP_3(1, 9, 8 + alpha, i + 9);
            STEP_3(2, 10, 9 + alpha, i + 10);
            STEP_3(1, 3, 11, i + 11);
            STEP_3(4, 12, 10 + alpha, i + 12);
            STEP_3(1, 5, 13, i + 13);
            STEP_3(2, 6, 14, i + 14);
            STEP_3(1, 3, 7, i + 15);
            STEP_3(15, 11 + alpha, 5 + beta, i + 16);
            STEP_3(1, 16, 12 + alpha, i + 17);
            STEP_3(2, 17, 13 + alpha, i + 18);
            STEP_3(1, 3, 18, i + 19);
            STEP_3(4, 19, 14 + alpha, i + 20);
            STEP_3(1, 5, 20, i + 21);
            STEP_3(2, 6, 21, i + 22);
            STEP_3(1, 3, 7, i + 23);
            STEP_3(8, 22, 15 + alpha, i + 24);
            STEP_3(1, 9, 23, i + 25);
            STEP_3(2, 10, 24, i + 26);
            STEP_3(1, 3, 11, i + 27);
            STEP_3(4, 12, 25, i + 28);
            STEP_3(1, 5, 13, i + 29);
            STEP_3(2, 6, 14, i + 30);
            STEP_3(1, 3, 7, i + 31);
            STEP_3(26, 16 + alpha, 6 + beta, i + 32);
            STEP_3(1, 27, 17 + alpha, i + 33);
            STEP_3(2, 28, 18 + alpha, i + 34);
            STEP_3(1, 3, 29, i + 35);
            STEP_3(4, 30, 19 + alpha, i + 36);
            STEP_3(1, 5, 31, i + 37);
            STEP_3(2, 6, 32, i + 38);
            STEP_3(1, 3, 7, i + 39);
            STEP_3(8, 33, 20 + alpha, i + 40);
            STEP_3(1, 9, 34, i + 41);
            STEP_3(2, 10, 35, i + 42);
            STEP_3(1, 3, 11, i + 43);
            STEP_3(4, 12, 36, i + 44);
            STEP_3(1, 5, 13, i + 45);
            STEP_3(2, 6, 14, i + 46);
            STEP_3(1, 3, 7, i + 47);
            STEP_3(15, 37, 21 + alpha, i + 48);
            STEP_3(1, 16, 38, i + 49);
            STEP_3(2, 17, 39, i + 50);
            STEP_3(1, 3, 18, i + 51);
            STEP_3(4, 19, 40, i + 52);
            STEP_3(1, 5, 20, i + 53);
            STEP_3(2, 6, 21, i + 54);
            STEP_3(1, 3, 7, i + 55);
            STEP_3(8, 22, 41, i + 56);
            STEP_3(1, 9, 23, i + 57);
            STEP_3(2, 10, 24, i + 58);
            STEP_3(1, 3, 11, i + 59);
            STEP_3(4, 12, 25, i + 60);
            STEP_3(1, 5, 13, i + 61);
            STEP_3(2, 6, 14, i + 62);
            STEP_3(1, 3, 7, i + 63);
            STEP_3(42, 22 + alpha, 7 + beta, i + 64);
            STEP_3(1, 43, 23 + alpha, i + 65);
            STEP_3(2, 44, 24 + alpha, i + 66);
            STEP_3(1, 3, 45, i + 67);
            STEP_3(4, 46, 25 + alpha, i + 68);
            STEP_3(1, 5, 47, i + 69);
            STEP_3(2, 6, 48, i + 70);
            STEP_3(1, 3, 7, i + 71);
            STEP_3(8, 49, 26 + alpha, i + 72);
            STEP_3(1, 9, 50, i + 73);
            STEP_3(2, 10, 51, i + 74);
            STEP_3(1, 3, 11, i + 75);
            STEP_3(4, 12, 52, i + 76);
            STEP_3(1, 5, 13, i + 77);
            STEP_3(2, 6, 14, i + 78);
            STEP_3(1, 3, 7, i + 79);
            STEP_3(15, 53, 27 + alpha, i + 80);
            STEP_3(1, 16, 54, i + 81);
            STEP_3(2, 17, 55, i + 82);
            STEP_3(1, 3, 18, i + 83);
            STEP_3(4, 19, 56, i + 84);
            STEP_3(1, 5, 20, i + 85);
            STEP_3(2, 6, 21, i + 86);
            STEP_3(1, 3, 7, i + 87);
            STEP_3(8, 22, 57, i + 88);
            STEP_3(1, 9, 23, i + 89);
            STEP_3(2, 10, 24, i + 90);
            STEP_3(1, 3, 11, i + 91);
            STEP_3(4, 12, 25, i + 92);
            STEP_3(1, 5, 13, i + 93);
            STEP_3(2, 6, 14, i + 94);
            STEP_3(1, 3, 7, i + 95);
            STEP_3(26, 58, 28 + alpha, i + 96);
            STEP_3(1, 27, 59, i + 97);
            STEP_3(2, 28, 60, i + 98);
            STEP_3(1, 3, 29, i + 99);
            STEP_3(4, 30, 61, i + 100);
            STEP_3(1, 5, 31, i + 101);
            STEP_3(2, 6, 32, i + 102);
            STEP_3(1, 3, 7, i + 103);
            STEP_3(8, 33, 62, i + 104);
            STEP_3(1, 9, 34, i + 105);
            STEP_3(2, 10, 35, i + 106);
            STEP_3(1, 3, 11, i + 107);
            STEP_3(4, 12, 36, i + 108);
            STEP_3(1, 5, 13, i + 109);
            STEP_3(2, 6, 14, i + 110);
            STEP_3(1, 3, 7, i + 111);
            STEP_3(15, 37, 63, i + 112);
            STEP_3(1, 16, 38, i + 113);
            STEP_3(2, 17, 39, i + 114);
            STEP_3(1, 3, 18, i + 115);
            STEP_3(4, 19, 40, i + 116);
            STEP_3(1, 5, 20, i + 117);
            STEP_3(2, 6, 21, i + 118);
            STEP_3(1, 3, 7, i + 119);
            STEP_3(8, 22, 41, i + 120);
            STEP_3(1, 9, 23, i + 121);
            STEP_3(2, 10, 24, i + 122);
            STEP_3(1, 3, 11, i + 123);
            STEP_3(4, 12, 25, i + 124);
            STEP_3(1, 5, 13, i + 125);
            STEP_3(2, 6, 14, i + 126);
            STEP_3(1, 3, 7, i + 127);
            STEP_3(64, 29 + alpha, 8 + beta, i + 128);
            STEP_3(1, 65, 30 + alpha, i + 129);
            STEP_3(2, 66, 31 + alpha, i + 130);
            STEP_3(1, 3, 67, i + 131);
            STEP_3(4, 68, 32 + alpha, i + 132);
            STEP_3(1, 5, 69, i + 133);
            STEP_3(2, 6, 70, i + 134);
            STEP_3(1, 3, 7, i + 135);
            STEP_3(8, 71, 33 + alpha, i + 136);
            STEP_3(1, 9, 72, i + 137);
            STEP_3(2, 10, 73, i + 138);
            STEP_3(1, 3, 11, i + 139);
            STEP_3(4, 12, 74, i + 140);
            STEP_3(1, 5, 13, i + 141);
            STEP_3(2, 6, 14, i + 142);
            STEP_3(1, 3, 7, i + 143);
            STEP_3(15, 75, 34 + alpha, i + 144);
            STEP_3(1, 16, 76, i + 145);
            STEP_3(2, 17, 77, i + 146);
            STEP_3(1, 3, 18, i + 147);
            STEP_3(4, 19, 78, i + 148);
            STEP_3(1, 5, 20, i + 149);
            STEP_3(2, 6, 21, i + 150);
            STEP_3(1, 3, 7, i + 151);
            STEP_3(8, 22, 79, i + 152);
            STEP_3(1, 9, 23, i + 153);
            STEP_3(2, 10, 24, i + 154);
            STEP_3(1, 3, 11, i + 155);
            STEP_3(4, 12, 25, i + 156);
            STEP_3(1, 5, 13, i + 157);
            STEP_3(2, 6, 14, i + 158);
            STEP_3(1, 3, 7, i + 159);
            STEP_3(26, 80, 35 + alpha, i + 160);
            STEP_3(1, 27, 81, i + 161);
            STEP_3(2, 28, 82, i + 162);
            STEP_3(1, 3, 29, i + 163);
            STEP_3(4, 30, 83, i + 164);
            STEP_3(1, 5, 31, i + 165);
            STEP_3(2, 6, 32, i + 166);
            STEP_3(1, 3, 7, i + 167);
            STEP_3(8, 33, 84, i + 168);
            STEP_3(1, 9, 34, i + 169);
            STEP_3(2, 10, 35, i + 170);
            STEP_3(1, 3, 11, i + 171);
            STEP_3(4, 12, 36, i + 172);
            STEP_3(1, 5, 13, i + 173);
            STEP_3(2, 6, 14, i + 174);
            STEP_3(1, 3, 7, i + 175);
            STEP_3(15, 37, 85, i + 176);
            STEP_3(1, 16, 38, i + 177);
            STEP_3(2, 17, 39, i + 178);
            STEP_3(1, 3, 18, i + 179);
            STEP_3(4, 19, 40, i + 180);
            STEP_3(1, 5, 20, i + 181);
            STEP_3(2, 6, 21, i + 182);
            STEP_3(1, 3, 7, i + 183);
            STEP_3(8, 22, 41, i + 184);
            STEP_3(1, 9, 23, i + 185);
            STEP_3(2, 10, 24, i + 186);
            STEP_3(1, 3, 11, i + 187);
            STEP_3(4, 12, 25, i + 188);
            STEP_3(1, 5, 13, i + 189);
            STEP_3(2, 6, 14, i + 190);
            STEP_3(1, 3, 7, i + 191);
            STEP_3(42, 86, 36 + alpha, i + 192);
            STEP_3(1, 43, 87, i + 193);
            STEP_3(2, 44, 88, i + 194);
            STEP_3(1, 3, 45, i + 195);
            STEP_3(4, 46, 89, i + 196);
            STEP_3(1, 5, 47, i + 197);
            STEP_3(2, 6, 48, i + 198);
            STEP_3(1, 3, 7, i + 199);
            STEP_3(8, 49, 90, i + 200);
            STEP_3(1, 9, 50, i + 201);
            STEP_3(2, 10, 51, i + 202);
            STEP_3(1, 3, 11, i + 203);
            STEP_3(4, 12, 52, i + 204);
            STEP_3(1, 5, 13, i + 205);
            STEP_3(2, 6, 14, i + 206);
            STEP_3(1, 3, 7, i + 207);
            STEP_3(15, 53, 91, i + 208);
            STEP_3(1, 16, 54, i + 209);
            STEP_3(2, 17, 55, i + 210);
            STEP_3(1, 3, 18, i + 211);
            STEP_3(4, 19, 56, i + 212);
            STEP_3(1, 5, 20, i + 213);
            STEP_3(2, 6, 21, i + 214);
            STEP_3(1, 3, 7, i + 215);
            STEP_3(8, 22, 57, i + 216);
            STEP_3(1, 9, 23, i + 217);
            STEP_3(2, 10, 24, i + 218);
            STEP_3(1, 3, 11, i + 219);
            STEP_3(4, 12, 25, i + 220);
            STEP_3(1, 5, 13, i + 221);
            STEP_3(2, 6, 14, i + 222);
            STEP_3(1, 3, 7, i + 223);
            STEP_3(26, 58, 92, i + 224);
            STEP_3(1, 27, 59, i + 225);
            STEP_3(2, 28, 60, i + 226);
            STEP_3(1, 3, 29, i + 227);
            STEP_3(4, 30, 61, i + 228);
            STEP_3(1, 5, 31, i + 229);
            STEP_3(2, 6, 32, i + 230);
            STEP_3(1, 3, 7, i + 231);
            STEP_3(8, 33, 62, i + 232);
            STEP_3(1, 9, 34, i + 233);
            STEP_3(2, 10, 35, i + 234);
            STEP_3(1, 3, 11, i + 235);
            STEP_3(4, 12, 36, i + 236);
            STEP_3(1, 5, 13, i + 237);
            STEP_3(2, 6, 14, i + 238);
            STEP_3(1, 3, 7, i + 239);
            STEP_3(15, 37, 63, i + 240);
            STEP_3(1, 16, 38, i + 241);
            STEP_3(2, 17, 39, i + 242);
            STEP_3(1, 3, 18, i + 243);
            STEP_3(4, 19, 40, i + 244);
            STEP_3(1, 5, 20, i + 245);
            STEP_3(2, 6, 21, i + 246);
            STEP_3(1, 3, 7, i + 247);
            STEP_3(8, 22, 41, i + 248);
            STEP_3(1, 9, 23, i + 249);
            STEP_3(2, 10, 24, i + 250);
            STEP_3(1, 3, 11, i + 251);
            STEP_3(4, 12, 25, i + 252);
            STEP_3(1, 5, 13, i + 253);
            STEP_3(2, 6, 14, i + 254);
            STEP_3(1, 3, 7, i + 255);
            STEP_3(93, 37 + alpha, 9 + beta, i + 256);
            STEP_3(1, 94, 38 + alpha, i + 257);
            STEP_3(2, 95, 39 + alpha, i + 258);
            STEP_3(1, 3, 96, i + 259);
            STEP_3(4, 97, 40 + alpha, i + 260);
            STEP_3(1, 5, 98, i + 261);
            STEP_3(2, 6, 99, i + 262);
            STEP_3(1, 3, 7, i + 263);
            STEP_3(8, 100, 41 + alpha, i + 264);
            STEP_3(1, 9, 101, i + 265);
            STEP_3(2, 10, 102, i + 266);
            STEP_3(1, 3, 11, i + 267);
            STEP_3(4, 12, 103, i + 268);
            STEP_3(1, 5, 13, i + 269);
            STEP_3(2, 6, 14, i + 270);
            STEP_3(1, 3, 7, i + 271);
            STEP_3(15, 104, 42 + alpha, i + 272);
            STEP_3(1, 16, 105, i + 273);
            STEP_3(2, 17, 106, i + 274);
            STEP_3(1, 3, 18, i + 275);
            STEP_3(4, 19, 107, i + 276);
            STEP_3(1, 5, 20, i + 277);
            STEP_3(2, 6, 21, i + 278);
            STEP_3(1, 3, 7, i + 279);
            STEP_3(8, 22, 108, i + 280);
            STEP_3(1, 9, 23, i + 281);
            STEP_3(2, 10, 24, i + 282);
            STEP_3(1, 3, 11, i + 283);
            STEP_3(4, 12, 25, i + 284);
            STEP_3(1, 5, 13, i + 285);
            STEP_3(2, 6, 14, i + 286);
            STEP_3(1, 3, 7, i + 287);
            STEP_3(26, 109, 43 + alpha, i + 288);
            STEP_3(1, 27, 110, i + 289);
            STEP_3(2, 28, 111, i + 290);
            STEP_3(1, 3, 29, i + 291);
            STEP_3(4, 30, 112, i + 292);
            STEP_3(1, 5, 31, i + 293);
            STEP_3(2, 6, 32, i + 294);
            STEP_3(1, 3, 7, i + 295);
            STEP_3(8, 33, 113, i + 296);
            STEP_3(1, 9, 34, i + 297);
            STEP_3(2, 10, 35, i + 298);
            STEP_3(1, 3, 11, i + 299);
            STEP_3(4, 12, 36, i + 300);
            STEP_3(1, 5, 13, i + 301);
            STEP_3(2, 6, 14, i + 302);
            STEP_3(1, 3, 7, i + 303);
            STEP_3(15, 37, 114, i + 304);
            STEP_3(1, 16, 38, i + 305);
            STEP_3(2, 17, 39, i + 306);
            STEP_3(1, 3, 18, i + 307);
            STEP_3(4, 19, 40, i + 308);
            STEP_3(1, 5, 20, i + 309);
            STEP_3(2, 6, 21, i + 310);
            STEP_3(1, 3, 7, i + 311);
            STEP_3(8, 22, 41, i + 312);
            STEP_3(1, 9, 23, i + 313);
            STEP_3(2, 10, 24, i + 314);
            STEP_3(1, 3, 11, i + 315);
            STEP_3(4, 12, 25, i + 316);
            STEP_3(1, 5, 13, i + 317);
            STEP_3(2, 6, 14, i + 318);
            STEP_3(1, 3, 7, i + 319);
            STEP_3(42, 115, 44 + alpha, i + 320);
            STEP_3(1, 43, 116, i + 321);
            STEP_3(2, 44, 117, i + 322);
            STEP_3(1, 3, 45, i + 323);
            STEP_3(4, 46, 118, i + 324);
            STEP_3(1, 5, 47, i + 325);
            STEP_3(2, 6, 48, i + 326);
            STEP_3(1, 3, 7, i + 327);
            STEP_3(8, 49, 119, i + 328);
            STEP_3(1, 9, 50, i + 329);
            STEP_3(2, 10, 51, i + 330);
            STEP_3(1, 3, 11, i + 331);
            STEP_3(4, 12, 52, i + 332);
            STEP_3(1, 5, 13, i + 333);
            STEP_3(2, 6, 14, i + 334);
            STEP_3(1, 3, 7, i + 335);
            STEP_3(15, 53, 120, i + 336);
            STEP_3(1, 16, 54, i + 337);
            STEP_3(2, 17, 55, i + 338);
            STEP_3(1, 3, 18, i + 339);
            STEP_3(4, 19, 56, i + 340);
            STEP_3(1, 5, 20, i + 341);
            STEP_3(2, 6, 21, i + 342);
            STEP_3(1, 3, 7, i + 343);
            STEP_3(8, 22, 57, i + 344);
            STEP_3(1, 9, 23, i + 345);
            STEP_3(2, 10, 24, i + 346);
            STEP_3(1, 3, 11, i + 347);
            STEP_3(4, 12, 25, i + 348);
            STEP_3(1, 5, 13, i + 349);
            STEP_3(2, 6, 14, i + 350);
            STEP_3(1, 3, 7, i + 351);
            STEP_3(26, 58, 121, i + 352);
            STEP_3(1, 27, 59, i + 353);
            STEP_3(2, 28, 60, i + 354);
            STEP_3(1, 3, 29, i + 355);
            STEP_3(4, 30, 61, i + 356);
            STEP_3(1, 5, 31, i + 357);
            STEP_3(2, 6, 32, i + 358);
            STEP_3(1, 3, 7, i + 359);
            STEP_3(8, 33, 62, i + 360);
            STEP_3(1, 9, 34, i + 361);
            STEP_3(2, 10, 35, i + 362);
            STEP_3(1, 3, 11, i + 363);
            STEP_3(4, 12, 36, i + 364);
            STEP_3(1, 5, 13, i + 365);
            STEP_3(2, 6, 14, i + 366);
            STEP_3(1, 3, 7, i + 367);
            STEP_3(15, 37, 63, i + 368);
            STEP_3(1, 16, 38, i + 369);
            STEP_3(2, 17, 39, i + 370);
            STEP_3(1, 3, 18, i + 371);
            STEP_3(4, 19, 40, i + 372);
            STEP_3(1, 5, 20, i + 373);
            STEP_3(2, 6, 21, i + 374);
            STEP_3(1, 3, 7, i + 375);
            STEP_3(8, 22, 41, i + 376);
            STEP_3(1, 9, 23, i + 377);
            STEP_3(2, 10, 24, i + 378);
            STEP_3(1, 3, 11, i + 379);
            STEP_3(4, 12, 25, i + 380);
            STEP_3(1, 5, 13, i + 381);
            STEP_3(2, 6, 14, i + 382);
            STEP_3(1, 3, 7, i + 383);
            STEP_3(64, 122, 45 + alpha, i + 384);
            STEP_3(1, 65, 123, i + 385);
            STEP_3(2, 66, 124, i + 386);
            STEP_3(1, 3, 67, i + 387);
            STEP_3(4, 68, 125, i + 388);
            STEP_3(1, 5, 69, i + 389);
            STEP_3(2, 6, 70, i + 390);
            STEP_3(1, 3, 7, i + 391);
            STEP_3(8, 71, 126, i + 392);
            STEP_3(1, 9, 72, i + 393);
            STEP_3(2, 10, 73, i + 394);
            STEP_3(1, 3, 11, i + 395);
            STEP_3(4, 12, 74, i + 396);
            STEP_3(1, 5, 13, i + 397);
            STEP_3(2, 6, 14, i + 398);
            STEP_3(1, 3, 7, i + 399);
            STEP_3(15, 75, 127, i + 400);
            STEP_3(1, 16, 76, i + 401);
            STEP_3(2, 17, 77, i + 402);
            STEP_3(1, 3, 18, i + 403);
            STEP_3(4, 19, 78, i + 404);
            STEP_3(1, 5, 20, i + 405);
            STEP_3(2, 6, 21, i + 406);
            STEP_3(1, 3, 7, i + 407);
            STEP_3(8, 22, 79, i + 408);
            STEP_3(1, 9, 23, i + 409);
            STEP_3(2, 10, 24, i + 410);
            STEP_3(1, 3, 11, i + 411);
            STEP_3(4, 12, 25, i + 412);
            STEP_3(1, 5, 13, i + 413);
            STEP_3(2, 6, 14, i + 414);
            STEP_3(1, 3, 7, i + 415);
            STEP_3(26, 80, 128, i + 416);
            STEP_3(1, 27, 81, i + 417);
            STEP_3(2, 28, 82, i + 418);
            STEP_3(1, 3, 29, i + 419);
            STEP_3(4, 30, 83, i + 420);
            STEP_3(1, 5, 31, i + 421);
            STEP_3(2, 6, 32, i + 422);
            STEP_3(1, 3, 7, i + 423);
            STEP_3(8, 33, 84, i + 424);
            STEP_3(1, 9, 34, i + 425);
            STEP_3(2, 10, 35, i + 426);
            STEP_3(1, 3, 11, i + 427);
            STEP_3(4, 12, 36, i + 428);
            STEP_3(1, 5, 13, i + 429);
            STEP_3(2, 6, 14, i + 430);
            STEP_3(1, 3, 7, i + 431);
            STEP_3(15, 37, 85, i + 432);
            STEP_3(1, 16, 38, i + 433);
            STEP_3(2, 17, 39, i + 434);
            STEP_3(1, 3, 18, i + 435);
            STEP_3(4, 19, 40, i + 436);
            STEP_3(1, 5, 20, i + 437);
            STEP_3(2, 6, 21, i + 438);
            STEP_3(1, 3, 7, i + 439);
            STEP_3(8, 22, 41, i + 440);
            STEP_3(1, 9, 23, i + 441);
            STEP_3(2, 10, 24, i + 442);
            STEP_3(1, 3, 11, i + 443);
            STEP_3(4, 12, 25, i + 444);
            STEP_3(1, 5, 13, i + 445);
            STEP_3(2, 6, 14, i + 446);
            STEP_3(1, 3, 7, i + 447);
            STEP_3(42, 86, 129, i + 448);
            STEP_3(1, 43, 87, i + 449);
            STEP_3(2, 44, 88, i + 450);
            STEP_3(1, 3, 45, i + 451);
            STEP_3(4, 46, 89, i + 452);
            STEP_3(1, 5, 47, i + 453);
            STEP_3(2, 6, 48, i + 454);
            STEP_3(1, 3, 7, i + 455);
            STEP_3(8, 49, 90, i + 456);
            STEP_3(1, 9, 50, i + 457);
            STEP_3(2, 10, 51, i + 458);
            STEP_3(1, 3, 11, i + 459);
            STEP_3(4, 12, 52, i + 460);
            STEP_3(1, 5, 13, i + 461);
            STEP_3(2, 6, 14, i + 462);
            STEP_3(1, 3, 7, i + 463);
            STEP_3(15, 53, 91, i + 464);
            STEP_3(1, 16, 54, i + 465);
            STEP_3(2, 17, 55, i + 466);
            STEP_3(1, 3, 18, i + 467);
            STEP_3(4, 19, 56, i + 468);
            STEP_3(1, 5, 20, i + 469);
            STEP_3(2, 6, 21, i + 470);
            STEP_3(1, 3, 7, i + 471);
            STEP_3(8, 22, 57, i + 472);
            STEP_3(1, 9, 23, i + 473);
            STEP_3(2, 10, 24, i + 474);
            STEP_3(1, 3, 11, i + 475);
            STEP_3(4, 12, 25, i + 476);
            STEP_3(1, 5, 13, i + 477);
            STEP_3(2, 6, 14, i + 478);
            STEP_3(1, 3, 7, i + 479);
            STEP_3(26, 58, 92, i + 480);
            STEP_3(1, 27, 59, i + 481);
            STEP_3(2, 28, 60, i + 482);
            STEP_3(1, 3, 29, i + 483);
            STEP_3(4, 30, 61, i + 484);
            STEP_3(1, 5, 31, i + 485);
            STEP_3(2, 6, 32, i + 486);
            STEP_3(1, 3, 7, i + 487);
            STEP_3(8, 33, 62, i + 488);
            STEP_3(1, 9, 34, i + 489);
            STEP_3(2, 10, 35, i + 490);
            STEP_3(1, 3, 11, i + 491);
            STEP_3(4, 12, 36, i + 492);
            STEP_3(1, 5, 13, i + 493);
            STEP_3(2, 6, 14, i + 494);
            STEP_3(1, 3, 7, i + 495);
            STEP_3(15, 37, 63, i + 496);
            STEP_3(1, 16, 38, i + 497);
            STEP_3(2, 17, 39, i + 498);
            STEP_3(1, 3, 18, i + 499);
            STEP_3(4, 19, 40, i + 500);
            STEP_3(1, 5, 20, i + 501);
            STEP_3(2, 6, 21, i + 502);
            STEP_3(1, 3, 7, i + 503);
            STEP_3(8, 22, 41, i + 504);
            STEP_3(1, 9, 23, i + 505);
            STEP_3(2, 10, 24, i + 506);
            STEP_3(1, 3, 11, i + 507);
            STEP_3(4, 12, 25, i + 508);
            STEP_3(1, 5, 13, i + 509);
            STEP_3(2, 6, 14, i + 510);
            STEP_3(1, 3, 7, i + 511);

            CHECK_SOLUTIONS();
        }

    }
  }
  FLUSH_SOLUTIONS();
  uint64_t end_time = rdtsc();
 if (verbose) printf("fes: enumeration+check = %" PRIu64 " cycles\n", end_time - enumeration_start_time);
  QUIT();
}
