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

#define STEP_3(a,b,c,i) { \
  F[ b ] ^= F [ c ]; \
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

#define STEP_4(a,b,c,d,i) { \
  F[ c ] ^= F [ d ]; \
  F[ b ] ^= F [ c ]; \
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

#define STEP_5(a,b,c,d,e,i) { \
  F[ d ] ^= F [ e ]; \
  F[ c ] ^= F [ d ]; \
  F[ b ] ^= F [ c ]; \
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

#define STEP_6(a,b,c,d,e,f,i) { \
  F[ e ] ^= F [ f ]; \
  F[ d ] ^= F [ e ]; \
  F[ c ] ^= F [ d ]; \
  F[ b ] ^= F [ c ]; \
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

#define STEP_7(a,b,c,d,e,f,g,i) { \
  F[ f ] ^= F [ g ]; \
  F[ e ] ^= F [ f ]; \
  F[ d ] ^= F [ e ]; \
  F[ c ] ^= F [ d ]; \
  F[ b ] ^= F [ c ]; \
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

#define STEP_8(a,b,c,d,e,f,g,h,i) { \
  F[ g ] ^= F [ h ]; \
  F[ f ] ^= F [ g ]; \
  F[ e ] ^= F [ f ]; \
  F[ d ] ^= F [ e ]; \
  F[ c ] ^= F [ d ]; \
  F[ b ] ^= F [ c ]; \
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

#define STEP_9(a,b,c,d,e,f,g,h,i0,i) { \
  F[ h ] ^= F [ i0 ]; \
  F[ g ] ^= F [ h ]; \
  F[ f ] ^= F [ g ]; \
  F[ e ] ^= F [ f ]; \
  F[ d ] ^= F [ e ]; \
  F[ c ] ^= F [ d ]; \
  F[ b ] ^= F [ c ]; \
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

