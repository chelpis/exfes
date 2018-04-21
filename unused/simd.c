#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <assert.h>
#include <getopt.h>


#include "fes.h"
#include "idx_LUT.h"
#include "rand.h"


typedef struct {
  int n;
  int n_eqs;
  system_t f;
  int n_sols;
} state_t;

int report_solution(void * _state, uint64_t size, uint64_t* n_solutions) {
  assert(_state == NULL);
  for(uint64_t i = 0; i < size ; i++)
    printf("%08" PRIx64 "\n", n_solutions[i]);
  return 0;
}

void usage(char **argv) {
  printf("USAGE: ./%s [options]\n", argv[0]);
  printf("--verbose          print extra statistics\n");
  printf("--n k              run test with k variables\n");
  printf("--e k              run test with k equations\n");
  printf("--degree d         run test with degree-d equations\n");
  printf("--seed xxx         initialize the pseudo-random generator with the integer xxx\n");
  printf("--simd_thread k    split the SIMD registers into 2^k smaller registers\n");

  exit(1);
}

int main(int argc, char** argv) {

  int n = 19;
  int n_eqs = 16;
  int degree = 2;
  int verbose = 0;
  unsigned long random_seed = 1;
  int T = 2;
  int ch;


  struct option longopts[7] = {
    { "verbose",     no_argument,          NULL,            'v' },
    { "n",           required_argument,    NULL,            'n' },
    { "e",           required_argument,    NULL,            'e' },
    { "degree",      required_argument,    NULL,            'd' },
    { "seed",        required_argument,    NULL,            's' },
    { "simd_thread", required_argument,    NULL,            'T' },
    { NULL,         0,                     NULL,             0 }
  };

  while ((ch = getopt_long(argc, argv, "", longopts, NULL)) != -1)
    switch (ch) {
    case 'v': verbose = 1; break;
    case 'n': n = atoi(optarg); break;
    case 'e': n_eqs = atoi(optarg); break;
    case 'd': degree = atoi(optarg); break;
    case 's': random_seed = atoi(optarg); break;
    case 'T': T = atoi(optarg); break;
    default: usage(argv);
    }

  if (n > 64 || n_eqs > 32) {
    printf("This hack can only handle up to 64 vars and 32 equations\n");
    exit(1);
  }

 // initialize lookup tables
  idx_lut_t* idx_table = init_deginvlex_LUT(n, degree);
  LUT_t LUT = idx_table->LUT;

  mysrand(random_seed);

  // initalize a random system
  const int N = n_monomials(n,degree);
  pck_vector_t *F = calloc(N, sizeof(pck_vector_t));
  if (F == NULL) {
    perror("impossible to allocate memory for the coefficients");
    exit(1);
  }
  for(int i=0; i<N; i++)
    F[i] = myrand() & ((1ll << n_eqs) - 1);

  uint64_t start = rdtsc();

  switch (degree) {
  case 2:
    switch (T) {
    case 2: exhaustive_sse2_deg_2_T_2_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    case 3: exhaustive_sse2_deg_2_T_3_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    case 4: exhaustive_sse2_deg_2_T_4_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    } break;
  case 3 :
    switch (T) {
    case 2: exhaustive_sse2_deg_3_T_2_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    case 3: exhaustive_sse2_deg_3_T_3_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    case 4: exhaustive_sse2_deg_3_T_4_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    } break;
  case 4 :
    switch (T) {
    case 2: exhaustive_sse2_deg_4_T_2_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    case 3: exhaustive_sse2_deg_4_T_3_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    case 4: exhaustive_sse2_deg_4_T_4_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    } break;
  case 5 :
    switch (T) {
    case 2: exhaustive_sse2_deg_5_T_2_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    case 3: exhaustive_sse2_deg_5_T_3_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    case 4: exhaustive_sse2_deg_5_T_4_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    } break;
  case 6 :
    switch (T) {
    case 2: exhaustive_sse2_deg_6_T_2_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    case 3: exhaustive_sse2_deg_6_T_3_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    case 4: exhaustive_sse2_deg_6_T_4_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    } break;
  case 7 :
    switch (T) {
    case 2: exhaustive_sse2_deg_7_T_2_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    case 3: exhaustive_sse2_deg_7_T_3_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    case 4: exhaustive_sse2_deg_7_T_4_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    } break;
  case 8 :
    switch (T) {
    case 2: exhaustive_sse2_deg_8_T_2_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    case 3: exhaustive_sse2_deg_8_T_3_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    case 4: exhaustive_sse2_deg_8_T_4_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    } break;
  case 9 :
    switch (T) {
    case 2: exhaustive_sse2_deg_9_T_2_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    case 3: exhaustive_sse2_deg_9_T_3_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    case 4: exhaustive_sse2_deg_9_T_4_el_0(LUT, n, F, &report_solution, NULL, verbose);  break;
    } break;
  default:
    printf("enumeration in degree %d is not yet implemented\n", degree);
    exit(0);
  }

  if (verbose) fprintf(stderr, "%.2f cycles/candidate\n", (rdtsc() - start) * 1.0 / (1ll << n));

  free_LUT(idx_table);

  return 0;
}
