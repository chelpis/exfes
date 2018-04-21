#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <assert.h>


#include "fes.h"
#include "idx_LUT.h"
#include "rand.h"


typedef void (*enumeration_t)(LUT_t, int, pck_vector_t *, solution_callback_t, void*, int);

  int terms_to_kill[10][32] = { {}, 
				{},
				{3, 5, 6, 8, 9, 10, 12, 13, 14, 15, 17, 18, 19, 20, 21, 23, 24, 25, 26, 27, 28, 30, 31, 32, 33, 34, 35, 36, 38, 39, 40, 41},
				{7, 11, 13, 14, 18, 20, 21, 23, 24, 25, 29, 31, 32, 34, 35, 36, 38, 39, 40, 41, 45, 47, 48, 50, 51, 52, 54, 55, 56, 57, 59, 60},
				{15, 23, 27, 29, 30, 38, 42, 44, 45, 49, 51, 52, 54, 55, 56, 64, 68, 70, 71, 75, 77, 78, 80, 81, 82, 86, 88, 89, 91, 92, 93, 95},
				{31, 47, 55, 59, 61, 62, 78, 86, 90, 92, 93, 101, 105, 107, 108, 112, 114, 115, 117, 118, 119, 135, 143, 147, 149, 150, 158, 162, 164, 165, 169, 171},
				{63, 95, 111, 119, 123, 125, 126, 158, 174, 182, 186, 188, 189, 205, 213, 217, 219, 220, 228, 232, 234, 235, 239, 241, 242, 244, 245, 246, 278, 294, 302, 306},
				{127, 191, 223, 239, 247, 251, 253, 254, 318, 350, 366, 374, 378, 380, 381, 413, 429, 437, 441, 443, 444, 460, 468, 472, 474, 475, 483, 487, 489, 490, 494, 496},
				{255, 383, 447, 479, 495, 503, 507, 509, 510, 638, 702, 734, 750, 758, 762, 764, 765, 829, 861, 877, 885, 889, 891, 892, 924, 940, 948, 952, 954, 955, 971, 979},
				{511, 767, 895, 959, 991, 1007, 1015, 1019, 1021, 1022, 1278, 1406, 1470, 1502, 1518, 1526, 1530, 1532, 1533, 1661, 1725, 1757, 1773, 1781, 1785, 1787, 1788, 1852, 1884, 1900, 1908, 1912}};


enumeration_t solvers[32] = {
  exhaustive_sse2_deg_2_el_0, 
  exhaustive_sse2_deg_2_el_1, 
  exhaustive_sse2_deg_2_el_2, 
  exhaustive_sse2_deg_2_el_3,
  exhaustive_sse2_deg_2_el_4,
  exhaustive_sse2_deg_2_el_5,
  exhaustive_sse2_deg_2_el_6,
  exhaustive_sse2_deg_2_el_7,
  exhaustive_sse2_deg_2_el_8,
  exhaustive_sse2_deg_2_el_9,
  exhaustive_sse2_deg_2_el_10,
  exhaustive_sse2_deg_2_el_11,
  exhaustive_sse2_deg_2_el_12,
  exhaustive_sse2_deg_2_el_13,
  exhaustive_sse2_deg_2_el_14,
  exhaustive_sse2_deg_2_el_15,
  exhaustive_sse2_deg_2_el_16,
  exhaustive_sse2_deg_2_el_17,
  exhaustive_sse2_deg_2_el_18,
  exhaustive_sse2_deg_2_el_19,
  exhaustive_sse2_deg_2_el_20,
  exhaustive_sse2_deg_2_el_21,
  exhaustive_sse2_deg_2_el_22,
  exhaustive_sse2_deg_2_el_23,
  exhaustive_sse2_deg_2_el_24,
  exhaustive_sse2_deg_2_el_25,
  exhaustive_sse2_deg_2_el_26,
  exhaustive_sse2_deg_2_el_27,
  exhaustive_sse2_deg_2_el_28,
  exhaustive_sse2_deg_2_el_29,
  exhaustive_sse2_deg_2_el_30,
  exhaustive_sse2_deg_2_el_31};


int  report_solution(void *state, uint64_t i) {
  assert(state == NULL);
  printf("%016" PRIx64 "\n", i);
  return 0;
}


int main(int argc, char **argv) {

  //  srand(time(0));

  int random_seed = 0;
  int n = 19;
  int n_eqs = 16;
  int degree = 2;
  int verbose = 0;
  int el = 1; 

  if (argc > 1) {
    n = atoi(argv[1]);
    n_eqs = atoi(argv[2]);
    degree = atoi(argv[3]);
    el = atoi(argv[4]);
    verbose = atoi(argv[5]);    
  }
  
  if (argc > 7) random_seed = atoi(argv[6]);

  if (n > 64 || n_eqs > 32 || el > 32) {
    printf("This hack can only handle up to 64 vars and 32 equations (and you can only eliminate at most 32 stuff\n");
    exit(1);
  }
 
 // initialize lookup tables
  idx_lut_t* idx_table = init_deginvlex_LUT(n, degree);
  LUT_t LUT = idx_table->LUT;

  srand(random_seed);

  // initalize a random system
  const int N = n_monomials(n,degree);
  pck_vector_t *F = calloc(N, sizeof(pck_vector_t)); 
  if (F == NULL) {
    perror("impossible to allocate memory for the coefficients");
    exit(1);
  } 
  for(int i=0; i<N; i++)
    F[i] = myrand() & ((1ll << n_eqs) - 1);

  for(int i=0; i<el; i++)
    F[ terms_to_kill[degree][i] ] = 0;

  uint64_t start = rdtsc();

  if (degree > 9) {
    printf("enumeration in degree %d is not yet implemented\n", degree);
    exit(1);
  }

  switch (degree) {
  case 2: (*solvers[el])(LUT, n, F, &report_solution, NULL, verbose); break;
  case 3: exhaustive_sse2_deg_3_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
  case 4: exhaustive_sse2_deg_4_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
  case 5: exhaustive_sse2_deg_5_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
  case 6: exhaustive_sse2_deg_6_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
  case 7: exhaustive_sse2_deg_7_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
  case 8: exhaustive_sse2_deg_8_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
  case 9: exhaustive_sse2_deg_9_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
  default:
    printf("enumeration in degree %d is not yet implemented\n", degree);
    exit(0);
  }


  fprintf(stderr, "%.2f cycles/candidate\n", (rdtsc() - start) * 1.0 / (1ll << n));

  free_LUT(idx_table);

  return 0;
}
