#include "miner.h"
#include "idxlut.h"
#include <assert.h>
#include <stdarg.h>  
#include <stdbool.h>
#include <err.h>
#include <sysexits.h>

#ifdef __cplusplus 

extern "C"{

#endif



typedef union {
    __m128i v;
    uint16_t e[8];
} Vec4;

#define THRESHOLD 9
vector_t init_vector(int n_rows) {
  return (vector_t)calloc(n_rows, sizeof(int));
}

pck_vector_t pack(int n, const vector_t v) {
  pck_vector_t r = 0;
  assert((unsigned int) n <= 8*sizeof(pck_vector_t));

  for(int i=n-1; i>=0; i--) {
    r = r << 1ll;
    r |= v[i] & 0x0001ll;
  }
  return r;
}

uint64_t to_gray(uint64_t i) {
  return (i ^ (i >> 1ll));
}

#ifdef __i386
uint64_t rdtsc() {
  uint64_t x;
  __asm__ volatile ("rdtsc" : "=A" (x));
  return x;
}
#else 
uint64_t rdtsc() {
  uint64_t a, d;
  __asm__ volatile ("rdtsc" : "=a" (a), "=d" (d));
  return (d<<32) | a;
}
#endif

pck_vector_t packed_eval(LUT_t LUT, int n, int d, pck_vector_t F[], uint64_t i) {
  if (d == 2) {
     return packed_eval_deg_2(LUT, n, F, i);
  } else {
	  printf("degree-%d naive evaluation is not yet implemented...\n", d);
	  assert(0);
	  return 0;
  }
}

// this ought to be optimized, unrolled, etc.
void BFS(pck_vector_t *A, int start, int n) {

  for(int i=0; i<n; i++) {
    
    int Sz = 1 << i;
    int Pos = start;

    while(Pos < start + (1 << n)) {
      for(int j=0; j<Sz; j++)
	A[Pos + Sz + j] ^= A[Pos + j];
      Pos += 2*Sz;
    }
  }
}

void hybrid_DFS_BFS(pck_vector_t *A, int from, int to) {
  if (from >= to-1) return;
 
  int center = (to+from)/2;
  int half_length = (to-from)/2;

  if (half_length == (1 << (THRESHOLD-1)))
    BFS(A, from, THRESHOLD);
  else {
    hybrid_DFS_BFS(A, from, center);
    hybrid_DFS_BFS(A, center, to);
    for(int i=0; i<half_length; i++)
      A[center + i] ^= A[from + i];
  }
}

void moebius_transform(int n, pck_vector_t F[], solution_callback_t callback, void* callback_state) {

  // compute the moebius transform
  hybrid_DFS_BFS(F, 0, (1ll << n));

  // check for solutions [could/should be integrated into BFS, at least to allow early abort, and to improve cache usage ?]
  uint64_t size = 1ll << n;
  for(uint64_t i=0; i<size; i++)
    if (F[i] == 0)
      if ((*callback)(callback_state, 1, &i))
	return;
}

void print_vec(__m128i foo) {
  Vec4 bar;
  bar.v = foo;
  for(int i=0; i<8; i++)
    printf("%04x ", bar.e[i]);
}

pck_vector_t packed_eval_deg_2(LUT_t LUT, int n, pck_vector_t F[], uint64_t i) {
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

     }
  }

  return y;
}

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

typedef struct {
  uint64_t int_idx;
  uint32_t mask;
} solution_t;

