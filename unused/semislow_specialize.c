#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <assert.h>

#include "fes.h"
#include "idx_LUT.h"

int main(int argc, char** argv) {

  int n = 19;
  int n_eqs = 16;
  int degree = 2;
  int random_seed = 0;
  int verbose = 0;

  if (argc > 1) {
    n = atoi(argv[1]);
    n_eqs = atoi(argv[2]);
    degree = atoi(argv[3]);
    verbose = atoi(argv[4]);    
  }
  
  if (argc > 6) random_seed = atoi(argv[5]);


  if (n > 64 || n_eqs > 32) {
    printf("This hack can only handle up to 64 vars and 64 equations\n");
    exit(1);
  }
 
  // initialize lookup tables
  idx_lut_t* idx_table = init_deginvlex_LUT(n, degree);
  LUT_t LUT = idx_table->LUT;

  srand(random_seed);

  // initalize a random system
  const int N = n_monomials(n, degree);
  pck_vector_t *F = calloc(N, sizeof(pck_vector_t)); 
  if (F == NULL) {
    perror("impossible to allocate memory for the coefficients");
    exit(1);
  } 
  for(int i=0; i<N; i++)
    F[i] = (rand() ^ (rand() << 16)) & ((1ll << n_eqs) - 1);


  if (verbose) printf("this is is used to test variable specialization...\n");


  // we specialize the last T variables and try the exhaustive search
  const int T=5;
  for(int k=0; k<(1<<T); k++) {

    pck_vector_t A[N];
    for(int i=0; i<N; i++)
      A[i] = F[i];

    variables_specialization(LUT, n, degree, A, T, k);
    
    // now, exhaustive search
    for(uint64_t i=0; i<(1ull << (n-T)); i++) 
      if (packed_eval(LUT, n-T, degree, A, i) == 0) 
	printf("%08" PRIx64 "\n", i + k*(1<<(n-T)));
  }
  
  free_LUT(idx_table);
  return 0;
}
