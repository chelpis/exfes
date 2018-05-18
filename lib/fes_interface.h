// fes_interface.h provides
//
// macro definitions:
//      ALGO_AUTO 0
//      ALGO_ENUMERATION 1
//      ALGO_FFT 2
//
// types:
//      solution_callback_t
//      wrapper_settings_t
//
// function declarations:
//      exhaustive_search_wrapper
//      init_settings
//      choose_settings

#ifndef FES_INTERFACE_H
#define FES_INTERFACE_H

#include <stdint.h>  // uint64_t

#define ALGO_AUTO 0
#define ALGO_ENUMERATION 1
#define ALGO_FFT 2

typedef int (*solution_callback_t)(void *, uint64_t, uint64_t *);

typedef struct {
    int algorithm;
    int word_size;
    int algo_enum_self_tune;
    int algo_auto_degree_bound;
    int algo_enum_use_sse;
    int verbose;
} wrapper_settings_t;

int exhaustive_search_wrapper(const int n, int n_eqs, const int degree, int ***coeffs, solution_callback_t callback, void *callback_state);
void init_settings(wrapper_settings_t *result);
void choose_settings(wrapper_settings_t *s, int n, int n_eqs, int degree);

#endif