// generated with L = 9
void exhaustive_ia32_deg_2(LUT_t LUT, int n, pck_vector_t F[], solution_callback_t callback, void* callback_state, int verbose) {
  #define QUIT() { \
    return; \
  }

  uint64_t init_start_time = rdtsc();
  // computes the derivatives required by the enumeration kernel up to degree 2
  // this is done in-place, meaning that if "F" described the coefficients of the
  // polynomials before, then afterwards, they describe the derivatives

  // here, degree-1 terms are affected by degree-2 terms
  for(int i0=1; i0<n; i0++) {
      if (i0  != 0 ) F[ idx_1(LUT, i0) ] ^= F[ idx_2(LUT, i0-1, i0) ];
  }

  if (verbose) printf("fes: initialisation = %" PRIu64 " cycles\n", rdtsc()-init_start_time);
  uint64_t enumeration_start_time = rdtsc();
  uint64_t n_solutions_found = 0;
  uint64_t current_solution_index = 0;
  uint64_t pack_of_solution[65536];
  solution_t solution_buffer[516];

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

      // we are now inside the critical part where the hamming weight is known to be >= 2
      // Thus, there are no special cases from now on

      // Because of the last step, the current iteration counter is a multiple of 512 plus one
      // This loop sets it to `rolled_end`, which is a multiple of 512, if possible

    const uint64_t rolled_end = weight_1_start + (1ll << min(9, idx_0));
    for(uint64_t i=1 + weight_1_start; i< rolled_end; i++) {

          int pos = 0;
          uint64_t _i = i;
          while ((_i & 0x0001) == 0) { _i >>= 1; pos++; }
          const int k_1 = pos;
          _i >>= 1; pos++;
          while ((_i & 0x0001) == 0) { _i >>= 1; pos++; }
          const int k_2 = pos;
        STEP_2( idx_1(LUT, k_1), idx_2(LUT, k_1, k_2), i );
    }

    CHECK_SOLUTIONS();

    // Here, the number of iterations to perform is (supposedly) sufficiently large
    // We will therefore unroll the loop 512 times

    // unrolled critical section where the hamming weight is >= 2
    for(uint64_t j=512; j<(1ull << idx_0); j+=512) {
          const uint64_t i = j + weight_1_start;
          int pos = 0;
          uint64_t _i = i;
          while ((_i & 0x0001) == 0) { _i >>= 1; pos++; }
          const int k_1 = pos;
          _i >>= 1; pos++;
          while ((_i & 0x0001) == 0) { _i >>= 1; pos++; }
          const int k_2 = pos;
          const int alpha = LUT[0][k_1];
          const int beta = LUT[1][k_1]+LUT[0][k_2];
          STEP_2(0 + alpha, 0 + beta, i + 0);
          STEP_2(1, 1 + alpha, i + 1);
          STEP_2(2, 2 + alpha, i + 2);
          STEP_2(1, 3, i + 3);
          STEP_2(4, 3 + alpha, i + 4);
          STEP_2(1, 5, i + 5);
          STEP_2(2, 6, i + 6);
          STEP_2(1, 3, i + 7);
          STEP_2(7, 4 + alpha, i + 8);
          STEP_2(1, 8, i + 9);
          STEP_2(2, 9, i + 10);
          STEP_2(1, 3, i + 11);
          STEP_2(4, 10, i + 12);
          STEP_2(1, 5, i + 13);
          STEP_2(2, 6, i + 14);
          STEP_2(1, 3, i + 15);
          STEP_2(11, 5 + alpha, i + 16);
          STEP_2(1, 12, i + 17);
          STEP_2(2, 13, i + 18);
          STEP_2(1, 3, i + 19);
          STEP_2(4, 14, i + 20);
          STEP_2(1, 5, i + 21);
          STEP_2(2, 6, i + 22);
          STEP_2(1, 3, i + 23);
          STEP_2(7, 15, i + 24);
          STEP_2(1, 8, i + 25);
          STEP_2(2, 9, i + 26);
          STEP_2(1, 3, i + 27);
          STEP_2(4, 10, i + 28);
          STEP_2(1, 5, i + 29);
          STEP_2(2, 6, i + 30);
          STEP_2(1, 3, i + 31);
          STEP_2(16, 6 + alpha, i + 32);
          STEP_2(1, 17, i + 33);
          STEP_2(2, 18, i + 34);
          STEP_2(1, 3, i + 35);
          STEP_2(4, 19, i + 36);
          STEP_2(1, 5, i + 37);
          STEP_2(2, 6, i + 38);
          STEP_2(1, 3, i + 39);
          STEP_2(7, 20, i + 40);
          STEP_2(1, 8, i + 41);
          STEP_2(2, 9, i + 42);
          STEP_2(1, 3, i + 43);
          STEP_2(4, 10, i + 44);
          STEP_2(1, 5, i + 45);
          STEP_2(2, 6, i + 46);
          STEP_2(1, 3, i + 47);
          STEP_2(11, 21, i + 48);
          STEP_2(1, 12, i + 49);
          STEP_2(2, 13, i + 50);
          STEP_2(1, 3, i + 51);
          STEP_2(4, 14, i + 52);
          STEP_2(1, 5, i + 53);
          STEP_2(2, 6, i + 54);
          STEP_2(1, 3, i + 55);
          STEP_2(7, 15, i + 56);
          STEP_2(1, 8, i + 57);
          STEP_2(2, 9, i + 58);
          STEP_2(1, 3, i + 59);
          STEP_2(4, 10, i + 60);
          STEP_2(1, 5, i + 61);
          STEP_2(2, 6, i + 62);
          STEP_2(1, 3, i + 63);
          STEP_2(22, 7 + alpha, i + 64);
          STEP_2(1, 23, i + 65);
          STEP_2(2, 24, i + 66);
          STEP_2(1, 3, i + 67);
          STEP_2(4, 25, i + 68);
          STEP_2(1, 5, i + 69);
          STEP_2(2, 6, i + 70);
          STEP_2(1, 3, i + 71);
          STEP_2(7, 26, i + 72);
          STEP_2(1, 8, i + 73);
          STEP_2(2, 9, i + 74);
          STEP_2(1, 3, i + 75);
          STEP_2(4, 10, i + 76);
          STEP_2(1, 5, i + 77);
          STEP_2(2, 6, i + 78);
          STEP_2(1, 3, i + 79);
          STEP_2(11, 27, i + 80);
          STEP_2(1, 12, i + 81);
          STEP_2(2, 13, i + 82);
          STEP_2(1, 3, i + 83);
          STEP_2(4, 14, i + 84);
          STEP_2(1, 5, i + 85);
          STEP_2(2, 6, i + 86);
          STEP_2(1, 3, i + 87);
          STEP_2(7, 15, i + 88);
          STEP_2(1, 8, i + 89);
          STEP_2(2, 9, i + 90);
          STEP_2(1, 3, i + 91);
          STEP_2(4, 10, i + 92);
          STEP_2(1, 5, i + 93);
          STEP_2(2, 6, i + 94);
          STEP_2(1, 3, i + 95);
          STEP_2(16, 28, i + 96);
          STEP_2(1, 17, i + 97);
          STEP_2(2, 18, i + 98);
          STEP_2(1, 3, i + 99);
          STEP_2(4, 19, i + 100);
          STEP_2(1, 5, i + 101);
          STEP_2(2, 6, i + 102);
          STEP_2(1, 3, i + 103);
          STEP_2(7, 20, i + 104);
          STEP_2(1, 8, i + 105);
          STEP_2(2, 9, i + 106);
          STEP_2(1, 3, i + 107);
          STEP_2(4, 10, i + 108);
          STEP_2(1, 5, i + 109);
          STEP_2(2, 6, i + 110);
          STEP_2(1, 3, i + 111);
          STEP_2(11, 21, i + 112);
          STEP_2(1, 12, i + 113);
          STEP_2(2, 13, i + 114);
          STEP_2(1, 3, i + 115);
          STEP_2(4, 14, i + 116);
          STEP_2(1, 5, i + 117);
          STEP_2(2, 6, i + 118);
          STEP_2(1, 3, i + 119);
          STEP_2(7, 15, i + 120);
          STEP_2(1, 8, i + 121);
          STEP_2(2, 9, i + 122);
          STEP_2(1, 3, i + 123);
          STEP_2(4, 10, i + 124);
          STEP_2(1, 5, i + 125);
          STEP_2(2, 6, i + 126);
          STEP_2(1, 3, i + 127);
          STEP_2(29, 8 + alpha, i + 128);
          STEP_2(1, 30, i + 129);
          STEP_2(2, 31, i + 130);
          STEP_2(1, 3, i + 131);
          STEP_2(4, 32, i + 132);
          STEP_2(1, 5, i + 133);
          STEP_2(2, 6, i + 134);
          STEP_2(1, 3, i + 135);
          STEP_2(7, 33, i + 136);
          STEP_2(1, 8, i + 137);
          STEP_2(2, 9, i + 138);
          STEP_2(1, 3, i + 139);
          STEP_2(4, 10, i + 140);
          STEP_2(1, 5, i + 141);
          STEP_2(2, 6, i + 142);
          STEP_2(1, 3, i + 143);
          STEP_2(11, 34, i + 144);
          STEP_2(1, 12, i + 145);
          STEP_2(2, 13, i + 146);
          STEP_2(1, 3, i + 147);
          STEP_2(4, 14, i + 148);
          STEP_2(1, 5, i + 149);
          STEP_2(2, 6, i + 150);
          STEP_2(1, 3, i + 151);
          STEP_2(7, 15, i + 152);
          STEP_2(1, 8, i + 153);
          STEP_2(2, 9, i + 154);
          STEP_2(1, 3, i + 155);
          STEP_2(4, 10, i + 156);
          STEP_2(1, 5, i + 157);
          STEP_2(2, 6, i + 158);
          STEP_2(1, 3, i + 159);
          STEP_2(16, 35, i + 160);
          STEP_2(1, 17, i + 161);
          STEP_2(2, 18, i + 162);
          STEP_2(1, 3, i + 163);
          STEP_2(4, 19, i + 164);
          STEP_2(1, 5, i + 165);
          STEP_2(2, 6, i + 166);
          STEP_2(1, 3, i + 167);
          STEP_2(7, 20, i + 168);
          STEP_2(1, 8, i + 169);
          STEP_2(2, 9, i + 170);
          STEP_2(1, 3, i + 171);
          STEP_2(4, 10, i + 172);
          STEP_2(1, 5, i + 173);
          STEP_2(2, 6, i + 174);
          STEP_2(1, 3, i + 175);
          STEP_2(11, 21, i + 176);
          STEP_2(1, 12, i + 177);
          STEP_2(2, 13, i + 178);
          STEP_2(1, 3, i + 179);
          STEP_2(4, 14, i + 180);
          STEP_2(1, 5, i + 181);
          STEP_2(2, 6, i + 182);
          STEP_2(1, 3, i + 183);
          STEP_2(7, 15, i + 184);
          STEP_2(1, 8, i + 185);
          STEP_2(2, 9, i + 186);
          STEP_2(1, 3, i + 187);
          STEP_2(4, 10, i + 188);
          STEP_2(1, 5, i + 189);
          STEP_2(2, 6, i + 190);
          STEP_2(1, 3, i + 191);
          STEP_2(22, 36, i + 192);
          STEP_2(1, 23, i + 193);
          STEP_2(2, 24, i + 194);
          STEP_2(1, 3, i + 195);
          STEP_2(4, 25, i + 196);
          STEP_2(1, 5, i + 197);
          STEP_2(2, 6, i + 198);
          STEP_2(1, 3, i + 199);
          STEP_2(7, 26, i + 200);
          STEP_2(1, 8, i + 201);
          STEP_2(2, 9, i + 202);
          STEP_2(1, 3, i + 203);
          STEP_2(4, 10, i + 204);
          STEP_2(1, 5, i + 205);
          STEP_2(2, 6, i + 206);
          STEP_2(1, 3, i + 207);
          STEP_2(11, 27, i + 208);
          STEP_2(1, 12, i + 209);
          STEP_2(2, 13, i + 210);
          STEP_2(1, 3, i + 211);
          STEP_2(4, 14, i + 212);
          STEP_2(1, 5, i + 213);
          STEP_2(2, 6, i + 214);
          STEP_2(1, 3, i + 215);
          STEP_2(7, 15, i + 216);
          STEP_2(1, 8, i + 217);
          STEP_2(2, 9, i + 218);
          STEP_2(1, 3, i + 219);
          STEP_2(4, 10, i + 220);
          STEP_2(1, 5, i + 221);
          STEP_2(2, 6, i + 222);
          STEP_2(1, 3, i + 223);
          STEP_2(16, 28, i + 224);
          STEP_2(1, 17, i + 225);
          STEP_2(2, 18, i + 226);
          STEP_2(1, 3, i + 227);
          STEP_2(4, 19, i + 228);
          STEP_2(1, 5, i + 229);
          STEP_2(2, 6, i + 230);
          STEP_2(1, 3, i + 231);
          STEP_2(7, 20, i + 232);
          STEP_2(1, 8, i + 233);
          STEP_2(2, 9, i + 234);
          STEP_2(1, 3, i + 235);
          STEP_2(4, 10, i + 236);
          STEP_2(1, 5, i + 237);
          STEP_2(2, 6, i + 238);
          STEP_2(1, 3, i + 239);
          STEP_2(11, 21, i + 240);
          STEP_2(1, 12, i + 241);
          STEP_2(2, 13, i + 242);
          STEP_2(1, 3, i + 243);
          STEP_2(4, 14, i + 244);
          STEP_2(1, 5, i + 245);
          STEP_2(2, 6, i + 246);
          STEP_2(1, 3, i + 247);
          STEP_2(7, 15, i + 248);
          STEP_2(1, 8, i + 249);
          STEP_2(2, 9, i + 250);
          STEP_2(1, 3, i + 251);
          STEP_2(4, 10, i + 252);
          STEP_2(1, 5, i + 253);
          STEP_2(2, 6, i + 254);
          STEP_2(1, 3, i + 255);
          STEP_2(37, 9 + alpha, i + 256);
          STEP_2(1, 38, i + 257);
          STEP_2(2, 39, i + 258);
          STEP_2(1, 3, i + 259);
          STEP_2(4, 40, i + 260);
          STEP_2(1, 5, i + 261);
          STEP_2(2, 6, i + 262);
          STEP_2(1, 3, i + 263);
          STEP_2(7, 41, i + 264);
          STEP_2(1, 8, i + 265);
          STEP_2(2, 9, i + 266);
          STEP_2(1, 3, i + 267);
          STEP_2(4, 10, i + 268);
          STEP_2(1, 5, i + 269);
          STEP_2(2, 6, i + 270);
          STEP_2(1, 3, i + 271);
          STEP_2(11, 42, i + 272);
          STEP_2(1, 12, i + 273);
          STEP_2(2, 13, i + 274);
          STEP_2(1, 3, i + 275);
          STEP_2(4, 14, i + 276);
          STEP_2(1, 5, i + 277);
          STEP_2(2, 6, i + 278);
          STEP_2(1, 3, i + 279);
          STEP_2(7, 15, i + 280);
          STEP_2(1, 8, i + 281);
          STEP_2(2, 9, i + 282);
          STEP_2(1, 3, i + 283);
          STEP_2(4, 10, i + 284);
          STEP_2(1, 5, i + 285);
          STEP_2(2, 6, i + 286);
          STEP_2(1, 3, i + 287);
          STEP_2(16, 43, i + 288);
          STEP_2(1, 17, i + 289);
          STEP_2(2, 18, i + 290);
          STEP_2(1, 3, i + 291);
          STEP_2(4, 19, i + 292);
          STEP_2(1, 5, i + 293);
          STEP_2(2, 6, i + 294);
          STEP_2(1, 3, i + 295);
          STEP_2(7, 20, i + 296);
          STEP_2(1, 8, i + 297);
          STEP_2(2, 9, i + 298);
          STEP_2(1, 3, i + 299);
          STEP_2(4, 10, i + 300);
          STEP_2(1, 5, i + 301);
          STEP_2(2, 6, i + 302);
          STEP_2(1, 3, i + 303);
          STEP_2(11, 21, i + 304);
          STEP_2(1, 12, i + 305);
          STEP_2(2, 13, i + 306);
          STEP_2(1, 3, i + 307);
          STEP_2(4, 14, i + 308);
          STEP_2(1, 5, i + 309);
          STEP_2(2, 6, i + 310);
          STEP_2(1, 3, i + 311);
          STEP_2(7, 15, i + 312);
          STEP_2(1, 8, i + 313);
          STEP_2(2, 9, i + 314);
          STEP_2(1, 3, i + 315);
          STEP_2(4, 10, i + 316);
          STEP_2(1, 5, i + 317);
          STEP_2(2, 6, i + 318);
          STEP_2(1, 3, i + 319);
          STEP_2(22, 44, i + 320);
          STEP_2(1, 23, i + 321);
          STEP_2(2, 24, i + 322);
          STEP_2(1, 3, i + 323);
          STEP_2(4, 25, i + 324);
          STEP_2(1, 5, i + 325);
          STEP_2(2, 6, i + 326);
          STEP_2(1, 3, i + 327);
          STEP_2(7, 26, i + 328);
          STEP_2(1, 8, i + 329);
          STEP_2(2, 9, i + 330);
          STEP_2(1, 3, i + 331);
          STEP_2(4, 10, i + 332);
          STEP_2(1, 5, i + 333);
          STEP_2(2, 6, i + 334);
          STEP_2(1, 3, i + 335);
          STEP_2(11, 27, i + 336);
          STEP_2(1, 12, i + 337);
          STEP_2(2, 13, i + 338);
          STEP_2(1, 3, i + 339);
          STEP_2(4, 14, i + 340);
          STEP_2(1, 5, i + 341);
          STEP_2(2, 6, i + 342);
          STEP_2(1, 3, i + 343);
          STEP_2(7, 15, i + 344);
          STEP_2(1, 8, i + 345);
          STEP_2(2, 9, i + 346);
          STEP_2(1, 3, i + 347);
          STEP_2(4, 10, i + 348);
          STEP_2(1, 5, i + 349);
          STEP_2(2, 6, i + 350);
          STEP_2(1, 3, i + 351);
          STEP_2(16, 28, i + 352);
          STEP_2(1, 17, i + 353);
          STEP_2(2, 18, i + 354);
          STEP_2(1, 3, i + 355);
          STEP_2(4, 19, i + 356);
          STEP_2(1, 5, i + 357);
          STEP_2(2, 6, i + 358);
          STEP_2(1, 3, i + 359);
          STEP_2(7, 20, i + 360);
          STEP_2(1, 8, i + 361);
          STEP_2(2, 9, i + 362);
          STEP_2(1, 3, i + 363);
          STEP_2(4, 10, i + 364);
          STEP_2(1, 5, i + 365);
          STEP_2(2, 6, i + 366);
          STEP_2(1, 3, i + 367);
          STEP_2(11, 21, i + 368);
          STEP_2(1, 12, i + 369);
          STEP_2(2, 13, i + 370);
          STEP_2(1, 3, i + 371);
          STEP_2(4, 14, i + 372);
          STEP_2(1, 5, i + 373);
          STEP_2(2, 6, i + 374);
          STEP_2(1, 3, i + 375);
          STEP_2(7, 15, i + 376);
          STEP_2(1, 8, i + 377);
          STEP_2(2, 9, i + 378);
          STEP_2(1, 3, i + 379);
          STEP_2(4, 10, i + 380);
          STEP_2(1, 5, i + 381);
          STEP_2(2, 6, i + 382);
          STEP_2(1, 3, i + 383);
          STEP_2(29, 45, i + 384);
          STEP_2(1, 30, i + 385);
          STEP_2(2, 31, i + 386);
          STEP_2(1, 3, i + 387);
          STEP_2(4, 32, i + 388);
          STEP_2(1, 5, i + 389);
          STEP_2(2, 6, i + 390);
          STEP_2(1, 3, i + 391);
          STEP_2(7, 33, i + 392);
          STEP_2(1, 8, i + 393);
          STEP_2(2, 9, i + 394);
          STEP_2(1, 3, i + 395);
          STEP_2(4, 10, i + 396);
          STEP_2(1, 5, i + 397);
          STEP_2(2, 6, i + 398);
          STEP_2(1, 3, i + 399);
          STEP_2(11, 34, i + 400);
          STEP_2(1, 12, i + 401);
          STEP_2(2, 13, i + 402);
          STEP_2(1, 3, i + 403);
          STEP_2(4, 14, i + 404);
          STEP_2(1, 5, i + 405);
          STEP_2(2, 6, i + 406);
          STEP_2(1, 3, i + 407);
          STEP_2(7, 15, i + 408);
          STEP_2(1, 8, i + 409);
          STEP_2(2, 9, i + 410);
          STEP_2(1, 3, i + 411);
          STEP_2(4, 10, i + 412);
          STEP_2(1, 5, i + 413);
          STEP_2(2, 6, i + 414);
          STEP_2(1, 3, i + 415);
          STEP_2(16, 35, i + 416);
          STEP_2(1, 17, i + 417);
          STEP_2(2, 18, i + 418);
          STEP_2(1, 3, i + 419);
          STEP_2(4, 19, i + 420);
          STEP_2(1, 5, i + 421);
          STEP_2(2, 6, i + 422);
          STEP_2(1, 3, i + 423);
          STEP_2(7, 20, i + 424);
          STEP_2(1, 8, i + 425);
          STEP_2(2, 9, i + 426);
          STEP_2(1, 3, i + 427);
          STEP_2(4, 10, i + 428);
          STEP_2(1, 5, i + 429);
          STEP_2(2, 6, i + 430);
          STEP_2(1, 3, i + 431);
          STEP_2(11, 21, i + 432);
          STEP_2(1, 12, i + 433);
          STEP_2(2, 13, i + 434);
          STEP_2(1, 3, i + 435);
          STEP_2(4, 14, i + 436);
          STEP_2(1, 5, i + 437);
          STEP_2(2, 6, i + 438);
          STEP_2(1, 3, i + 439);
          STEP_2(7, 15, i + 440);
          STEP_2(1, 8, i + 441);
          STEP_2(2, 9, i + 442);
          STEP_2(1, 3, i + 443);
          STEP_2(4, 10, i + 444);
          STEP_2(1, 5, i + 445);
          STEP_2(2, 6, i + 446);
          STEP_2(1, 3, i + 447);
          STEP_2(22, 36, i + 448);
          STEP_2(1, 23, i + 449);
          STEP_2(2, 24, i + 450);
          STEP_2(1, 3, i + 451);
          STEP_2(4, 25, i + 452);
          STEP_2(1, 5, i + 453);
          STEP_2(2, 6, i + 454);
          STEP_2(1, 3, i + 455);
          STEP_2(7, 26, i + 456);
          STEP_2(1, 8, i + 457);
          STEP_2(2, 9, i + 458);
          STEP_2(1, 3, i + 459);
          STEP_2(4, 10, i + 460);
          STEP_2(1, 5, i + 461);
          STEP_2(2, 6, i + 462);
          STEP_2(1, 3, i + 463);
          STEP_2(11, 27, i + 464);
          STEP_2(1, 12, i + 465);
          STEP_2(2, 13, i + 466);
          STEP_2(1, 3, i + 467);
          STEP_2(4, 14, i + 468);
          STEP_2(1, 5, i + 469);
          STEP_2(2, 6, i + 470);
          STEP_2(1, 3, i + 471);
          STEP_2(7, 15, i + 472);
          STEP_2(1, 8, i + 473);
          STEP_2(2, 9, i + 474);
          STEP_2(1, 3, i + 475);
          STEP_2(4, 10, i + 476);
          STEP_2(1, 5, i + 477);
          STEP_2(2, 6, i + 478);
          STEP_2(1, 3, i + 479);
          STEP_2(16, 28, i + 480);
          STEP_2(1, 17, i + 481);
          STEP_2(2, 18, i + 482);
          STEP_2(1, 3, i + 483);
          STEP_2(4, 19, i + 484);
          STEP_2(1, 5, i + 485);
          STEP_2(2, 6, i + 486);
          STEP_2(1, 3, i + 487);
          STEP_2(7, 20, i + 488);
          STEP_2(1, 8, i + 489);
          STEP_2(2, 9, i + 490);
          STEP_2(1, 3, i + 491);
          STEP_2(4, 10, i + 492);
          STEP_2(1, 5, i + 493);
          STEP_2(2, 6, i + 494);
          STEP_2(1, 3, i + 495);
          STEP_2(11, 21, i + 496);
          STEP_2(1, 12, i + 497);
          STEP_2(2, 13, i + 498);
          STEP_2(1, 3, i + 499);
          STEP_2(4, 14, i + 500);
          STEP_2(1, 5, i + 501);
          STEP_2(2, 6, i + 502);
          STEP_2(1, 3, i + 503);
          STEP_2(7, 15, i + 504);
          STEP_2(1, 8, i + 505);
          STEP_2(2, 9, i + 506);
          STEP_2(1, 3, i + 507);
          STEP_2(4, 10, i + 508);
          STEP_2(1, 5, i + 509);
          STEP_2(2, 6, i + 510);
          STEP_2(1, 3, i + 511);

          CHECK_SOLUTIONS();
      }

  }
  FLUSH_SOLUTIONS();
  uint64_t end_time = rdtsc();
 if (verbose) printf("fes: enumeration+check = %" PRIu64 " cycles\n", end_time - enumeration_start_time);
  QUIT();
}


