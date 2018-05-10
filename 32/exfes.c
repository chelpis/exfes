#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "fes_interface.h"
#include "exfes.h"

struct exfes_context {
	int mcopy;
	int ncopy;
	uint64_t solm;
	uint64_t **SolMerge;
	uint64_t SolCount;
	uint64_t MaxSolCount;
	uint64_t *MaskCopy;
};

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

// 1. 已知只會有一組解
// 2. 傳遞一個 struct exfes_context 物件的指標
int Merge_Solution (void *_ctx_ptr, uint64_t count, uint64_t *Sol) {
	struct exfes_context *p = (struct exfes_context*) _ctx_ptr;

	int	    const mcopy       = p -> mcopy	 ;
	int	    const ncopy       = p -> ncopy	 ;
	uint64_t    const solm	      = p -> solm	 ;
	uint64_t ** const SolMerge    = p -> SolMerge	 ;
	uint64_t    const SolCount    = p -> SolCount	 ; // XXX value is 1
//	uint64_t    const MaxSolCount = p -> MaxSolCount ; // XXX value is 1
	uint64_t  * const MaskCopy    = p -> MaskCopy	 ;

	SolMerge[SolCount][0] = (Sol[count-1] << mcopy) ^ solm;

	if (mcopy > 0) {
		SolMerge[SolCount][1] = Sol[count-1] >> (64 - mcopy);
	}

	if (ncopy < 64) {
		SolMerge[SolCount][0] ^= (MaskCopy[0] << (64 - ncopy)) >> (64 - ncopy);
	} else {
		SolMerge[SolCount][0] ^= MaskCopy[0];
		SolMerge[SolCount][1] ^= (MaskCopy[1] << (128 - ncopy)) >> (128 - ncopy);
	}

	return 1;
}

void exfes (int m, int n, int e, uint64_t *Mask, uint64_t maxsol, int ***EqsUnmask, uint64_t **SolArray) {
	struct exfes_context exfes_ctx;
	exfes_ctx.mcopy = m;
	exfes_ctx.ncopy = n;
	exfes_ctx.solm = 0;
	exfes_ctx.SolMerge = SolArray;
	exfes_ctx.SolCount = 0;
	exfes_ctx.MaxSolCount = maxsol;
	exfes_ctx.MaskCopy = Mask;

	// Make a copy of EqsUnmask for masking.
	int ***Eqs = calloc(e, sizeof(int **));
	for (int i=0; i<e; i++) {
		Eqs[i] = calloc(3, sizeof(int *));
		for (int j=0; j<3; j++) {
			Eqs[i][j] = malloc(C(n, j) * sizeof(int));
			for (int k=0; k<C(n, j); k++)
				Eqs[i][j][k] = EqsUnmask[i][j][k];
		}
	}
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

		// Determine to early aborb or not.
		if (exfes_ctx.SolCount == 1)
			return;
	}
}
