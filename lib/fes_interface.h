// fes_interface.h provides
//
// function declarations:
//      exhaustive_search_wrapper

#ifndef FES_INTERFACE_H_INCLUDED
#define FES_INTERFACE_H_INCLUDED

#include "exfes_internal.h"  // struct exfes_context

int exhaustive_search_wrapper(const int n, int n_eqs, const int degree, int ***coeffs, struct exfes_context *exfes_ctx_ptr);

#endif  // FES_INTERFACE_H_INCLUDED
