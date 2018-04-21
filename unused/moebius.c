#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <assert.h>
#include <getopt.h>

#include "fes.h"
#include "idx_LUT.h"
#include "rand.h"



int report_solution(void * _state, uint64_t size, uint64_t* n_solutions) {
  assert(_state == NULL);
  for(uint64_t i = 0; i < size ; i++)
    printf("%016" PRIx64 "\n", n_solutions[i]);
  return 0;
}


void usage(char **argv) {
  printf("USAGE: ./%s [options]\n", argv[0]);
  printf("--verbose          print extra statistics\n");
  printf("--n k              run test with k variables\n");
  printf("--e k              run test with k equations\n");
  printf("--degree d         run test with degree-d equations\n");
  printf("--seed xxx         initialize the pseudo-random generator with the integer xxx\n");

  exit(1);
}

////////////////////////
int main(int argc, char** argv) {

  int n = 19;
  int n_eqs = 16;
  int degree = 2;
  int verbose = 0;
  unsigned long random_seed = 1;
  int ch;

  struct option longopts[6] = {
    { "verbose",     no_argument,          NULL,            'v' },
    { "n",           required_argument,    NULL,            'n' },
    { "e",           required_argument,    NULL,            'e' },
    { "degree",      required_argument,    NULL,            'd' },
    { "seed",        required_argument,    NULL,            's' },
    { NULL,         0,                     NULL,             0 }
  };

  while ((ch = getopt_long(argc, argv, "", longopts, NULL)) != -1)
    switch (ch) {
    case 'v': verbose = 1; break;
    case 'n': n = atoi(optarg); break;
    case 'e': n_eqs = atoi(optarg); break;
    case 'd': degree = atoi(optarg); break;
    case 's': random_seed = atoi(optarg); break;
    default: usage(argv);
    }

  if (n > 64 || n_eqs > 32) {
    printf("This hack can only handle up to 64 vars and 32 equations\n");
    exit(1);
  }

  srand(random_seed);
 // initialize lookup tables
  idx_lut_t* deginvlex_LUT = init_deginvlex_LUT(n, degree);
  idx_lut_t* lex_LUT = init_lex_LUT(n);


  // initalize a random degree-d system
  const int N = n_monomials(n,degree);
  pck_vector_t *F = calloc(1 << n, sizeof(pck_vector_t));
  for(int i=0; i<N; i++)
    F[i] = myrand() & ((1ll << n_eqs) - 1);

  // scatter it to the full array of all possible monomials
  pck_vector_t *A = calloc(1 << n, sizeof(pck_vector_t));
  if (F == NULL || A == NULL) {
    printf("Not enough RAM to perform this test. Try a smaller number of variables\n");
    exit(1);
  }
  for(int i=0; i<N; i++)
    A[ idx_convert(deginvlex_LUT, lex_LUT, i) ] = F[i];
  

  uint64_t start = rdtsc();

  moebius_transform(n, A, &report_solution, NULL);

  if (verbose) fprintf(stderr, "%.2f cycles/candidate\n", (rdtsc() - start) * 1.0 / (1ll << n));

  free_LUT(deginvlex_LUT);
  free_LUT(lex_LUT);
  return 0;
}
