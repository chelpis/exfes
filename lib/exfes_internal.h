#ifndef EXFES_INTERNAL_H
#define EXFES_INTERNAL_H

#include <stdbool.h>
#include <stdint.h>

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

#endif
