#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "fes_interface.h"
#include "exfes.h"

int mcopy;
int ncopy;
uint64_t solm;
uint64_t **SolMerge;
uint64_t SolCount;
uint64_t MaxSolCount;
uint64_t *MaskCopy;

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

int M (uint64_t *Mask, int index) {
	if (index < 64)
		return (Mask[0] >> index) & 1;
	else
		return (Mask[1] >> (index - 64)) & 1;
}

int Merge_Solution (void *unused, uint64_t count, uint64_t *Sol) {
	assert(unused == NULL);
	while (count > 0 && SolCount < MaxSolCount) {
		// Merge m-bit solution and (n-m)-bit solution.
		SolMerge[SolCount][0] = (Sol[count-1] << mcopy) ^ solm;
		if (mcopy > 0)
			SolMerge[SolCount][1] = Sol[count-1] >> (64 - mcopy);
		// Unmask solution.
		if (ncopy < 64)
			SolMerge[SolCount][0] ^= (MaskCopy[0] << (64 - ncopy)) >> (64 - ncopy);
		else {
			SolMerge[SolCount][0] ^= MaskCopy[0];
			SolMerge[SolCount][1] ^= (MaskCopy[1] << (128 - ncopy)) >> (128 - ncopy);
		}
		SolCount += 1;
		count -= 1;
	}
	return 0;
}

void exfes (int m, int n, int e, uint64_t *Mask, uint64_t maxsol, int ***Eqs, uint64_t **SolArray) {
	wrapper_settings_t *Settings = init_settings();
	mcopy = m;
	ncopy = n;
	SolMerge = SolArray;
	SolCount = 0;
	MaxSolCount = maxsol;
	MaskCopy = Mask;
	// Mask Eqs for a random start point.
	for (int i=0; i<e; i++) {
		for (int j=0; j<n; j++)
			Eqs[i][0][0] ^= Eqs[i][1][j] & M(Mask, j);
		int offset = 0;
		for (int j=0; j<n-1; j++)
			for (int k=j+1; k<n; k++) {
				Eqs[i][0][0] ^= Eqs[i][2][offset] & M(Mask, j) & M(Mask, k);
				Eqs[i][1][j] ^= Eqs[i][2][offset] & M(Mask, k);
				Eqs[i][1][k] ^= Eqs[i][2][offset] & M(Mask, j);
				offset += 1;
			}
	}
	// Make a copy of Eqs for evaluating fixed variables.
	int *** EqsCopy = calloc(e, sizeof(int **));
	for (int i=0; i<e; i++) {
		EqsCopy[i] = calloc(3, sizeof(int *));
		for (int j=0; j<3; j++)
			EqsCopy[i][j] = malloc(C(n, j) * sizeof(int));
	}
	// Partition problem into (1<<n_fixed) sub_problems.
	int p = n - m;
	int ncopy;
	int fixvalue;
	for (solm=0; solm<(uint64_t)1<<m; solm++) {
		// Initialize ncopy and EqsCopy.
		ncopy = n;
		for (int i=0; i<e; i++)
			for (int j=0; j<3; j++)
				for (int k=0; k<C(n, j); k++)
					EqsCopy[i][j][k] = Eqs[i][j][k];
		// Fix m variables.
		while (ncopy != p) {
			fixvalue = (solm >> (n - ncopy)) & 1;
			for (int i=0; i<e; i++) {
				// Fix a variable.
				for (int j=0; j<ncopy-1; j++)
					EqsCopy[i][1][j+1] ^= EqsCopy[i][2][j] & fixvalue;
				EqsCopy[i][0][0] ^= EqsCopy[i][1][0] & fixvalue;
				// Shrink EqsCopy.
				for (int j=0; j<ncopy-1; j++)
					EqsCopy[i][1][j] = EqsCopy[i][1][j+1];
				for (int j=0; j<C(ncopy-1, 2); j++)
					EqsCopy[i][2][j] = EqsCopy[i][2][j+ncopy-1];
			}
			ncopy -= 1;
		}
		exhaustive_search_wrapper(ncopy, e, 2, EqsCopy, Merge_Solution, NULL, Settings);
	}
}
