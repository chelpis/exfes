// fes_interface.h provides
//
// macro definitions:
//      ALGO_AUTO 0
//      ALGO_ENUMERATION 1
//      ALGO_FFT 2
//
// types:
//      solution_callback_t
//
// function declarations:
//      exhaustive_search_wrapper

#ifndef FES_INTERFACE_H
#define FES_INTERFACE_H

#include <stdint.h>  // uint64_t

#define ALGO_AUTO 0
#define ALGO_ENUMERATION 1
#define ALGO_FFT 2

typedef int (*solution_callback_t)(void *, uint64_t, uint64_t *);

int exhaustive_search_wrapper(const int n, int n_eqs, const int degree, int ***coeffs, solution_callback_t callback, void *callback_state);

#endif
