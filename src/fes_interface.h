#ifndef FES_INTERFACE_H
#define FES_INTERFACE_H

typedef int (*solution_callback_t)(void *, uint64_t, uint64_t*);

#define ALGO_AUTO        0
#define ALGO_ENUMERATION 1
#define ALGO_FFT         2

typedef struct {
  int algorithm;
  int word_size;

  int algo_enum_self_tune;
  int algo_auto_degree_bound;
  int algo_enum_use_sse;
  int verbose;
} wrapper_settings_t;


/* In principe, this function should be the single entry point to the library
   The input format is explicit, except for the coefficients of the equations.
   coeffs[e][d][m] = coefficient of the m-th monomial of degree d in the e-th equation
   the monomials of each degree are assumed to be in invlex order (i.e. in lex order with reversed variables)
*/
int exhaustive_search_wrapper(const int n, int n_eqs, const int degree, int ***coeffs, solution_callback_t callback, void* callback_state, wrapper_settings_t *settings);

wrapper_settings_t * init_settings();
void choose_settings( wrapper_settings_t *s, int n, int n_eqs, int degree);

#endif
