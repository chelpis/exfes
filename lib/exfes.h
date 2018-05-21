// exfes.h provides
//
// function declarations:
//      exfes

#ifndef EXFES_H_INCLUDED
#define EXFES_H_INCLUDED

#include <stdbool.h>  // bool
#include <stdint.h>  // uint8_t, uint32_t, uint64_t

#ifdef __cplusplus
extern "C" {
#endif

int exfes(uint32_t numFixedVariables, uint32_t numVariables, uint32_t numEquations, uint64_t startPointHigh, uint64_t startPointLow, const uint8_t *coefficientsMatrix, bool (*shouldAbortNow)(void), uint64_t *solutionHigh, uint64_t *solutionLow);

#ifdef __cplusplus
}
#endif

#endif  // EXFES_H_INCLUDED
