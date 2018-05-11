#ifndef EXFES_H
#define EXFES_H

#include <stdint.h>
#include <stdbool.h>

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

int exfes (uint32_t numFixedVariables, uint32_t numVariables, uint32_t numEquations, uint64_t startPointHigh, uint64_t startPointLow, const uint8_t *coefficientsMatrix, bool (*shouldAbortNow)(void), uint64_t *solutionHigh, uint64_t *solutionLow);

#endif
