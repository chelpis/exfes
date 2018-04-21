#include <stdio.h>
#include "idx_LUT.h"

int main () {

  const int n = 20;
  const int d = 4;

  printf("# n=%d, d=%d\n", n,d);
  printf("1..5\n");

  // generate LUT
  const idx_lut_t * LUT = init_deginvlex_LUT(n,d);
  const idx_lut_t * lex_LUT = init_lex_LUT(n);

  // print LUT
  //print_idx_LUT(LUT);

  // crash-test
  LUT_int_t n_iterations = n_monomials(n,d);

  int ok = 1;
  for(LUT_int_t idx=0; idx<n_iterations; idx++) {
    int set[d];
    int2set(LUT, idx, set);
    if (set2int(LUT, set) != idx) {
      ok = 0;
      printf("not ok 1 - failure at index %d (set2int says %d). set = ", idx, set2int(LUT, set));
      print_idx(d, set);
      printf("\n");
      break;
    }
  }
  if (ok) {
    printf("ok 1 - bijectivity of int2set <--> set2int\n");
  }

  ok = 1;
  for(LUT_int_t idx=0; idx<n_iterations; idx++) {

    if (idx_convert(lex_LUT, LUT, idx_convert(LUT, lex_LUT, idx)) != idx) {
      int set[d];
      int2set(LUT, idx, set);
      ok = 0;
      printf("not ok 2 - failure at index %d (set2int says %d). set = ", idx, set2int(LUT, set));
      print_idx(d, set);
      printf("\n");
      break;
    }
  }
  if (ok) {
    printf("ok 2 - bijectivity of deginvlex <--> lex conversions\n");
  }


  printf("# testing idx_1\n");
  ok = 1;
  for(int i=0; i<n; i++) {
    int set[d];
    for(int j=0; j<d; j++) {
      set[j] = -1;
    }
    set[0] = i;
    LUT_int_t x = set2int(LUT, set);
    if (x != idx_1(LUT->LUT, i)) {
      ok = 0;
    };
  }
 if (ok) {
    printf("ok 3 - idx_1 macro\n");
 } else {
    printf("not ok 3\n");
 }

  printf("# testing idx_2\n");
  ok = 1;
  for(int j=0; j<n; j++) {
    for(int i=0; i<j; i++) {
      int set[d];
      for(int a=0; a<d; a++) {
	set[a] = -1;
      }
      set[0] = j;
      set[1] = i;
      LUT_int_t x = set2int(LUT, set);
      if ( x != idx_2(LUT->LUT, i, j) ) {
	ok = 0;
      }
    }
  }
  if (ok) {
    printf("ok 4 - idx_2 macro\n");
  } else {
    printf("not ok 4\n");
  }

  printf("# testing idx_3\n");
  for(int k=0; k<n; k++) {
    for(int j=0; j<k; j++) {
      for(int i=0; i<j; i++) {
	int set[d];
	for(int a=0; a<d; a++) {
	  set[a] = -1;
	}
	set[0] = k;
	set[1] = j;
	set[2] = i;

	LUT_int_t x = set2int(LUT, set);
	if (x != idx_3(LUT->LUT, i, j, k) ) {
	  ok = 0;
	}
      }
    }
  }
  if (ok) {
    printf("ok 5 - idx_3 macro\n");
  } else {
    printf("not ok 5\n");
  }
  printf("# all test passed\n");

  free_LUT(LUT);
}
