#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <assert.h>
#include <getopt.h>
#include <err.h>
#include <sysexits.h>

#include "fes_interface.h"
#include "binomials.h"

#include "rand.h"

int principal_solution_found = 0;
uint64_t principal_solution = 0;

int report_solution(void * _state, uint64_t size, uint64_t* n_solutions) {
  assert(_state == NULL);
  for(uint64_t i = 0; i < size ; i++) {
    if ( n_solutions[i] == principal_solution ) {
      principal_solution_found = 1;
    }
    printf("%016" PRIx64 "\n", n_solutions[i]);
  }
  return 0;
}


void usage(char **argv) {
  printf("USAGE: ./%s [options]\n", argv[0]);
  printf("--verbose          print extra statistics\n");
  printf("--n k              run test with k variables\n");
  printf("--e k              run test with k equations\n");
  printf("--degree d         run test with degree-d equations\n");
  printf("--seed xxx         initialize the pseudo-random generator with the integer xxx\n");
  printf("--eliminate k      generate equations where the first k monomial (the the invlex order) are absent\n");
  printf("--fft              force the use of the FFT algorithm\n");
  printf("--enumeration      force the use of the FFT algorithm\n");

  exit(1);
}

void my_next_set(int n, int d, int set[]) {
  if (d == 0) return;
  set[0] += 1;
  if (set[0] == n) {
    my_next_set(n-1, d-1, &set[1]);
    if (d > 0) set[0] = set[1] + 1;
  }
  }


////////////////////////

int main(int argc, char** argv) {

 int n = 19;
  int n_eqs = 16;
  int degree = 2;
  unsigned long random_seed = 1;
  int el = 0;
  int ch;
  wrapper_settings_t *settings = init_settings();


  struct option longopts[8] = {
    { "verbose",     no_argument,          NULL,            'v' },
    { "fft",         no_argument,          NULL,            'F' },
    { "enumeration", no_argument,          NULL,            'E' },

    { "n",           required_argument,    NULL,            'n' },
    { "e",           required_argument,    NULL,            'e' },
    { "degree",      required_argument,    NULL,            'd' },
    { "seed",        required_argument,    NULL,            's' },
    { NULL,         0,                     NULL,             0 }
  };

  while ((ch = getopt_long(argc, argv, "", longopts, NULL)) != -1)
    switch (ch) {
    case 'v': settings->verbose = 1; break;
    case 'F': settings->algorithm = ALGO_FFT; break;
    case 'E': settings->algorithm = ALGO_ENUMERATION; break;

    case 'n': n = atoi(optarg); break;
    case 'e': n_eqs = atoi(optarg); break;
    case 'd': degree = atoi(optarg); break;
    case 's': random_seed = atoi(optarg); break;
    case 'k': el = atoi(optarg); break;
    default: usage(argv);
    }

  if ( n > 64) {
    errx(EX_USAGE, "This hack can only handle up to 64 vars (but an almost unlimited number of equations");
  }

  if ( degree > n) {
    errx(EX_USAGE, "the degree cannot be larger than the number of equations");
  }


  mysrand(random_seed);

  // allocate the equations & fill them with random coeffs
  int solution[ n ];
  int accumulator[ n_eqs ];
  int set[ n ]; // represent the monomial `m` enumerated below

  int *** eqs = calloc(n_eqs, sizeof(int **));
  for(int e=0; e<n_eqs; e++) {
    accumulator[e] = 0;
    eqs[e] = calloc(degree+1, sizeof(int *));
    for(int d=0; d<=degree; d++) {
      eqs[e][d] = malloc(binomials[n][d] * sizeof(int));
      for(unsigned int m=0; m<binomials[n][d]; m++)
	eqs[e][d][m] = myrand() & 0x0001;
    }
  }

  for(int j=0; j<n; j++) {
    solution[n-1-j] = myrand() & 0x0001;
    principal_solution = (principal_solution << 1) + solution[n-1-j];
  }
  //principal_solution = pack(n, solution);
  printf("Randomly chosen solution = %llx\n", principal_solution);

  // evaluate the equations on the solution (ignoring constant terms)
  for(int d=1; d<degree+1; d++) {
    for(int j=0; j<d; j++) {
      set[j] = d-1-j;
    }
    for(uint64_t m=0; m<binomials[n][d]; m++) {
      // loop invariant: `set` describes the m-th degree-d monomial

      int m_value = 1;
      for(int i=0; i<d; i++) {
	m_value &= solution[ set[i] ];
      }

      for(int e=0; e<n_eqs; e++) {
	accumulator[e] ^= eqs[e][d][m] & m_value;
      }
      my_next_set(n, n, &set[0]); // maintain invariant
    }
  }
  // fix constant terms so that solution is `actually` a solution
  for(int e=0; e<n_eqs; e++) {
    eqs[e][0][0] = accumulator[e];
  }

  exhaustive_search_wrapper(n, n_eqs, degree, eqs, report_solution, NULL, settings);
  return (principal_solution_found ? 0 : 666);
}
