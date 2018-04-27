#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <emmintrin.h>
typedef __m128i simd_t;

#include "fes.h"
#include "idx_LUT.h"

#define STEP_0(i) { \
  const simd_t Mask = _mm_cmpeq_epi16(F[ 0 ], simd_zero); \
  const int mask = _mm_movemask_epi8(Mask);  \
  if (unlikely(mask)) { \
      solution_buffer[n_solutions_found].int_idx = i; \
      solution_buffer[n_solutions_found].mask = mask; \
      n_solutions_found++; \
   }\
}

#define STEP_1(a,i) { \
  F[ 0 ] ^= F [ a ]; \
  const simd_t Mask = _mm_cmpeq_epi16(F[ 0 ], simd_zero); \
  const int mask = _mm_movemask_epi8(Mask);  \
  if (unlikely(mask)) { \
      solution_buffer[n_solutions_found].int_idx = i; \
      solution_buffer[n_solutions_found].mask = mask; \
      n_solutions_found++; \
   }\
}

#define STEP_2(a,b,i) { \
  F[ a ] ^= F [ b ]; \
  F[ 0 ] ^= F [ a ]; \
  const simd_t Mask = _mm_cmpeq_epi16(F[ 0 ], simd_zero); \
  const int mask = _mm_movemask_epi8(Mask);  \
  if (unlikely(mask)) { \
      solution_buffer[n_solutions_found].int_idx = i; \
      solution_buffer[n_solutions_found].mask = mask; \
      n_solutions_found++; \
   }\
}


typedef struct {
  uint64_t int_idx;
  uint32_t mask;
} solution_t;

// generated with L = 9
void exhaustive_sse2_deg_2_T_3_el_0(LUT_t LUT, int n, pck_vector_t nonsimd_F[], solution_callback_t callback, void* callback_state, int verbose) {
  const __m128i simd_zero = _mm_setzero_si128();

  // *****initialise an SIMD version of the constants and internal state
  const int N = n_monomials(n, 2);
  simd_t * F = NULL;
  int foo = posix_memalign((void **) &F, sizeof(simd_t), N*sizeof(simd_t));
  if (foo != 0 || F == NULL) {
    perror("[fes/enumeration/simd]");
    return;
  }
  for(int i=0; i<N; i++)
    F[i] = _mm_set1_epi16(nonsimd_F[i]);  // expand all the coefficients into SIMD vectors

  uint64_t everything_start_time = rdtsc();
  // we want to specialize each one of the 8 SIMD-thread to a combination of the last 3 variables
  simd_t v[3];
  v[0] = _mm_set_epi32(0x0000ffff,0x0000ffff,0x0000ffff,0x0000ffff);
  v[1] = _mm_set_epi32(0x00000000,0xffffffff,0x00000000,0xffffffff);
  v[2] = _mm_set_epi32(0x00000000,0x00000000,0xffffffff,0xffffffff);

  // updates degree-0 term with degree-1 terms
  for(int a=0; a<3; a++)
    F[ 0 ] ^= F[ idx_1(LUT, n-3+a) ] & (v[a]);

  // updates degree-0 term with degree-2 terms
  for(int b=0; b<3; b++)
    for(int a=0; a<b; a++)
      F[ 0 ] ^= F[ idx_2(LUT, n-3+a, n-3+b) ] & (v[a] & v[b]);

  // updates degree-1 term with degree-2 terms
  for(int idx_0=0; idx_0<n-2; idx_0++)
    for(int a=0; a<3; a++)
      F[ idx_1(LUT, idx_0) ] ^= F[ idx_2(LUT, idx_0, n-3+a) ] & (v[a]);

  if (verbose) printf("fes: specialization = %" PRIu64 " cycles\n", rdtsc()-everything_start_time);
  #define QUIT() { \
    if (F != NULL) free(F); \
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
      if (solution_buffer[i].mask & 0x0003) \
        PUSH_SOLUTION(to_gray(solution_buffer[i].int_idx) + 7*(1ll << (n-3))); \
      if (solution_buffer[i].mask & 0x000c) \
        PUSH_SOLUTION(to_gray(solution_buffer[i].int_idx) + 6*(1ll << (n-3))); \
      if (solution_buffer[i].mask & 0x0030) \
        PUSH_SOLUTION(to_gray(solution_buffer[i].int_idx) + 5*(1ll << (n-3))); \
      if (solution_buffer[i].mask & 0x00c0) \
        PUSH_SOLUTION(to_gray(solution_buffer[i].int_idx) + 4*(1ll << (n-3))); \
      if (solution_buffer[i].mask & 0x0300) \
        PUSH_SOLUTION(to_gray(solution_buffer[i].int_idx) + 3*(1ll << (n-3))); \
      if (solution_buffer[i].mask & 0x0c00) \
        PUSH_SOLUTION(to_gray(solution_buffer[i].int_idx) + 2*(1ll << (n-3))); \
      if (solution_buffer[i].mask & 0x3000) \
        PUSH_SOLUTION(to_gray(solution_buffer[i].int_idx) + 1*(1ll << (n-3))); \
      if (solution_buffer[i].mask & 0xc000) \
        PUSH_SOLUTION(to_gray(solution_buffer[i].int_idx) + 0*(1ll << (n-3))); \
    } \
    n_solutions_found = 0; \
  }

  // special case for i=0
  const uint64_t weight_0_start = 0;
  STEP_0(0);

  // from now on, hamming weight is >= 1
  for(int idx_0=0; idx_0<n-3; idx_0++) {

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
          uint64_t F_sp[ 1 ] = { alpha*16 };
          func_deg_2_T_3_el_0(F, &F_sp[0], solution_buffer, &n_solutions_found, i);

          CHECK_SOLUTIONS();
      }

  }
  FLUSH_SOLUTIONS();
  uint64_t end_time = rdtsc();
 if (verbose) printf("fes: enumeration+check = %" PRIu64 " cycles\n", end_time - enumeration_start_time);
  QUIT();
}
