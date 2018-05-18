// fes_interface.h provides
//
// macro definitions:
//      ALGO_AUTO 0
//      ALGO_ENUMERATION 1
//      ALGO_FFT 2
//
// function declarations:
//      exhaustive_search_wrapper

#ifndef FES_INTERFACE_H
#define FES_INTERFACE_H

#include <stdint.h>  // uint64_t

#define ALGO_AUTO 0
#define ALGO_ENUMERATION 1
#define ALGO_FFT 2

int exhaustive_search_wrapper(const int n, int n_eqs, const int degree, int ***coeffs, struct exfes_context *callback_state);

#endif
