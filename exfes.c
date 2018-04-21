#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "fes_interface.h"
#include "exfes.h"

int mcopy;
uint64_t solm;
uint64_t **SolMerge;
uint64_t SolCount;
uint64_t MaxSolCount;

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

int Merge_Solution (void *unused, uint64_t count, uint64_t *Sol) {
	while (count > 0 && SolCount < MaxSolCount) {
		SolMerge[SolCount][0] = (Sol[count-1] << mcopy) ^ solm;
		SolMerge[SolCount][1] = Sol[count-1] >> (64 - mcopy);
		SolCount += 1;
		count -= 1;
	}
	return 0;
}

void exfes (int m, int n, int e, uint64_t maxsol, int ***Eqs, uint64_t **SolArray) {
	wrapper_settings_t *Settings = init_settings();
	mcopy = m;
	SolMerge = SolArray;
	SolCount = 0;
	MaxSolCount = maxsol;
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