// generated with L = 6
void exhaustive_sse2_deg_9_T_3_el_0(LUT_t LUT, int n, pck_vector_t nonsimd_F[], solution_callback_t callback, void* callback_state, int verbose) {
  const __m128i simd_zero = _mm_setzero_si128();

  // *****initialise an SIMD version of the constants and internal state
  const int N = n_monomials(n, 9);
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

  // updates degree-0 term with degree-3 terms
  for(int c=0; c<3; c++)
    for(int b=0; b<c; b++)
      for(int a=0; a<b; a++)
        F[ 0 ] ^= F[ idx_3(LUT, n-3+a, n-3+b, n-3+c) ] & (v[a] & v[b] & v[c]);

  // updates degree-1 term with degree-3 terms
  for(int idx_0=0; idx_0<n-2; idx_0++)
    for(int b=0; b<3; b++)
      for(int a=0; a<b; a++)
        F[ idx_1(LUT, idx_0) ] ^= F[ idx_3(LUT, idx_0, n-3+a, n-3+b) ] & (v[a] & v[b]);

  // updates degree-2 term with degree-3 terms
  for(int idx_1=0; idx_1<n-2; idx_1++)
    for(int idx_0=0; idx_0<idx_1; idx_0++)
      for(int a=0; a<3; a++)
        F[ idx_2(LUT, idx_0, idx_1) ] ^= F[ idx_3(LUT, idx_0, idx_1, n-3+a) ] & (v[a]);

  // updates degree-0 term with degree-4 terms
  for(int d=0; d<3; d++)
    for(int c=0; c<d; c++)
      for(int b=0; b<c; b++)
        for(int a=0; a<b; a++)
          F[ 0 ] ^= F[ idx_4(LUT, n-3+a, n-3+b, n-3+c, n-3+d) ] & (v[a] & v[b] & v[c] & v[d]);

  // updates degree-1 term with degree-4 terms
  for(int idx_0=0; idx_0<n-2; idx_0++)
    for(int c=0; c<3; c++)
      for(int b=0; b<c; b++)
        for(int a=0; a<b; a++)
          F[ idx_1(LUT, idx_0) ] ^= F[ idx_4(LUT, idx_0, n-3+a, n-3+b, n-3+c) ] & (v[a] & v[b] & v[c]);

  // updates degree-2 term with degree-4 terms
  for(int idx_1=0; idx_1<n-2; idx_1++)
    for(int idx_0=0; idx_0<idx_1; idx_0++)
      for(int b=0; b<3; b++)
        for(int a=0; a<b; a++)
          F[ idx_2(LUT, idx_0, idx_1) ] ^= F[ idx_4(LUT, idx_0, idx_1, n-3+a, n-3+b) ] & (v[a] & v[b]);

  // updates degree-3 term with degree-4 terms
  for(int idx_2=0; idx_2<n-2; idx_2++)
    for(int idx_1=0; idx_1<idx_2; idx_1++)
      for(int idx_0=0; idx_0<idx_1; idx_0++)
        for(int a=0; a<3; a++)
          F[ idx_3(LUT, idx_0, idx_1, idx_2) ] ^= F[ idx_4(LUT, idx_0, idx_1, idx_2, n-3+a) ] & (v[a]);

  // updates degree-0 term with degree-5 terms
  for(int e=0; e<3; e++)
    for(int d=0; d<e; d++)
      for(int c=0; c<d; c++)
        for(int b=0; b<c; b++)
          for(int a=0; a<b; a++)
            F[ 0 ] ^= F[ idx_5(LUT, n-3+a, n-3+b, n-3+c, n-3+d, n-3+e) ] & (v[a] & v[b] & v[c] & v[d] & v[e]);

  // updates degree-1 term with degree-5 terms
  for(int idx_0=0; idx_0<n-2; idx_0++)
    for(int d=0; d<3; d++)
      for(int c=0; c<d; c++)
        for(int b=0; b<c; b++)
          for(int a=0; a<b; a++)
            F[ idx_1(LUT, idx_0) ] ^= F[ idx_5(LUT, idx_0, n-3+a, n-3+b, n-3+c, n-3+d) ] & (v[a] & v[b] & v[c] & v[d]);

  // updates degree-2 term with degree-5 terms
  for(int idx_1=0; idx_1<n-2; idx_1++)
    for(int idx_0=0; idx_0<idx_1; idx_0++)
      for(int c=0; c<3; c++)
        for(int b=0; b<c; b++)
          for(int a=0; a<b; a++)
            F[ idx_2(LUT, idx_0, idx_1) ] ^= F[ idx_5(LUT, idx_0, idx_1, n-3+a, n-3+b, n-3+c) ] & (v[a] & v[b] & v[c]);

  // updates degree-3 term with degree-5 terms
  for(int idx_2=0; idx_2<n-2; idx_2++)
    for(int idx_1=0; idx_1<idx_2; idx_1++)
      for(int idx_0=0; idx_0<idx_1; idx_0++)
        for(int b=0; b<3; b++)
          for(int a=0; a<b; a++)
            F[ idx_3(LUT, idx_0, idx_1, idx_2) ] ^= F[ idx_5(LUT, idx_0, idx_1, idx_2, n-3+a, n-3+b) ] & (v[a] & v[b]);

  // updates degree-4 term with degree-5 terms
  for(int idx_3=0; idx_3<n-2; idx_3++)
    for(int idx_2=0; idx_2<idx_3; idx_2++)
      for(int idx_1=0; idx_1<idx_2; idx_1++)
        for(int idx_0=0; idx_0<idx_1; idx_0++)
          for(int a=0; a<3; a++)
            F[ idx_4(LUT, idx_0, idx_1, idx_2, idx_3) ] ^= F[ idx_5(LUT, idx_0, idx_1, idx_2, idx_3, n-3+a) ] & (v[a]);

  // updates degree-0 term with degree-6 terms
  for(int f=0; f<3; f++)
    for(int e=0; e<f; e++)
      for(int d=0; d<e; d++)
        for(int c=0; c<d; c++)
          for(int b=0; b<c; b++)
            for(int a=0; a<b; a++)
              F[ 0 ] ^= F[ idx_6(LUT, n-3+a, n-3+b, n-3+c, n-3+d, n-3+e, n-3+f) ] & (v[a] & v[b] & v[c] & v[d] & v[e] & v[f]);

  // updates degree-1 term with degree-6 terms
  for(int idx_0=0; idx_0<n-2; idx_0++)
    for(int e=0; e<3; e++)
      for(int d=0; d<e; d++)
        for(int c=0; c<d; c++)
          for(int b=0; b<c; b++)
            for(int a=0; a<b; a++)
              F[ idx_1(LUT, idx_0) ] ^= F[ idx_6(LUT, idx_0, n-3+a, n-3+b, n-3+c, n-3+d, n-3+e) ] & (v[a] & v[b] & v[c] & v[d] & v[e]);

  // updates degree-2 term with degree-6 terms
  for(int idx_1=0; idx_1<n-2; idx_1++)
    for(int idx_0=0; idx_0<idx_1; idx_0++)
      for(int d=0; d<3; d++)
        for(int c=0; c<d; c++)
          for(int b=0; b<c; b++)
            for(int a=0; a<b; a++)
              F[ idx_2(LUT, idx_0, idx_1) ] ^= F[ idx_6(LUT, idx_0, idx_1, n-3+a, n-3+b, n-3+c, n-3+d) ] & (v[a] & v[b] & v[c] & v[d]);

  // updates degree-3 term with degree-6 terms
  for(int idx_2=0; idx_2<n-2; idx_2++)
    for(int idx_1=0; idx_1<idx_2; idx_1++)
      for(int idx_0=0; idx_0<idx_1; idx_0++)
        for(int c=0; c<3; c++)
          for(int b=0; b<c; b++)
            for(int a=0; a<b; a++)
              F[ idx_3(LUT, idx_0, idx_1, idx_2) ] ^= F[ idx_6(LUT, idx_0, idx_1, idx_2, n-3+a, n-3+b, n-3+c) ] & (v[a] & v[b] & v[c]);

  // updates degree-4 term with degree-6 terms
  for(int idx_3=0; idx_3<n-2; idx_3++)
    for(int idx_2=0; idx_2<idx_3; idx_2++)
      for(int idx_1=0; idx_1<idx_2; idx_1++)
        for(int idx_0=0; idx_0<idx_1; idx_0++)
          for(int b=0; b<3; b++)
            for(int a=0; a<b; a++)
              F[ idx_4(LUT, idx_0, idx_1, idx_2, idx_3) ] ^= F[ idx_6(LUT, idx_0, idx_1, idx_2, idx_3, n-3+a, n-3+b) ] & (v[a] & v[b]);

  // updates degree-5 term with degree-6 terms
  for(int idx_4=0; idx_4<n-2; idx_4++)
    for(int idx_3=0; idx_3<idx_4; idx_3++)
      for(int idx_2=0; idx_2<idx_3; idx_2++)
        for(int idx_1=0; idx_1<idx_2; idx_1++)
          for(int idx_0=0; idx_0<idx_1; idx_0++)
            for(int a=0; a<3; a++)
              F[ idx_5(LUT, idx_0, idx_1, idx_2, idx_3, idx_4) ] ^= F[ idx_6(LUT, idx_0, idx_1, idx_2, idx_3, idx_4, n-3+a) ] & (v[a]);

  // updates degree-0 term with degree-7 terms
  for(int g=0; g<3; g++)
    for(int f=0; f<g; f++)
      for(int e=0; e<f; e++)
        for(int d=0; d<e; d++)
          for(int c=0; c<d; c++)
            for(int b=0; b<c; b++)
              for(int a=0; a<b; a++)
                F[ 0 ] ^= F[ idx_7(LUT, n-3+a, n-3+b, n-3+c, n-3+d, n-3+e, n-3+f, n-3+g) ] & (v[a] & v[b] & v[c] & v[d] & v[e] & v[f] & v[g]);

  // updates degree-1 term with degree-7 terms
  for(int idx_0=0; idx_0<n-2; idx_0++)
    for(int f=0; f<3; f++)
      for(int e=0; e<f; e++)
        for(int d=0; d<e; d++)
          for(int c=0; c<d; c++)
            for(int b=0; b<c; b++)
              for(int a=0; a<b; a++)
                F[ idx_1(LUT, idx_0) ] ^= F[ idx_7(LUT, idx_0, n-3+a, n-3+b, n-3+c, n-3+d, n-3+e, n-3+f) ] & (v[a] & v[b] & v[c] & v[d] & v[e] & v[f]);

  // updates degree-2 term with degree-7 terms
  for(int idx_1=0; idx_1<n-2; idx_1++)
    for(int idx_0=0; idx_0<idx_1; idx_0++)
      for(int e=0; e<3; e++)
        for(int d=0; d<e; d++)
          for(int c=0; c<d; c++)
            for(int b=0; b<c; b++)
              for(int a=0; a<b; a++)
                F[ idx_2(LUT, idx_0, idx_1) ] ^= F[ idx_7(LUT, idx_0, idx_1, n-3+a, n-3+b, n-3+c, n-3+d, n-3+e) ] & (v[a] & v[b] & v[c] & v[d] & v[e]);

  // updates degree-3 term with degree-7 terms
  for(int idx_2=0; idx_2<n-2; idx_2++)
    for(int idx_1=0; idx_1<idx_2; idx_1++)
      for(int idx_0=0; idx_0<idx_1; idx_0++)
        for(int d=0; d<3; d++)
          for(int c=0; c<d; c++)
            for(int b=0; b<c; b++)
              for(int a=0; a<b; a++)
                F[ idx_3(LUT, idx_0, idx_1, idx_2) ] ^= F[ idx_7(LUT, idx_0, idx_1, idx_2, n-3+a, n-3+b, n-3+c, n-3+d) ] & (v[a] & v[b] & v[c] & v[d]);

  // updates degree-4 term with degree-7 terms
  for(int idx_3=0; idx_3<n-2; idx_3++)
    for(int idx_2=0; idx_2<idx_3; idx_2++)
      for(int idx_1=0; idx_1<idx_2; idx_1++)
        for(int idx_0=0; idx_0<idx_1; idx_0++)
          for(int c=0; c<3; c++)
            for(int b=0; b<c; b++)
              for(int a=0; a<b; a++)
                F[ idx_4(LUT, idx_0, idx_1, idx_2, idx_3) ] ^= F[ idx_7(LUT, idx_0, idx_1, idx_2, idx_3, n-3+a, n-3+b, n-3+c) ] & (v[a] & v[b] & v[c]);

  // updates degree-5 term with degree-7 terms
  for(int idx_4=0; idx_4<n-2; idx_4++)
    for(int idx_3=0; idx_3<idx_4; idx_3++)
      for(int idx_2=0; idx_2<idx_3; idx_2++)
        for(int idx_1=0; idx_1<idx_2; idx_1++)
          for(int idx_0=0; idx_0<idx_1; idx_0++)
            for(int b=0; b<3; b++)
              for(int a=0; a<b; a++)
                F[ idx_5(LUT, idx_0, idx_1, idx_2, idx_3, idx_4) ] ^= F[ idx_7(LUT, idx_0, idx_1, idx_2, idx_3, idx_4, n-3+a, n-3+b) ] & (v[a] & v[b]);

  // updates degree-6 term with degree-7 terms
  for(int idx_5=0; idx_5<n-2; idx_5++)
    for(int idx_4=0; idx_4<idx_5; idx_4++)
      for(int idx_3=0; idx_3<idx_4; idx_3++)
        for(int idx_2=0; idx_2<idx_3; idx_2++)
          for(int idx_1=0; idx_1<idx_2; idx_1++)
            for(int idx_0=0; idx_0<idx_1; idx_0++)
              for(int a=0; a<3; a++)
                F[ idx_6(LUT, idx_0, idx_1, idx_2, idx_3, idx_4, idx_5) ] ^= F[ idx_7(LUT, idx_0, idx_1, idx_2, idx_3, idx_4, idx_5, n-3+a) ] & (v[a]);

  // updates degree-0 term with degree-8 terms
  for(int h=0; h<3; h++)
    for(int g=0; g<h; g++)
      for(int f=0; f<g; f++)
        for(int e=0; e<f; e++)
          for(int d=0; d<e; d++)
            for(int c=0; c<d; c++)
              for(int b=0; b<c; b++)
                for(int a=0; a<b; a++)
                  F[ 0 ] ^= F[ idx_8(LUT, n-3+a, n-3+b, n-3+c, n-3+d, n-3+e, n-3+f, n-3+g, n-3+h) ] & (v[a] & v[b] & v[c] & v[d] & v[e] & v[f] & v[g] & v[h]);

  // updates degree-1 term with degree-8 terms
  for(int idx_0=0; idx_0<n-2; idx_0++)
    for(int g=0; g<3; g++)
      for(int f=0; f<g; f++)
        for(int e=0; e<f; e++)
          for(int d=0; d<e; d++)
            for(int c=0; c<d; c++)
              for(int b=0; b<c; b++)
                for(int a=0; a<b; a++)
                  F[ idx_1(LUT, idx_0) ] ^= F[ idx_8(LUT, idx_0, n-3+a, n-3+b, n-3+c, n-3+d, n-3+e, n-3+f, n-3+g) ] & (v[a] & v[b] & v[c] & v[d] & v[e] & v[f] & v[g]);

  // updates degree-2 term with degree-8 terms
  for(int idx_1=0; idx_1<n-2; idx_1++)
    for(int idx_0=0; idx_0<idx_1; idx_0++)
      for(int f=0; f<3; f++)
        for(int e=0; e<f; e++)
          for(int d=0; d<e; d++)
            for(int c=0; c<d; c++)
              for(int b=0; b<c; b++)
                for(int a=0; a<b; a++)
                  F[ idx_2(LUT, idx_0, idx_1) ] ^= F[ idx_8(LUT, idx_0, idx_1, n-3+a, n-3+b, n-3+c, n-3+d, n-3+e, n-3+f) ] & (v[a] & v[b] & v[c] & v[d] & v[e] & v[f]);

  // updates degree-3 term with degree-8 terms
  for(int idx_2=0; idx_2<n-2; idx_2++)
    for(int idx_1=0; idx_1<idx_2; idx_1++)
      for(int idx_0=0; idx_0<idx_1; idx_0++)
        for(int e=0; e<3; e++)
          for(int d=0; d<e; d++)
            for(int c=0; c<d; c++)
              for(int b=0; b<c; b++)
                for(int a=0; a<b; a++)
                  F[ idx_3(LUT, idx_0, idx_1, idx_2) ] ^= F[ idx_8(LUT, idx_0, idx_1, idx_2, n-3+a, n-3+b, n-3+c, n-3+d, n-3+e) ] & (v[a] & v[b] & v[c] & v[d] & v[e]);

  // updates degree-4 term with degree-8 terms
  for(int idx_3=0; idx_3<n-2; idx_3++)
    for(int idx_2=0; idx_2<idx_3; idx_2++)
      for(int idx_1=0; idx_1<idx_2; idx_1++)
        for(int idx_0=0; idx_0<idx_1; idx_0++)
          for(int d=0; d<3; d++)
            for(int c=0; c<d; c++)
              for(int b=0; b<c; b++)
                for(int a=0; a<b; a++)
                  F[ idx_4(LUT, idx_0, idx_1, idx_2, idx_3) ] ^= F[ idx_8(LUT, idx_0, idx_1, idx_2, idx_3, n-3+a, n-3+b, n-3+c, n-3+d) ] & (v[a] & v[b] & v[c] & v[d]);

  // updates degree-5 term with degree-8 terms
  for(int idx_4=0; idx_4<n-2; idx_4++)
    for(int idx_3=0; idx_3<idx_4; idx_3++)
      for(int idx_2=0; idx_2<idx_3; idx_2++)
        for(int idx_1=0; idx_1<idx_2; idx_1++)
          for(int idx_0=0; idx_0<idx_1; idx_0++)
            for(int c=0; c<3; c++)
              for(int b=0; b<c; b++)
                for(int a=0; a<b; a++)
                  F[ idx_5(LUT, idx_0, idx_1, idx_2, idx_3, idx_4) ] ^= F[ idx_8(LUT, idx_0, idx_1, idx_2, idx_3, idx_4, n-3+a, n-3+b, n-3+c) ] & (v[a] & v[b] & v[c]);

  // updates degree-6 term with degree-8 terms
  for(int idx_5=0; idx_5<n-2; idx_5++)
    for(int idx_4=0; idx_4<idx_5; idx_4++)
      for(int idx_3=0; idx_3<idx_4; idx_3++)
        for(int idx_2=0; idx_2<idx_3; idx_2++)
          for(int idx_1=0; idx_1<idx_2; idx_1++)
            for(int idx_0=0; idx_0<idx_1; idx_0++)
              for(int b=0; b<3; b++)
                for(int a=0; a<b; a++)
                  F[ idx_6(LUT, idx_0, idx_1, idx_2, idx_3, idx_4, idx_5) ] ^= F[ idx_8(LUT, idx_0, idx_1, idx_2, idx_3, idx_4, idx_5, n-3+a, n-3+b) ] & (v[a] & v[b]);

  // updates degree-7 term with degree-8 terms
  for(int idx_6=0; idx_6<n-2; idx_6++)
    for(int idx_5=0; idx_5<idx_6; idx_5++)
      for(int idx_4=0; idx_4<idx_5; idx_4++)
        for(int idx_3=0; idx_3<idx_4; idx_3++)
          for(int idx_2=0; idx_2<idx_3; idx_2++)
            for(int idx_1=0; idx_1<idx_2; idx_1++)
              for(int idx_0=0; idx_0<idx_1; idx_0++)
                for(int a=0; a<3; a++)
                  F[ idx_7(LUT, idx_0, idx_1, idx_2, idx_3, idx_4, idx_5, idx_6) ] ^= F[ idx_8(LUT, idx_0, idx_1, idx_2, idx_3, idx_4, idx_5, idx_6, n-3+a) ] & (v[a]);

  // updates degree-0 term with degree-9 terms
  for(int i=0; i<3; i++)
    for(int h=0; h<i; h++)
      for(int g=0; g<h; g++)
        for(int f=0; f<g; f++)
          for(int e=0; e<f; e++)
            for(int d=0; d<e; d++)
              for(int c=0; c<d; c++)
                for(int b=0; b<c; b++)
                  for(int a=0; a<b; a++)
                    F[ 0 ] ^= F[ idx_9(LUT, n-3+a, n-3+b, n-3+c, n-3+d, n-3+e, n-3+f, n-3+g, n-3+h, n-3+i) ] & (v[a] & v[b] & v[c] & v[d] & v[e] & v[f] & v[g] & v[h] & v[i]);

  // updates degree-1 term with degree-9 terms
  for(int idx_0=0; idx_0<n-2; idx_0++)
    for(int h=0; h<3; h++)
      for(int g=0; g<h; g++)
        for(int f=0; f<g; f++)
          for(int e=0; e<f; e++)
            for(int d=0; d<e; d++)
              for(int c=0; c<d; c++)
                for(int b=0; b<c; b++)
                  for(int a=0; a<b; a++)
                    F[ idx_1(LUT, idx_0) ] ^= F[ idx_9(LUT, idx_0, n-3+a, n-3+b, n-3+c, n-3+d, n-3+e, n-3+f, n-3+g, n-3+h) ] & (v[a] & v[b] & v[c] & v[d] & v[e] & v[f] & v[g] & v[h]);

  // updates degree-2 term with degree-9 terms
  for(int idx_1=0; idx_1<n-2; idx_1++)
    for(int idx_0=0; idx_0<idx_1; idx_0++)
      for(int g=0; g<3; g++)
        for(int f=0; f<g; f++)
          for(int e=0; e<f; e++)
            for(int d=0; d<e; d++)
              for(int c=0; c<d; c++)
                for(int b=0; b<c; b++)
                  for(int a=0; a<b; a++)
                    F[ idx_2(LUT, idx_0, idx_1) ] ^= F[ idx_9(LUT, idx_0, idx_1, n-3+a, n-3+b, n-3+c, n-3+d, n-3+e, n-3+f, n-3+g) ] & (v[a] & v[b] & v[c] & v[d] & v[e] & v[f] & v[g]);

  // updates degree-3 term with degree-9 terms
  for(int idx_2=0; idx_2<n-2; idx_2++)
    for(int idx_1=0; idx_1<idx_2; idx_1++)
      for(int idx_0=0; idx_0<idx_1; idx_0++)
        for(int f=0; f<3; f++)
          for(int e=0; e<f; e++)
            for(int d=0; d<e; d++)
              for(int c=0; c<d; c++)
                for(int b=0; b<c; b++)
                  for(int a=0; a<b; a++)
                    F[ idx_3(LUT, idx_0, idx_1, idx_2) ] ^= F[ idx_9(LUT, idx_0, idx_1, idx_2, n-3+a, n-3+b, n-3+c, n-3+d, n-3+e, n-3+f) ] & (v[a] & v[b] & v[c] & v[d] & v[e] & v[f]);

  // updates degree-4 term with degree-9 terms
  for(int idx_3=0; idx_3<n-2; idx_3++)
    for(int idx_2=0; idx_2<idx_3; idx_2++)
      for(int idx_1=0; idx_1<idx_2; idx_1++)
        for(int idx_0=0; idx_0<idx_1; idx_0++)
          for(int e=0; e<3; e++)
            for(int d=0; d<e; d++)
              for(int c=0; c<d; c++)
                for(int b=0; b<c; b++)
                  for(int a=0; a<b; a++)
                    F[ idx_4(LUT, idx_0, idx_1, idx_2, idx_3) ] ^= F[ idx_9(LUT, idx_0, idx_1, idx_2, idx_3, n-3+a, n-3+b, n-3+c, n-3+d, n-3+e) ] & (v[a] & v[b] & v[c] & v[d] & v[e]);

  // updates degree-5 term with degree-9 terms
  for(int idx_4=0; idx_4<n-2; idx_4++)
    for(int idx_3=0; idx_3<idx_4; idx_3++)
      for(int idx_2=0; idx_2<idx_3; idx_2++)
        for(int idx_1=0; idx_1<idx_2; idx_1++)
          for(int idx_0=0; idx_0<idx_1; idx_0++)
            for(int d=0; d<3; d++)
              for(int c=0; c<d; c++)
                for(int b=0; b<c; b++)
                  for(int a=0; a<b; a++)
                    F[ idx_5(LUT, idx_0, idx_1, idx_2, idx_3, idx_4) ] ^= F[ idx_9(LUT, idx_0, idx_1, idx_2, idx_3, idx_4, n-3+a, n-3+b, n-3+c, n-3+d) ] & (v[a] & v[b] & v[c] & v[d]);

  // updates degree-6 term with degree-9 terms
  for(int idx_5=0; idx_5<n-2; idx_5++)
    for(int idx_4=0; idx_4<idx_5; idx_4++)
      for(int idx_3=0; idx_3<idx_4; idx_3++)
        for(int idx_2=0; idx_2<idx_3; idx_2++)
          for(int idx_1=0; idx_1<idx_2; idx_1++)
            for(int idx_0=0; idx_0<idx_1; idx_0++)
              for(int c=0; c<3; c++)
                for(int b=0; b<c; b++)
                  for(int a=0; a<b; a++)
                    F[ idx_6(LUT, idx_0, idx_1, idx_2, idx_3, idx_4, idx_5) ] ^= F[ idx_9(LUT, idx_0, idx_1, idx_2, idx_3, idx_4, idx_5, n-3+a, n-3+b, n-3+c) ] & (v[a] & v[b] & v[c]);

  // updates degree-7 term with degree-9 terms
  for(int idx_6=0; idx_6<n-2; idx_6++)
    for(int idx_5=0; idx_5<idx_6; idx_5++)
      for(int idx_4=0; idx_4<idx_5; idx_4++)
        for(int idx_3=0; idx_3<idx_4; idx_3++)
          for(int idx_2=0; idx_2<idx_3; idx_2++)
            for(int idx_1=0; idx_1<idx_2; idx_1++)
              for(int idx_0=0; idx_0<idx_1; idx_0++)
                for(int b=0; b<3; b++)
                  for(int a=0; a<b; a++)
                    F[ idx_7(LUT, idx_0, idx_1, idx_2, idx_3, idx_4, idx_5, idx_6) ] ^= F[ idx_9(LUT, idx_0, idx_1, idx_2, idx_3, idx_4, idx_5, idx_6, n-3+a, n-3+b) ] & (v[a] & v[b]);

  // updates degree-8 term with degree-9 terms
  for(int idx_7=0; idx_7<n-2; idx_7++)
    for(int idx_6=0; idx_6<idx_7; idx_6++)
      for(int idx_5=0; idx_5<idx_6; idx_5++)
        for(int idx_4=0; idx_4<idx_5; idx_4++)
          for(int idx_3=0; idx_3<idx_4; idx_3++)
            for(int idx_2=0; idx_2<idx_3; idx_2++)
              for(int idx_1=0; idx_1<idx_2; idx_1++)
                for(int idx_0=0; idx_0<idx_1; idx_0++)
                  for(int a=0; a<3; a++)
                    F[ idx_8(LUT, idx_0, idx_1, idx_2, idx_3, idx_4, idx_5, idx_6, idx_7) ] ^= F[ idx_9(LUT, idx_0, idx_1, idx_2, idx_3, idx_4, idx_5, idx_6, idx_7, n-3+a) ] & (v[a]);

  if (verbose) printf("fes: specialization = %" PRIu64 " cycles\n", rdtsc()-everything_start_time);
  #define QUIT() { \
    if (F != NULL) free(F); \
    return; \
  }

  uint64_t init_start_time = rdtsc();
  // computes the derivatives required by the enumeration kernel up to degree 9
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

  // here, degree-4 terms are affected by degree-8 terms
  for(int i3=4; i3<n; i3++) {
      for(int i2=2; i2<i3; i2++) {
        for(int i1=1; i1<i2; i1++) {
          for(int i0=0; i0<i1; i0++) {
            if (i0  != 0  && i1-1 > i0 && i2-1 > i1 && i3-1 > i2) F[ idx_4(LUT, i0, i1, i2, i3) ] ^= F[ idx_8(LUT, i0-1, i0, i1-1, i1, i2-1, i2, i3-1, i3) ];
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

  // here, degree-5 terms are affected by degree-8 terms
  for(int i4=5; i4<n; i4++) {
      for(int i3=3; i3<i4; i3++) {
        for(int i2=2; i2<i3; i2++) {
          for(int i1=1; i1<i2; i1++) {
            for(int i0=0; i0<i1; i0++) {
              if (i0  != 0  && i1-1 > i0 && i2-1 > i1) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_8(LUT, i0-1, i0, i1-1, i1, i2-1, i2, i3, i4) ];
              if (i0  != 0  && i1-1 > i0 && i3-1 > i2) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_8(LUT, i0-1, i0, i1-1, i1, i2, i3-1, i3, i4) ];
              if (i0  != 0  && i1-1 > i0 && i4-1 > i3) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_8(LUT, i0-1, i0, i1-1, i1, i2, i3, i4-1, i4) ];
              if (i0  != 0  && i2-1 > i1 && i3-1 > i2) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_8(LUT, i0-1, i0, i1, i2-1, i2, i3-1, i3, i4) ];
              if (i0  != 0  && i2-1 > i1 && i4-1 > i3) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_8(LUT, i0-1, i0, i1, i2-1, i2, i3, i4-1, i4) ];
              if (i0  != 0  && i3-1 > i2 && i4-1 > i3) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_8(LUT, i0-1, i0, i1, i2, i3-1, i3, i4-1, i4) ];
              if (i1-1 > i0 && i2-1 > i1 && i3-1 > i2) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_8(LUT, i0, i1-1, i1, i2-1, i2, i3-1, i3, i4) ];
              if (i1-1 > i0 && i2-1 > i1 && i4-1 > i3) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_8(LUT, i0, i1-1, i1, i2-1, i2, i3, i4-1, i4) ];
              if (i1-1 > i0 && i3-1 > i2 && i4-1 > i3) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_8(LUT, i0, i1-1, i1, i2, i3-1, i3, i4-1, i4) ];
              if (i2-1 > i1 && i3-1 > i2 && i4-1 > i3) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_8(LUT, i0, i1, i2-1, i2, i3-1, i3, i4-1, i4) ];
  }}}}}

  // here, degree-5 terms are affected by degree-9 terms
  for(int i4=5; i4<n; i4++) {
      for(int i3=3; i3<i4; i3++) {
        for(int i2=2; i2<i3; i2++) {
          for(int i1=1; i1<i2; i1++) {
            for(int i0=0; i0<i1; i0++) {
              if (i0  != 0  && i1-1 > i0 && i2-1 > i1 && i3-1 > i2) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_9(LUT, i0-1, i0, i1-1, i1, i2-1, i2, i3-1, i3, i4) ];
              if (i0  != 0  && i1-1 > i0 && i2-1 > i1 && i4-1 > i3) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_9(LUT, i0-1, i0, i1-1, i1, i2-1, i2, i3, i4-1, i4) ];
              if (i0  != 0  && i1-1 > i0 && i3-1 > i2 && i4-1 > i3) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_9(LUT, i0-1, i0, i1-1, i1, i2, i3-1, i3, i4-1, i4) ];
              if (i0  != 0  && i2-1 > i1 && i3-1 > i2 && i4-1 > i3) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_9(LUT, i0-1, i0, i1, i2-1, i2, i3-1, i3, i4-1, i4) ];
              if (i1-1 > i0 && i2-1 > i1 && i3-1 > i2 && i4-1 > i3) F[ idx_5(LUT, i0, i1, i2, i3, i4) ] ^= F[ idx_9(LUT, i0, i1-1, i1, i2-1, i2, i3-1, i3, i4-1, i4) ];
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

  // here, degree-6 terms are affected by degree-8 terms
  for(int i5=6; i5<n; i5++) {
      for(int i4=4; i4<i5; i4++) {
        for(int i3=3; i3<i4; i3++) {
          for(int i2=2; i2<i3; i2++) {
            for(int i1=1; i1<i2; i1++) {
              for(int i0=0; i0<i1; i0++) {
                if (i0  != 0  && i1-1 > i0) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_8(LUT, i0-1, i0, i1-1, i1, i2, i3, i4, i5) ];
                if (i0  != 0  && i2-1 > i1) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_8(LUT, i0-1, i0, i1, i2-1, i2, i3, i4, i5) ];
                if (i0  != 0  && i3-1 > i2) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_8(LUT, i0-1, i0, i1, i2, i3-1, i3, i4, i5) ];
                if (i0  != 0  && i4-1 > i3) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_8(LUT, i0-1, i0, i1, i2, i3, i4-1, i4, i5) ];
                if (i0  != 0  && i5-1 > i4) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_8(LUT, i0-1, i0, i1, i2, i3, i4, i5-1, i5) ];
                if (i1-1 > i0 && i2-1 > i1) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_8(LUT, i0, i1-1, i1, i2-1, i2, i3, i4, i5) ];
                if (i1-1 > i0 && i3-1 > i2) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_8(LUT, i0, i1-1, i1, i2, i3-1, i3, i4, i5) ];
                if (i1-1 > i0 && i4-1 > i3) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_8(LUT, i0, i1-1, i1, i2, i3, i4-1, i4, i5) ];
                if (i1-1 > i0 && i5-1 > i4) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_8(LUT, i0, i1-1, i1, i2, i3, i4, i5-1, i5) ];
                if (i2-1 > i1 && i3-1 > i2) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_8(LUT, i0, i1, i2-1, i2, i3-1, i3, i4, i5) ];
                if (i2-1 > i1 && i4-1 > i3) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_8(LUT, i0, i1, i2-1, i2, i3, i4-1, i4, i5) ];
                if (i2-1 > i1 && i5-1 > i4) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_8(LUT, i0, i1, i2-1, i2, i3, i4, i5-1, i5) ];
                if (i3-1 > i2 && i4-1 > i3) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_8(LUT, i0, i1, i2, i3-1, i3, i4-1, i4, i5) ];
                if (i3-1 > i2 && i5-1 > i4) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_8(LUT, i0, i1, i2, i3-1, i3, i4, i5-1, i5) ];
                if (i4-1 > i3 && i5-1 > i4) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_8(LUT, i0, i1, i2, i3, i4-1, i4, i5-1, i5) ];
  }}}}}}

  // here, degree-6 terms are affected by degree-9 terms
  for(int i5=6; i5<n; i5++) {
      for(int i4=4; i4<i5; i4++) {
        for(int i3=3; i3<i4; i3++) {
          for(int i2=2; i2<i3; i2++) {
            for(int i1=1; i1<i2; i1++) {
              for(int i0=0; i0<i1; i0++) {
                if (i0  != 0  && i1-1 > i0 && i2-1 > i1) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_9(LUT, i0-1, i0, i1-1, i1, i2-1, i2, i3, i4, i5) ];
                if (i0  != 0  && i1-1 > i0 && i3-1 > i2) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_9(LUT, i0-1, i0, i1-1, i1, i2, i3-1, i3, i4, i5) ];
                if (i0  != 0  && i1-1 > i0 && i4-1 > i3) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_9(LUT, i0-1, i0, i1-1, i1, i2, i3, i4-1, i4, i5) ];
                if (i0  != 0  && i1-1 > i0 && i5-1 > i4) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_9(LUT, i0-1, i0, i1-1, i1, i2, i3, i4, i5-1, i5) ];
                if (i0  != 0  && i2-1 > i1 && i3-1 > i2) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_9(LUT, i0-1, i0, i1, i2-1, i2, i3-1, i3, i4, i5) ];
                if (i0  != 0  && i2-1 > i1 && i4-1 > i3) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_9(LUT, i0-1, i0, i1, i2-1, i2, i3, i4-1, i4, i5) ];
                if (i0  != 0  && i2-1 > i1 && i5-1 > i4) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_9(LUT, i0-1, i0, i1, i2-1, i2, i3, i4, i5-1, i5) ];
                if (i0  != 0  && i3-1 > i2 && i4-1 > i3) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_9(LUT, i0-1, i0, i1, i2, i3-1, i3, i4-1, i4, i5) ];
                if (i0  != 0  && i3-1 > i2 && i5-1 > i4) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_9(LUT, i0-1, i0, i1, i2, i3-1, i3, i4, i5-1, i5) ];
                if (i0  != 0  && i4-1 > i3 && i5-1 > i4) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_9(LUT, i0-1, i0, i1, i2, i3, i4-1, i4, i5-1, i5) ];
                if (i1-1 > i0 && i2-1 > i1 && i3-1 > i2) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_9(LUT, i0, i1-1, i1, i2-1, i2, i3-1, i3, i4, i5) ];
                if (i1-1 > i0 && i2-1 > i1 && i4-1 > i3) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_9(LUT, i0, i1-1, i1, i2-1, i2, i3, i4-1, i4, i5) ];
                if (i1-1 > i0 && i2-1 > i1 && i5-1 > i4) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_9(LUT, i0, i1-1, i1, i2-1, i2, i3, i4, i5-1, i5) ];
                if (i1-1 > i0 && i3-1 > i2 && i4-1 > i3) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_9(LUT, i0, i1-1, i1, i2, i3-1, i3, i4-1, i4, i5) ];
                if (i1-1 > i0 && i3-1 > i2 && i5-1 > i4) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_9(LUT, i0, i1-1, i1, i2, i3-1, i3, i4, i5-1, i5) ];
                if (i1-1 > i0 && i4-1 > i3 && i5-1 > i4) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_9(LUT, i0, i1-1, i1, i2, i3, i4-1, i4, i5-1, i5) ];
                if (i2-1 > i1 && i3-1 > i2 && i4-1 > i3) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_9(LUT, i0, i1, i2-1, i2, i3-1, i3, i4-1, i4, i5) ];
                if (i2-1 > i1 && i3-1 > i2 && i5-1 > i4) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_9(LUT, i0, i1, i2-1, i2, i3-1, i3, i4, i5-1, i5) ];
                if (i2-1 > i1 && i4-1 > i3 && i5-1 > i4) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_9(LUT, i0, i1, i2-1, i2, i3, i4-1, i4, i5-1, i5) ];
                if (i3-1 > i2 && i4-1 > i3 && i5-1 > i4) F[ idx_6(LUT, i0, i1, i2, i3, i4, i5) ] ^= F[ idx_9(LUT, i0, i1, i2, i3-1, i3, i4-1, i4, i5-1, i5) ];
  }}}}}}

  // here, degree-7 terms are affected by degree-8 terms
  for(int i6=7; i6<n; i6++) {
      for(int i5=5; i5<i6; i5++) {
        for(int i4=4; i4<i5; i4++) {
          for(int i3=3; i3<i4; i3++) {
            for(int i2=2; i2<i3; i2++) {
              for(int i1=1; i1<i2; i1++) {
                for(int i0=0; i0<i1; i0++) {
                  if (i0  != 0 ) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_8(LUT, i0-1, i0, i1, i2, i3, i4, i5, i6) ];
                  if (i1-1 > i0) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_8(LUT, i0, i1-1, i1, i2, i3, i4, i5, i6) ];
                  if (i2-1 > i1) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_8(LUT, i0, i1, i2-1, i2, i3, i4, i5, i6) ];
                  if (i3-1 > i2) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_8(LUT, i0, i1, i2, i3-1, i3, i4, i5, i6) ];
                  if (i4-1 > i3) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_8(LUT, i0, i1, i2, i3, i4-1, i4, i5, i6) ];
                  if (i5-1 > i4) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_8(LUT, i0, i1, i2, i3, i4, i5-1, i5, i6) ];
                  if (i6-1 > i5) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_8(LUT, i0, i1, i2, i3, i4, i5, i6-1, i6) ];
  }}}}}}}

  // here, degree-7 terms are affected by degree-9 terms
  for(int i6=7; i6<n; i6++) {
      for(int i5=5; i5<i6; i5++) {
        for(int i4=4; i4<i5; i4++) {
          for(int i3=3; i3<i4; i3++) {
            for(int i2=2; i2<i3; i2++) {
              for(int i1=1; i1<i2; i1++) {
                for(int i0=0; i0<i1; i0++) {
                  if (i0  != 0  && i1-1 > i0) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_9(LUT, i0-1, i0, i1-1, i1, i2, i3, i4, i5, i6) ];
                  if (i0  != 0  && i2-1 > i1) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_9(LUT, i0-1, i0, i1, i2-1, i2, i3, i4, i5, i6) ];
                  if (i0  != 0  && i3-1 > i2) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_9(LUT, i0-1, i0, i1, i2, i3-1, i3, i4, i5, i6) ];
                  if (i0  != 0  && i4-1 > i3) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_9(LUT, i0-1, i0, i1, i2, i3, i4-1, i4, i5, i6) ];
                  if (i0  != 0  && i5-1 > i4) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_9(LUT, i0-1, i0, i1, i2, i3, i4, i5-1, i5, i6) ];
                  if (i0  != 0  && i6-1 > i5) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_9(LUT, i0-1, i0, i1, i2, i3, i4, i5, i6-1, i6) ];
                  if (i1-1 > i0 && i2-1 > i1) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_9(LUT, i0, i1-1, i1, i2-1, i2, i3, i4, i5, i6) ];
                  if (i1-1 > i0 && i3-1 > i2) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_9(LUT, i0, i1-1, i1, i2, i3-1, i3, i4, i5, i6) ];
                  if (i1-1 > i0 && i4-1 > i3) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_9(LUT, i0, i1-1, i1, i2, i3, i4-1, i4, i5, i6) ];
                  if (i1-1 > i0 && i5-1 > i4) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_9(LUT, i0, i1-1, i1, i2, i3, i4, i5-1, i5, i6) ];
                  if (i1-1 > i0 && i6-1 > i5) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_9(LUT, i0, i1-1, i1, i2, i3, i4, i5, i6-1, i6) ];
                  if (i2-1 > i1 && i3-1 > i2) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_9(LUT, i0, i1, i2-1, i2, i3-1, i3, i4, i5, i6) ];
                  if (i2-1 > i1 && i4-1 > i3) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_9(LUT, i0, i1, i2-1, i2, i3, i4-1, i4, i5, i6) ];
                  if (i2-1 > i1 && i5-1 > i4) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_9(LUT, i0, i1, i2-1, i2, i3, i4, i5-1, i5, i6) ];
                  if (i2-1 > i1 && i6-1 > i5) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_9(LUT, i0, i1, i2-1, i2, i3, i4, i5, i6-1, i6) ];
                  if (i3-1 > i2 && i4-1 > i3) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_9(LUT, i0, i1, i2, i3-1, i3, i4-1, i4, i5, i6) ];
                  if (i3-1 > i2 && i5-1 > i4) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_9(LUT, i0, i1, i2, i3-1, i3, i4, i5-1, i5, i6) ];
                  if (i3-1 > i2 && i6-1 > i5) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_9(LUT, i0, i1, i2, i3-1, i3, i4, i5, i6-1, i6) ];
                  if (i4-1 > i3 && i5-1 > i4) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_9(LUT, i0, i1, i2, i3, i4-1, i4, i5-1, i5, i6) ];
                  if (i4-1 > i3 && i6-1 > i5) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_9(LUT, i0, i1, i2, i3, i4-1, i4, i5, i6-1, i6) ];
                  if (i5-1 > i4 && i6-1 > i5) F[ idx_7(LUT, i0, i1, i2, i3, i4, i5, i6) ] ^= F[ idx_9(LUT, i0, i1, i2, i3, i4, i5-1, i5, i6-1, i6) ];
  }}}}}}}

  // here, degree-8 terms are affected by degree-9 terms
  for(int i7=8; i7<n; i7++) {
      for(int i6=6; i6<i7; i6++) {
        for(int i5=5; i5<i6; i5++) {
          for(int i4=4; i4<i5; i4++) {
            for(int i3=3; i3<i4; i3++) {
              for(int i2=2; i2<i3; i2++) {
                for(int i1=1; i1<i2; i1++) {
                  for(int i0=0; i0<i1; i0++) {
                    if (i0  != 0 ) F[ idx_8(LUT, i0, i1, i2, i3, i4, i5, i6, i7) ] ^= F[ idx_9(LUT, i0-1, i0, i1, i2, i3, i4, i5, i6, i7) ];
                    if (i1-1 > i0) F[ idx_8(LUT, i0, i1, i2, i3, i4, i5, i6, i7) ] ^= F[ idx_9(LUT, i0, i1-1, i1, i2, i3, i4, i5, i6, i7) ];
                    if (i2-1 > i1) F[ idx_8(LUT, i0, i1, i2, i3, i4, i5, i6, i7) ] ^= F[ idx_9(LUT, i0, i1, i2-1, i2, i3, i4, i5, i6, i7) ];
                    if (i3-1 > i2) F[ idx_8(LUT, i0, i1, i2, i3, i4, i5, i6, i7) ] ^= F[ idx_9(LUT, i0, i1, i2, i3-1, i3, i4, i5, i6, i7) ];
                    if (i4-1 > i3) F[ idx_8(LUT, i0, i1, i2, i3, i4, i5, i6, i7) ] ^= F[ idx_9(LUT, i0, i1, i2, i3, i4-1, i4, i5, i6, i7) ];
                    if (i5-1 > i4) F[ idx_8(LUT, i0, i1, i2, i3, i4, i5, i6, i7) ] ^= F[ idx_9(LUT, i0, i1, i2, i3, i4, i5-1, i5, i6, i7) ];
                    if (i6-1 > i5) F[ idx_8(LUT, i0, i1, i2, i3, i4, i5, i6, i7) ] ^= F[ idx_9(LUT, i0, i1, i2, i3, i4, i5, i6-1, i6, i7) ];
                    if (i7-1 > i6) F[ idx_8(LUT, i0, i1, i2, i3, i4, i5, i6, i7) ] ^= F[ idx_9(LUT, i0, i1, i2, i3, i4, i5, i6, i7-1, i7) ];
  }}}}}}}}



  if (verbose) printf("fes: initialisation = %" PRIu64 " cycles\n", rdtsc()-init_start_time);
  uint64_t enumeration_start_time = rdtsc();
  uint64_t n_solutions_found = 0;
  uint64_t current_solution_index = 0;
  uint64_t pack_of_solution[65536];
  solution_t solution_buffer[576];

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

              for(int idx_6=0; idx_6<idx_5; idx_6++) {

                // special case when i has hamming weight exactly 7
                const uint64_t weight_7_start = weight_6_start + (1ll << idx_6);
                  STEP_7( idx_1(LUT, idx_6), idx_2(LUT, idx_6, idx_5), idx_3(LUT, idx_6, idx_5, idx_4), idx_4(LUT, idx_6, idx_5, idx_4, idx_3), idx_5(LUT, idx_6, idx_5, idx_4, idx_3, idx_2), idx_6(LUT, idx_6, idx_5, idx_4, idx_3, idx_2, idx_1), idx_7(LUT, idx_6, idx_5, idx_4, idx_3, idx_2, idx_1, idx_0), weight_7_start );

                for(int idx_7=0; idx_7<idx_6; idx_7++) {

                  // special case when i has hamming weight exactly 8
                  const uint64_t weight_8_start = weight_7_start + (1ll << idx_7);
                    STEP_8( idx_1(LUT, idx_7), idx_2(LUT, idx_7, idx_6), idx_3(LUT, idx_7, idx_6, idx_5), idx_4(LUT, idx_7, idx_6, idx_5, idx_4), idx_5(LUT, idx_7, idx_6, idx_5, idx_4, idx_3), idx_6(LUT, idx_7, idx_6, idx_5, idx_4, idx_3, idx_2), idx_7(LUT, idx_7, idx_6, idx_5, idx_4, idx_3, idx_2, idx_1), idx_8(LUT, idx_7, idx_6, idx_5, idx_4, idx_3, idx_2, idx_1, idx_0), weight_8_start );

                    // we are now inside the critical part where the hamming weight is known to be >= 9
                    // Thus, there are no special cases from now on

                    // Because of the last step, the current iteration counter is a multiple of 64 plus one
                    // This loop sets it to `rolled_end`, which is a multiple of 64, if possible

                  const uint64_t rolled_end = weight_8_start + (1ll << min(6, idx_7));
                  for(uint64_t i=1 + weight_8_start; i< rolled_end; i++) {

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
                        _i >>= 1; pos++;
                        while ((_i & 0x0001) == 0) { _i >>= 1; pos++; }
                        const int k_8 = pos;
                        _i >>= 1; pos++;
                        while ((_i & 0x0001) == 0) { _i >>= 1; pos++; }
                        const int k_9 = pos;
                      STEP_9( idx_1(LUT, k_1), idx_2(LUT, k_1, k_2), idx_3(LUT, k_1, k_2, k_3), idx_4(LUT, k_1, k_2, k_3, k_4), idx_5(LUT, k_1, k_2, k_3, k_4, k_5), idx_6(LUT, k_1, k_2, k_3, k_4, k_5, k_6), idx_7(LUT, k_1, k_2, k_3, k_4, k_5, k_6, k_7), idx_8(LUT, k_1, k_2, k_3, k_4, k_5, k_6, k_7, k_8), idx_9(LUT, k_1, k_2, k_3, k_4, k_5, k_6, k_7, k_8, k_9), i );
                  }

                  CHECK_SOLUTIONS();

                  // Here, the number of iterations to perform is (supposedly) sufficiently large
                  // We will therefore unroll the loop 64 times

                  // unrolled critical section where the hamming weight is >= 9
                  for(uint64_t j=64; j<(1ull << idx_7); j+=64) {
                        const uint64_t i = j + weight_8_start;
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
                        _i >>= 1; pos++;
                        while ((_i & 0x0001) == 0) { _i >>= 1; pos++; }
                        const int k_8 = pos;
                        _i >>= 1; pos++;
                        while ((_i & 0x0001) == 0) { _i >>= 1; pos++; }
                        const int k_9 = pos;
                        const int alpha = LUT[0][k_1];
                        const int beta = LUT[1][k_1]+LUT[0][k_2];
                        const int gamma = LUT[2][k_1]+LUT[1][k_2]+LUT[0][k_3];
                        const int delta = LUT[3][k_1]+LUT[2][k_2]+LUT[1][k_3]+LUT[0][k_4];
                        const int epsilon = LUT[4][k_1]+LUT[3][k_2]+LUT[2][k_3]+LUT[1][k_4]+LUT[0][k_5];
                        const int zeta = LUT[5][k_1]+LUT[4][k_2]+LUT[3][k_3]+LUT[2][k_4]+LUT[1][k_5]+LUT[0][k_6];
                        const int eta = LUT[6][k_1]+LUT[5][k_2]+LUT[4][k_3]+LUT[3][k_4]+LUT[2][k_5]+LUT[1][k_6]+LUT[0][k_7];
                        const int iota = LUT[7][k_1]+LUT[6][k_2]+LUT[5][k_3]+LUT[4][k_4]+LUT[3][k_5]+LUT[2][k_6]+LUT[1][k_7]+LUT[0][k_8];
                        const int kappa = LUT[8][k_1]+LUT[7][k_2]+LUT[6][k_3]+LUT[5][k_4]+LUT[4][k_5]+LUT[3][k_6]+LUT[2][k_7]+LUT[1][k_8]+LUT[0][k_9];
                        STEP_9(0 + alpha, 0 + beta, 0 + gamma, 0 + delta, 0 + epsilon, 0 + zeta, 0 + eta, 0 + iota, 0 + kappa, i + 0);
                        uint64_t F_sp[ 8 ] = { alpha*16, beta*16, gamma*16, delta*16, epsilon*16, zeta*16, eta*16, iota*16 };
                        func_deg_9_T_3_el_0(F, &F_sp[0], solution_buffer, &n_solutions_found, i);

                        CHECK_SOLUTIONS();
                    }

                }
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
