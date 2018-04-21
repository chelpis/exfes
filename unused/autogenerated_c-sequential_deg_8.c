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

#define STEP_8(a,b,c,d,e,f,g,h,i) { \
  F[ g ] ^= F [ h ]; \
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

// generated with L = 6
void exhaustive_ia32_deg_8(LUT_t LUT, int n, pck_vector_t F[], solution_callback_t callback, void* callback_state, int verbose) {
  #define QUIT() { \
    return; \
  }

  uint64_t init_start_time = rdtsc();
  // computes the derivatives required by the enumeration kernel up to degree 8
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



  if (verbose) printf("fes: initialisation = %" PRIu64 " cycles\n", rdtsc()-init_start_time);
  uint64_t enumeration_start_time = rdtsc();
  uint64_t n_solutions_found = 0;
  uint64_t current_solution_index = 0;
  uint64_t pack_of_solution[65536];
  solution_t solution_buffer[320];

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

              for(int idx_6=0; idx_6<idx_5; idx_6++) {

                // special case when i has hamming weight exactly 7
                const uint64_t weight_7_start = weight_6_start + (1ll << idx_6);
                  STEP_7( idx_1(LUT, idx_6), idx_2(LUT, idx_6, idx_5), idx_3(LUT, idx_6, idx_5, idx_4), idx_4(LUT, idx_6, idx_5, idx_4, idx_3), idx_5(LUT, idx_6, idx_5, idx_4, idx_3, idx_2), idx_6(LUT, idx_6, idx_5, idx_4, idx_3, idx_2, idx_1), idx_7(LUT, idx_6, idx_5, idx_4, idx_3, idx_2, idx_1, idx_0), weight_7_start );

                  // we are now inside the critical part where the hamming weight is known to be >= 8
                  // Thus, there are no special cases from now on

                  // Because of the last step, the current iteration counter is a multiple of 64 plus one
                  // This loop sets it to `rolled_end`, which is a multiple of 64, if possible

                const uint64_t rolled_end = weight_7_start + (1ll << min(6, idx_6));
                for(uint64_t i=1 + weight_7_start; i< rolled_end; i++) {

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
                    STEP_8( idx_1(LUT, k_1), idx_2(LUT, k_1, k_2), idx_3(LUT, k_1, k_2, k_3), idx_4(LUT, k_1, k_2, k_3, k_4), idx_5(LUT, k_1, k_2, k_3, k_4, k_5), idx_6(LUT, k_1, k_2, k_3, k_4, k_5, k_6), idx_7(LUT, k_1, k_2, k_3, k_4, k_5, k_6, k_7), idx_8(LUT, k_1, k_2, k_3, k_4, k_5, k_6, k_7, k_8), i );
                }

                CHECK_SOLUTIONS();

                // Here, the number of iterations to perform is (supposedly) sufficiently large
                // We will therefore unroll the loop 64 times

                // unrolled critical section where the hamming weight is >= 8
                for(uint64_t j=64; j<(1ull << idx_6); j+=64) {
                      const uint64_t i = j + weight_7_start;
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
                      const int alpha = LUT[0][k_1];
                      const int beta = LUT[1][k_1]+LUT[0][k_2];
                      const int gamma = LUT[2][k_1]+LUT[1][k_2]+LUT[0][k_3];
                      const int delta = LUT[3][k_1]+LUT[2][k_2]+LUT[1][k_3]+LUT[0][k_4];
                      const int epsilon = LUT[4][k_1]+LUT[3][k_2]+LUT[2][k_3]+LUT[1][k_4]+LUT[0][k_5];
                      const int zeta = LUT[5][k_1]+LUT[4][k_2]+LUT[3][k_3]+LUT[2][k_4]+LUT[1][k_5]+LUT[0][k_6];
                      const int eta = LUT[6][k_1]+LUT[5][k_2]+LUT[4][k_3]+LUT[3][k_4]+LUT[2][k_5]+LUT[1][k_6]+LUT[0][k_7];
                      const int iota = LUT[7][k_1]+LUT[6][k_2]+LUT[5][k_3]+LUT[4][k_4]+LUT[3][k_5]+LUT[2][k_6]+LUT[1][k_7]+LUT[0][k_8];
                      STEP_8(0 + alpha, 0 + beta, 0 + gamma, 0 + delta, 0 + epsilon, 0 + zeta, 0 + eta, 0 + iota, i + 0);
                      STEP_8(1, 1 + alpha, 1 + beta, 1 + gamma, 1 + delta, 1 + epsilon, 1 + zeta, 1 + eta, i + 1);
                      STEP_8(2, 2 + alpha, 2 + beta, 2 + gamma, 2 + delta, 2 + epsilon, 2 + zeta, 2 + eta, i + 2);
                      STEP_8(1, 3, 3 + alpha, 3 + beta, 3 + gamma, 3 + delta, 3 + epsilon, 3 + zeta, i + 3);
                      STEP_8(4, 4 + alpha, 4 + beta, 4 + gamma, 4 + delta, 4 + epsilon, 4 + zeta, 3 + eta, i + 4);
                      STEP_8(1, 5, 5 + alpha, 5 + beta, 5 + gamma, 5 + delta, 5 + epsilon, 5 + zeta, i + 5);
                      STEP_8(2, 6, 6 + alpha, 6 + beta, 6 + gamma, 6 + delta, 6 + epsilon, 6 + zeta, i + 6);
                      STEP_8(1, 3, 7, 7 + alpha, 7 + beta, 7 + gamma, 7 + delta, 7 + epsilon, i + 7);
                      STEP_8(8, 8 + alpha, 8 + beta, 8 + gamma, 8 + delta, 8 + epsilon, 7 + zeta, 4 + eta, i + 8);
                      STEP_8(1, 9, 9 + alpha, 9 + beta, 9 + gamma, 9 + delta, 9 + epsilon, 8 + zeta, i + 9);
                      STEP_8(2, 10, 10 + alpha, 10 + beta, 10 + gamma, 10 + delta, 10 + epsilon, 9 + zeta, i + 10);
                      STEP_8(1, 3, 11, 11 + alpha, 11 + beta, 11 + gamma, 11 + delta, 11 + epsilon, i + 11);
                      STEP_8(4, 12, 12 + alpha, 12 + beta, 12 + gamma, 12 + delta, 12 + epsilon, 10 + zeta, i + 12);
                      STEP_8(1, 5, 13, 13 + alpha, 13 + beta, 13 + gamma, 13 + delta, 13 + epsilon, i + 13);
                      STEP_8(2, 6, 14, 14 + alpha, 14 + beta, 14 + gamma, 14 + delta, 14 + epsilon, i + 14);
                      STEP_8(1, 3, 7, 15, 15 + alpha, 15 + beta, 15 + gamma, 15 + delta, i + 15);
                      STEP_8(16, 16 + alpha, 16 + beta, 16 + gamma, 16 + delta, 15 + epsilon, 11 + zeta, 5 + eta, i + 16);
                      STEP_8(1, 17, 17 + alpha, 17 + beta, 17 + gamma, 17 + delta, 16 + epsilon, 12 + zeta, i + 17);
                      STEP_8(2, 18, 18 + alpha, 18 + beta, 18 + gamma, 18 + delta, 17 + epsilon, 13 + zeta, i + 18);
                      STEP_8(1, 3, 19, 19 + alpha, 19 + beta, 19 + gamma, 19 + delta, 18 + epsilon, i + 19);
                      STEP_8(4, 20, 20 + alpha, 20 + beta, 20 + gamma, 20 + delta, 19 + epsilon, 14 + zeta, i + 20);
                      STEP_8(1, 5, 21, 21 + alpha, 21 + beta, 21 + gamma, 21 + delta, 20 + epsilon, i + 21);
                      STEP_8(2, 6, 22, 22 + alpha, 22 + beta, 22 + gamma, 22 + delta, 21 + epsilon, i + 22);
                      STEP_8(1, 3, 7, 23, 23 + alpha, 23 + beta, 23 + gamma, 23 + delta, i + 23);
                      STEP_8(8, 24, 24 + alpha, 24 + beta, 24 + gamma, 24 + delta, 22 + epsilon, 15 + zeta, i + 24);
                      STEP_8(1, 9, 25, 25 + alpha, 25 + beta, 25 + gamma, 25 + delta, 23 + epsilon, i + 25);
                      STEP_8(2, 10, 26, 26 + alpha, 26 + beta, 26 + gamma, 26 + delta, 24 + epsilon, i + 26);
                      STEP_8(1, 3, 11, 27, 27 + alpha, 27 + beta, 27 + gamma, 27 + delta, i + 27);
                      STEP_8(4, 12, 28, 28 + alpha, 28 + beta, 28 + gamma, 28 + delta, 25 + epsilon, i + 28);
                      STEP_8(1, 5, 13, 29, 29 + alpha, 29 + beta, 29 + gamma, 29 + delta, i + 29);
                      STEP_8(2, 6, 14, 30, 30 + alpha, 30 + beta, 30 + gamma, 30 + delta, i + 30);
                      STEP_8(1, 3, 7, 15, 31, 31 + alpha, 31 + beta, 31 + gamma, i + 31);
                      STEP_8(32, 32 + alpha, 32 + beta, 32 + gamma, 31 + delta, 26 + epsilon, 16 + zeta, 6 + eta, i + 32);
                      STEP_8(1, 33, 33 + alpha, 33 + beta, 33 + gamma, 32 + delta, 27 + epsilon, 17 + zeta, i + 33);
                      STEP_8(2, 34, 34 + alpha, 34 + beta, 34 + gamma, 33 + delta, 28 + epsilon, 18 + zeta, i + 34);
                      STEP_8(1, 3, 35, 35 + alpha, 35 + beta, 35 + gamma, 34 + delta, 29 + epsilon, i + 35);
                      STEP_8(4, 36, 36 + alpha, 36 + beta, 36 + gamma, 35 + delta, 30 + epsilon, 19 + zeta, i + 36);
                      STEP_8(1, 5, 37, 37 + alpha, 37 + beta, 37 + gamma, 36 + delta, 31 + epsilon, i + 37);
                      STEP_8(2, 6, 38, 38 + alpha, 38 + beta, 38 + gamma, 37 + delta, 32 + epsilon, i + 38);
                      STEP_8(1, 3, 7, 39, 39 + alpha, 39 + beta, 39 + gamma, 38 + delta, i + 39);
                      STEP_8(8, 40, 40 + alpha, 40 + beta, 40 + gamma, 39 + delta, 33 + epsilon, 20 + zeta, i + 40);
                      STEP_8(1, 9, 41, 41 + alpha, 41 + beta, 41 + gamma, 40 + delta, 34 + epsilon, i + 41);
                      STEP_8(2, 10, 42, 42 + alpha, 42 + beta, 42 + gamma, 41 + delta, 35 + epsilon, i + 42);
                      STEP_8(1, 3, 11, 43, 43 + alpha, 43 + beta, 43 + gamma, 42 + delta, i + 43);
                      STEP_8(4, 12, 44, 44 + alpha, 44 + beta, 44 + gamma, 43 + delta, 36 + epsilon, i + 44);
                      STEP_8(1, 5, 13, 45, 45 + alpha, 45 + beta, 45 + gamma, 44 + delta, i + 45);
                      STEP_8(2, 6, 14, 46, 46 + alpha, 46 + beta, 46 + gamma, 45 + delta, i + 46);
                      STEP_8(1, 3, 7, 15, 47, 47 + alpha, 47 + beta, 47 + gamma, i + 47);
                      STEP_8(16, 48, 48 + alpha, 48 + beta, 48 + gamma, 46 + delta, 37 + epsilon, 21 + zeta, i + 48);
                      STEP_8(1, 17, 49, 49 + alpha, 49 + beta, 49 + gamma, 47 + delta, 38 + epsilon, i + 49);
                      STEP_8(2, 18, 50, 50 + alpha, 50 + beta, 50 + gamma, 48 + delta, 39 + epsilon, i + 50);
                      STEP_8(1, 3, 19, 51, 51 + alpha, 51 + beta, 51 + gamma, 49 + delta, i + 51);
                      STEP_8(4, 20, 52, 52 + alpha, 52 + beta, 52 + gamma, 50 + delta, 40 + epsilon, i + 52);
                      STEP_8(1, 5, 21, 53, 53 + alpha, 53 + beta, 53 + gamma, 51 + delta, i + 53);
                      STEP_8(2, 6, 22, 54, 54 + alpha, 54 + beta, 54 + gamma, 52 + delta, i + 54);
                      STEP_8(1, 3, 7, 23, 55, 55 + alpha, 55 + beta, 55 + gamma, i + 55);
                      STEP_8(8, 24, 56, 56 + alpha, 56 + beta, 56 + gamma, 53 + delta, 41 + epsilon, i + 56);
                      STEP_8(1, 9, 25, 57, 57 + alpha, 57 + beta, 57 + gamma, 54 + delta, i + 57);
                      STEP_8(2, 10, 26, 58, 58 + alpha, 58 + beta, 58 + gamma, 55 + delta, i + 58);
                      STEP_8(1, 3, 11, 27, 59, 59 + alpha, 59 + beta, 59 + gamma, i + 59);
                      STEP_8(4, 12, 28, 60, 60 + alpha, 60 + beta, 60 + gamma, 56 + delta, i + 60);
                      STEP_8(1, 5, 13, 29, 61, 61 + alpha, 61 + beta, 61 + gamma, i + 61);
                      STEP_8(2, 6, 14, 30, 62, 62 + alpha, 62 + beta, 62 + gamma, i + 62);
                      STEP_8(1, 3, 7, 15, 31, 63, 63 + alpha, 63 + beta, i + 63);

                      CHECK_SOLUTIONS();
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
