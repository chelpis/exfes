// exfes_internal.h provides
//
// types:
//      struct exfes_context

#ifndef EXFES_INTERNAL_H_INCLUDED
#define EXFES_INTERNAL_H_INCLUDED

#include <stdbool.h>  // bool
#include <stdint.h>  // uint64_t

struct exfes_context {
    int mcopy;
    int ncopy;
    uint64_t solm;
    uint64_t SolCount;
    uint64_t startPointHigh;
    uint64_t startPointLow;
    uint64_t *solutionHigh;
    uint64_t *solutionLow;
    bool (*shouldAbortNow)(void);
};

#endif  // EXFES_INTERNAL_H_INCLUDED
