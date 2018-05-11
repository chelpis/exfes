#ifndef EXFES_H
#define EXFES_H

#include <stdint.h>

void exfes (uint32_t numFixedVariables, uint32_t numVariables, uint32_t numEquations, uint64_t startPointHigh, uint64_t startPointLow, int ***Eqs, uint64_t *solutionHigh, uint64_t *solutionLow);

#endif
