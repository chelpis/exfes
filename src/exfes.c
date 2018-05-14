#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "fes_interface.h"
#include "exfes.h"

int C (int n, int m) {
	if (m == 0)
		return 1;
	else if (m == 1)
		return n;
	else if (m == 2)
		return n * (n - 1) >> 1;
	else
		return 0;
}

int M (uint64_t startPointHigh, uint64_t startPointLow, int index) {
	if (index < 64)
		return (startPointLow >> index) & 1;
	else
		return (startPointHigh >> (index - 64)) & 1;
}

void freeEqs(int ***Eqs, int count_i, int count_j) {
	for (int i=0; i<count_i; i++) {
		for (int j=0; j<count_j; j++) {
			free(Eqs[i][j]);
		}
		free(Eqs[i]);
	}
	free(Eqs);
}

void Transform_Data_Structure (int n, int e, const uint8_t *coefficientsMatrix, int ***Eqs) {
	uint64_t offset = 0;
	for (int i=0; i<e; i++) {
		for (int j=0; j<C(n, 2); j++)
			Eqs[i][2][j] = (int)coefficientsMatrix[offset+j];
		offset += C(n, 2);
		for (int j=0; j<n; j++)
			Eqs[i][1][j] = (int)coefficientsMatrix[offset+j];
		offset += n;
		Eqs[i][0][0] = (int)coefficientsMatrix[offset];
		offset += 1;
	}
}

int Merge_Solution (void *_ctx_ptr, uint64_t count, uint64_t *Sol) {
	struct exfes_context *p = (struct exfes_context*) _ctx_ptr;

	int	const mcopy = p -> mcopy;
	int	const ncopy = p -> ncopy;
	uint64_t const solm	= p -> solm;
	uint64_t const startPointHigh = p -> startPointHigh;
	uint64_t const startPointLow = p -> startPointLow;
	uint64_t *solutionHigh = p -> solutionHigh;
	uint64_t *solutionLow = p -> solutionLow;

	solutionLow[0] = (Sol[count-1] << mcopy) ^ solm;

	if (mcopy > 0) {
		solutionHigh[0] = Sol[count-1] >> (64 - mcopy);
	}

	if (ncopy < 64) {
		solutionLow[0] ^= (startPointLow << (64 - ncopy)) >> (64 - ncopy);
	} else {
		solutionLow[0] ^= startPointLow;
		solutionHigh[0] ^= (startPointHigh << (128 - ncopy)) >> (128 - ncopy);
	}
	p -> SolCount = 1;

	return 1;
}

int exfes (uint32_t numFixedVariables, uint32_t numVariables, uint32_t numEquations, uint64_t startPointHigh, uint64_t startPointLow, const uint8_t *coefficientsMatrix, bool (*shouldAbortNow)(void), uint64_t *solutionHigh, uint64_t *solutionLow) {

	int m = numFixedVariables;
	int n = numVariables;
	int e = numEquations;

	if (n == 0 || n >= 127)
		return -3;
	else if (n - m <= 0 || n - m >= 64)
		return -3;
	else if (e <= 16 || e >= 256)
		return -3;

	struct exfes_context exfes_ctx;
	exfes_ctx.mcopy = m;
	exfes_ctx.ncopy = n;
	exfes_ctx.solm = 0;
	exfes_ctx.SolCount = 0;
	exfes_ctx.startPointHigh = startPointHigh;
	exfes_ctx.startPointLow = startPointLow;
	exfes_ctx.solutionHigh = solutionHigh;
	exfes_ctx.solutionLow = solutionLow;
	exfes_ctx.shouldAbortNow = shouldAbortNow;

	// Make a copy of EqsUnmask for masking.
	int ***Eqs = calloc(e, sizeof(int **));
	for (int i=0; i<e; i++) {
		Eqs[i] = calloc(3, sizeof(int *));
		for (int j=0; j<3; j++) {
			Eqs[i][j] = calloc(C(n, j), sizeof(int));
		}
	}
	Transform_Data_Structure (n, e, coefficientsMatrix, Eqs);

	// Mask Eqs for a random start point.
	for (int i=0; i<e; i++) {
		for (int j=0; j<n; j++)
			Eqs[i][0][0] ^= Eqs[i][1][j] & M(startPointHigh, startPointLow, j);
		int offset = 0;
		for (int j=0; j<n-1; j++)
			for (int k=j+1; k<n; k++) {
				Eqs[i][0][0] ^= Eqs[i][2][offset] & M(startPointHigh, startPointLow, j) & M(startPointHigh, startPointLow, k);
				Eqs[i][1][j] ^= Eqs[i][2][offset] & M(startPointHigh, startPointLow, k);
				Eqs[i][1][k] ^= Eqs[i][2][offset] & M(startPointHigh, startPointLow, j);
				offset += 1;
			}
	}
	// Make a copy of Eqs for evaluating fixed variables.
	int ***EqsCopy = calloc(e, sizeof(int **));
	for (int i=0; i<e; i++) {
		EqsCopy[i] = calloc(3, sizeof(int *));
		for (int j=0; j<3; j++)
			EqsCopy[i][j] = malloc(C(n, j) * sizeof(int));
	}

	// Partition problem into (1<<n_fixed) sub_problems.
	int p = n - m;
	int npartial;
	int fixvalue;
	for (exfes_ctx.solm=0; exfes_ctx.solm<(uint64_t)1<<m; exfes_ctx.solm++) {
		// Initialize npartial and EqsCopy.
		npartial = n;
		for (int i=0; i<e; i++)
			for (int j=0; j<3; j++)
				for (int k=0; k<C(n, j); k++)
					EqsCopy[i][j][k] = Eqs[i][j][k];
		// Fix m variables.
		while (npartial != p) {
			fixvalue = (exfes_ctx.solm >> (n - npartial)) & 1;
			for (int i=0; i<e; i++) {
				// Fix a variable.
				for (int j=0; j<npartial-1; j++)
					EqsCopy[i][1][j+1] ^= EqsCopy[i][2][j] & fixvalue;
				EqsCopy[i][0][0] ^= EqsCopy[i][1][0] & fixvalue;
				// Shrink EqsCopy.
				for (int j=0; j<npartial-1; j++)
					EqsCopy[i][1][j] = EqsCopy[i][1][j+1];
				for (int j=0; j<C(npartial-1, 2); j++)
					EqsCopy[i][2][j] = EqsCopy[i][2][j+npartial-1];
			}
			npartial -= 1;
		}

		exhaustive_search_wrapper(npartial, e, 2, EqsCopy, Merge_Solution, &exfes_ctx, 0);

		// Determine to early abort or not.
		if (exfes_ctx.SolCount == 1)
			break;
		else if (exfes_ctx.SolCount == 2)
			break;
	}

	freeEqs(EqsCopy, e, 3);
	freeEqs(Eqs, e, 3);

	if (exfes_ctx.SolCount == 1)
		return 0;
	else if (exfes_ctx.SolCount == 0) 
		return -1;
	else
		return -2;
}