uint64_t timeSecondStep = 0;


void verbose_print(wrapper_settings_t *settings, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  if ( settings->verbose ) {
    fprintf(stderr, "fes: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
  }
  va_end(args);
}

wrapper_settings_t *init_settings() {
  wrapper_settings_t *result = (wrapper_settings_t *)malloc( sizeof(wrapper_settings_t) );
  if ( result == NULL ) {
    err(EX_OSERR, "impossible to allocate the settings for the wrapper");
  }
  result->word_size = 32;
  result->algorithm = ALGO_AUTO;
  result->algo_auto_degree_bound = 10;

  result->algo_enum_self_tune = 1;

#ifdef HAVE_SSE2
  result->algo_enum_use_sse = 1;
#else
  result->algo_enum_use_sse = 0;
#endif

  result->verbose = 0;
  return result;
}


void choose_settings( wrapper_settings_t *s, int n, int n_eqs, int degree) {
  // choose algorithm
  assert(n_eqs > 0);
  if ( s->algorithm == ALGO_AUTO ) {
    if (degree < s->algo_auto_degree_bound) {
      verbose_print(s, "low degree (%d) --> using enumeration code [threshold=%d]", degree, s->algo_auto_degree_bound);
      s->algorithm = ALGO_ENUMERATION;
    } else {
      verbose_print(s, "''large'' degree (%d) --> using FFT evaluation");
      s->algorithm = ALGO_FFT;
    }

    if( s->algorithm == ALGO_ENUMERATION && s->algo_enum_self_tune ) {
      if ( degree == 2 ) {
	verbose_print(s, "very small degree, using 16-bit words");
	s->word_size = 16;
      }
      if ( n < SIMD_CHUNK_SIZE + 2 ) {
	verbose_print(s, "too few variables (%d), disabling sse assembly code [threshold=%d]", n, SIMD_CHUNK_SIZE + 2);
	s->algo_enum_use_sse = 0;
      } else {
	verbose_print(s, "Using SIMD code (sse2 instructions available)");
      }
    }
  }
}


// ---------- convert the input given by the SAGE interface to the right format
void next_set(int n, int d, int set[]) {
  if (d == 0) return;
  set[0] += 1;
  if (set[0] == n) {
    next_set(n-1, d-1, &set[1]);
    if (d > 0) set[0] = set[1] + 1;
  }
}

//  assumes that F (the target array) is already allocated
void convert_input_equations(const int n, const int degree, int from, int to, int ***coeffs, idx_lut_t *idx_LUT, pck_vector_t F[]) {

  assert(to-from <= (int) (8*sizeof(pck_vector_t)));
  vector_t x = init_vector(to-from);   // this is used to pack the equations in memory words

  int set[ n ]; // represent the monomial `m` enumerated below
  for(int j=0; j<n; j++) {
    set[j] = -1;
  }
  for(int d=0; d<degree+1; d++) {   // collect degree-d terms
    for(int j=0; j<d; j++) {
      set[j] = d-1-j;
    }
    for(uint64_t m=0; m<binomials[n][d]; m++) { // iterates over all monomials of degree d
      // loop invariant: `set` describes the m-th monomial

      for(int e=from; e<to; e++) { // skim through all the equations
	x[e-from] = coeffs[e][d][m];
      }
      F[set2int( idx_LUT, set) ] = pack(to-from, x);

      next_set(n, n, &set[0]); // maintain invariant
    }
  }
  free(x);
}


// --------------------------------------------------------------------------------------------------

// this callback is used when there are more than 32 equations
int solution_tester(void *_state, uint64_t size, uint64_t* n_solutions) {
  wrapper_state_t * state = (wrapper_state_t *) _state;
  uint64_t start = rdtsc();

  assert( state->degree < enumerated_degree_bound); // enumerated_degree_bound is defined in fes.h

  uint64_t* corrects_solutions = (uint64_t*)malloc(sizeof(uint64_t)*size);
  uint64_t current_solution;
  int index_correct_solution = 0;
  int is_correct;
  int j;

  for(uint64_t i=0; i<size; i++){
    is_correct = 1;
    j = 0;
    current_solution = n_solutions[i];
    while(is_correct && j<(state->n_batches)){
      if (packed_eval(state->testing_LUT->LUT, state->n, state->degree, state->G[j], current_solution) != 0)
	is_correct = 0;
      j++;
    }
    if (is_correct){
      corrects_solutions[index_correct_solution] = current_solution;
      index_correct_solution++;
    }
  }

  timeSecondStep += (rdtsc() - start);

  if (index_correct_solution == 0)
    return 0;   // no new solution found, keep searching

  // report solution to the actual callback, and ask whether it wants to keep going
  return (*(state->callback))(state->callback_state, index_correct_solution, corrects_solutions);
}

// --------------------------------------------------------------------------------------
void moebius_wrapper(int n, pck_vector_t F[], solution_callback_t callback, void* callback_state, wrapper_settings_t *settings) {
  verbose_print(settings, "running FFT");
  moebius_transform(n, F, callback, callback_state);
}


// ------------------------------------------------
void enumeration_wrapper(LUT_t LUT, int n, int d, pck_vector_t F[], solution_callback_t callback, void* callback_state, wrapper_settings_t *settings ) {

  // TODO : this should probably also include a run-time check that SSE2 instructions are actually there

  //if ( !settings->algo_enum_use_sse ) {
  if (1) {
    switch (d) {
    case 2: exhaustive_ia32_deg_2(LUT, n, F, callback, callback_state, settings->verbose); break;
    default:
      assert(0);
    }
  } else {
    if ( settings->word_size == 32 ) {
      switch (d) {
      //case 2: exhaustive_sse2_deg_2_T_2_el_0(LUT, n, F, callback, callback_state, settings->verbose); break;
      default:
	assert(0);
      }
    }
    else if ( settings->word_size == 16 ){
      switch (d) {
      //case 2: exhaustive_sse2_deg_2_T_3_el_0(LUT, n, F, callback, callback_state, settings->verbose); break;
      default:
	assert(0);
      }
    }
    else if ( settings->word_size == 8 ){
      switch (d) {
      //case 2: exhaustive_sse2_deg_2_T_4_el_0(LUT, n, F, callback, callback_state, settings->verbose); break;
      default:
	assert(0);
      }
    }
  }
}


// -------------------------------------

void exhaustive_search_wrapper(const int n, int n_eqs, const int degree, int ***coeffs, solution_callback_t callback, void* callback_state, wrapper_settings_t *settings ) {
  wrapper_state_t * tester_state = NULL;

  if (settings == NULL) {
    settings = init_settings();
  }
  choose_settings(settings, n, n_eqs, degree);

  if (n > 64) {
    fprintf(stderr, "fes: Your equations have more than 64 variables, but the FES library handles at most 64.\n");
    fprintf(stderr, "fes: You may want to specialize (n-64) variables to fit into the constraints.\n");
    fprintf(stderr, "fes: (also, do you realize that it will take a **LONG** time?)\n");
    fprintf(stderr, "fes: aborting.\n");
    return;
  }

  //bool must_free_tester_state = false;
  const uint64_t N = n_monomials(n, degree);
  //  int enumerated_equations = 128 >> (T);

  // --------- allocate/initialize some of our data structures
  pck_vector_t *F = NULL;
  idx_lut_t* idx_LUT = NULL;
  size_t F_size = -1;

  switch( settings->algorithm ) {
  case ALGO_ENUMERATION:
    idx_LUT = init_deginvlex_LUT(n, degree);
    F_size = N;
    break;

  case ALGO_FFT:
    idx_LUT = init_lex_LUT(n);
    F_size = 1 << n;
    break;

  default:
    err(EX_OSERR, "internal bug (settings not chosen ?!?)");
  }

  F = (pck_vector_t *)malloc(F_size * sizeof(pck_vector_t));
  if (F == NULL) {
    err(EX_OSERR, "[fes/wrapper/enumeration/allocate F]");
  }

  // ---------- deal where the case where there is more equations than what the kernel(s) deals with

  if ( n_eqs <= settings->word_size ) {
    // this is the simple case where the enumeration itself is enough
    // prepare the input for the enumeration
    verbose_print(settings, "wordsize (=%d) >= #equations (=%d) --> using core fixed-size algorithm directly", settings->word_size, n_eqs );
    convert_input_equations( n, degree, 0, n_eqs, coeffs, idx_LUT, F );

  } else {
     // if there are more equations that what we can enumerate simultaneously,
     // we just deal with the first `enumerated_equations`, and then check
     // any eventual solutions of these against the remaining equations

     verbose_print(settings, "wordsize (=%d) < #equations (=%d) --> wrapping tester around core fixed-size algorithm directly", settings->word_size, n_eqs );

     // we split the equations into "batches" of `settings->word_size` each
     int n_batches = n_eqs / settings->word_size;
     if ( (n_eqs % settings->word_size) > 0 ) {
       n_batches++;
     }

     // the first batch goes into the enumeration code
     convert_input_equations(n, degree, 0, settings->word_size, coeffs, idx_LUT, F); // prepare the input for the enumeration

     // the next batches will be used by the tester. They must be in deginvlex order
     idx_lut_t *testing_LUT = idx_LUT;
     if (settings->algorithm == ALGO_FFT) {
       testing_LUT = init_deginvlex_LUT(n, degree);
     }

     pck_vector_t **G = NULL;
     G = (pck_vector_t **)calloc(n_batches-1, sizeof(pck_vector_t *));
     if (G == NULL) {
       err(EX_OSERR, "[fes/wrapper/enumeration/allocate G]");
     }

     for(int i=1; i<n_batches; i++) {
       G[i-1] = (pck_vector_t *)calloc(N, sizeof(pck_vector_t));
       if (G[i-1] == NULL) {
	     err(EX_OSERR, "[fes/wrapper/enumeration/allocate G[i]]");
       }
       convert_input_equations(n, degree, settings->word_size*i, min(n_eqs, settings->word_size*(i+1)), coeffs, testing_LUT, G[i-1]);
     }

    // the "tester" needs some internal state
    if ( ( tester_state = (wrapper_state_t *)malloc( sizeof(wrapper_state_t) ) ) == NULL) {
       err(EX_OSERR, "[fes/wrapper/allocate wrapper]");
    }
    tester_state->n = n;
    tester_state->degree = degree;
    tester_state->n_batches = n_batches-1;
    tester_state->G = G;
    tester_state->testing_LUT = testing_LUT;

    tester_state->callback = callback;
    tester_state->callback_state = callback_state;
    //must_free_tester_state = true;

    callback = solution_tester;
    callback_state = (void *) tester_state;
  }

  // ------------ start actual computation
  verbose_print(settings, "starting kernel");
  uint64_t start = rdtsc();

  switch( settings->algorithm) {
  case ALGO_ENUMERATION:
    enumeration_wrapper(idx_LUT->LUT, n, degree, F, callback, callback_state, settings);
    break;
  case ALGO_FFT:
    moebius_wrapper(n, F, callback, callback_state, settings);
    break;
  }

  uint64_t totalTime = rdtsc() - start;
  float sizeSecondStep = (timeSecondStep * 100.0) / totalTime;
  verbose_print(settings, "%.2f CPU cycles/candidate solution", totalTime * 1.0 / (1ll << n));
  verbose_print(settings, "Enumeration : %.2f%% , Verification : \%.2f%%", 100-sizeSecondStep, sizeSecondStep);

  // ----------- clean up

  if ( tester_state != NULL ) {
    // TODO HERE -- must free G
    if ( settings->algorithm == ALGO_FFT ) {
      free_LUT( tester_state->testing_LUT );
    }
    free( tester_state );
  }

  free_LUT( idx_LUT );
  free( F );
}

int mcopy;
int ncopy;
uint64_t solm;
uint64_t **SolMerge;
uint64_t SolCount;
uint64_t MaxSolCount;
uint64_t *MaskCopy;

int C(int n, int m) {
	if (m == 0)
		return 1;
	else if (m == 1)
		return n;
	else if (m == 2)
		return n * (n - 1) >> 1;
	else
		return 0;
}

int M(uint64_t *Mask, int index) {
	if (index < 64)
		return (Mask[0] >> index) & 1;
	else
		return (Mask[1] >> (index - 64)) & 1;
}

static int Merge_Solution(void *unused, uint64_t count, uint64_t *Sol) {
	assert(unused == NULL);
	while (count > 0 && SolCount < MaxSolCount) {
		// Merge m-bit solution and (n-m)-bit solution.
		SolMerge[SolCount][0] = (Sol[count-1] << mcopy) ^ solm;
		if (mcopy > 0)
			SolMerge[SolCount][1] = Sol[count-1] >> (64 - mcopy);
		// Unmask solution.
		if (ncopy < 64)
			SolMerge[SolCount][0] ^= (MaskCopy[0] << (64 - ncopy)) >> (64 - ncopy);
		else {
			SolMerge[SolCount][0] ^= MaskCopy[0];
			SolMerge[SolCount][1] ^= (MaskCopy[1] << (128 - ncopy)) >> (128 - ncopy);
		}
		SolCount += 1;
		count -= 1;
		return 1; // Early aborb.
	}
	return 0;
}

void exfes(int m, int n, int e, uint64_t *Mask, uint64_t maxsol, int ***EqsUnmask, uint64_t **SolArray) {
	wrapper_settings_t *Settings = init_settings();
	mcopy = m;
	ncopy = n;
	SolMerge = SolArray;
	SolCount = 0;
	MaxSolCount = maxsol;
	MaskCopy = Mask;
	// Make a copy of EqsUnmask for masking.
	int ***Eqs = (int ***)calloc(e, sizeof(int **));
	for (int i=0; i<e; i++) {
		Eqs[i] = (int **)calloc(3, sizeof(int *));
		for (int j=0; j<3; j++) {
			Eqs[i][j] = (int *)malloc(C(n, j) * sizeof(int));
			for (int k=0; k<C(n, j); k++)
				Eqs[i][j][k] = EqsUnmask[i][j][k];
		}
	}
	// Mask Eqs for a random start point.
	for (int i=0; i<e; i++) {
		for (int j=0; j<n; j++)
			Eqs[i][0][0] ^= Eqs[i][1][j] & M(Mask, j);
		int offset = 0;
		for (int j=0; j<n-1; j++)
			for (int k=j+1; k<n; k++) {
				Eqs[i][0][0] ^= Eqs[i][2][offset] & M(Mask, j) & M(Mask, k);
				Eqs[i][1][j] ^= Eqs[i][2][offset] & M(Mask, k);
				Eqs[i][1][k] ^= Eqs[i][2][offset] & M(Mask, j);
				offset += 1;
			}
	}
	// Make a copy of Eqs for evaluating fixed variables.
	int *** EqsCopy = (int ***)calloc(e, sizeof(int **));
	for (int i=0; i<e; i++) {
		EqsCopy[i] = (int **)calloc(3, sizeof(int *));
		for (int j=0; j<3; j++)
			EqsCopy[i][j] = (int *)malloc(C(n, j) * sizeof(int));
	}
	// Partition problem into (1<<n_fixed) sub_problems.
	int p = n - m;
	int ncopy;
	int fixvalue;
	for (solm=0; solm<(uint64_t)1<<m; solm++) {
		// Initialize ncopy and EqsCopy.
		ncopy = n;
		for (int i=0; i<e; i++)
			for (int j=0; j<3; j++)
				for (int k=0; k<C(n, j); k++)
					EqsCopy[i][j][k] = Eqs[i][j][k];
		// Fix m variables.
		while (ncopy != p) {
			fixvalue = (solm >> (n - ncopy)) & 1;
			for (int i=0; i<e; i++) {
				// Fix a variable.
				for (int j=0; j<ncopy-1; j++)
					EqsCopy[i][1][j+1] ^= EqsCopy[i][2][j] & fixvalue;
				EqsCopy[i][0][0] ^= EqsCopy[i][1][0] & fixvalue;
				// Shrink EqsCopy.
				for (int j=0; j<ncopy-1; j++)
					EqsCopy[i][1][j] = EqsCopy[i][1][j+1];
				for (int j=0; j<C(ncopy-1, 2); j++)
					EqsCopy[i][2][j] = EqsCopy[i][2][j+ncopy-1];
			}
			ncopy -= 1;
		}
		exhaustive_search_wrapper(ncopy, e, 2, EqsCopy, Merge_Solution, NULL, Settings);
		// Determine to early aborb or not.
		if (SolCount == 1)
			return;
	}
}

#ifdef __cplusplus

}

#endif
