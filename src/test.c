#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>

#include "exfes.h"

// Define the binomial function to calculate number of terms in different degrees.
int B (int n, int m) {
	if (m == 0)
		return 1;
	else if (m == 1)
		return n;
	else if (m == 2)
		return n * (n - 1) >> 1;
	else
		return 0;
}

// Define a function to find the index-th bit of Sol.
int S (uint64_t solutionHigh, uint64_t solutionLow, int index) {
	if (index < 64)
		return (solutionLow >> index) & 1;
	else
		return (solutionHigh >> (index - 64)) & 1;
}

bool otherNodeReady(void) {
	return 1;
}

// Define a function to print equations for debugging.
void Print_Equation (int n, int e, int ***Eqs) {
	for (int i=0; i<e; i++) {
		printf("Eqs[%d] = ", i);
		for (int j=0; j<3; j++)
			for (int k=0; k<B(n, j); k++)
				printf("%d ", Eqs[i][j][k]);
		printf("\n");
	}
}

// Define a function set all equation elements to zero.
int ***Initialize_Equations (int n, int e) {
	int ***Eqs = (int ***)calloc(e, sizeof(int **)); // Create an array for saving coefficients for exfes.
	for (int i=0; i<e; i++) {
		Eqs[i] = (int **)calloc(3, sizeof(int *));
		for (int j=0; j<3; j++) {
			Eqs[i][j] = (int *)calloc(B(n, j), sizeof(int));
		}
	}
	return Eqs;
}

void Free_Equations (int e, int ***Eqs) {
	for (int i=0; i<e; i++) {
		for (int j=0; j<3; j++) {
			free(Eqs[i][j]);
		}
		free(Eqs[i]);
	}
	free(Eqs);
}

// Transform coefficientsMatrix into the structure required by exfes.
void Transform_Data_Structure_Temp (int n, int e, uint8_t *coefficientsMatrix, int ***Eqs) {
	uint64_t offset = 0;
	for (int i=0; i<e; i++) {
		for (int j=0; j<B(n, 2); j++)
			coefficientsMatrix[offset+j] = Eqs[i][2][j];
		offset += B(n, 2);
		for (int j=0; j<n; j++)
			coefficientsMatrix[offset+j] = Eqs[i][1][j];
		offset += n;
		coefficientsMatrix[offset] = Eqs[i][0][0];
		offset += 1;
	}
}

// Define a function to generate a solution randomly.
void Generate_Solution (int n, uint64_t *genSolutionHigh, uint64_t *genSolutionLow) {
	genSolutionLow[0] = 0;
	genSolutionHigh[0] = 0;
	if (n <= 64)
		for (int i=0; i<n; i++) {
			genSolutionLow[0] = genSolutionLow[0] << 1;
			genSolutionLow[0] = genSolutionLow[0] ^ (rand() & 1);
		}
	else {
		for (int i=0; i<64; i++) {
			genSolutionLow[0] = genSolutionLow[0] << 1;
			genSolutionLow[0] = genSolutionLow[0] ^ (rand() & 1);
		}
		for (int i=0; i<n-64; i++) {
			genSolutionHigh[0] = genSolutionHigh[0] << 1;
			genSolutionHigh[0] = genSolutionHigh[0] ^ (rand() & 1);
		}
	}
}

// Define a function to fix the costant term of equations so that they fit the generated solution.
int Evaluate_Solution (int n, uint64_t solutionHigh, uint64_t solutionLow, int **Eqs) {
	int val = 0;
	val ^= Eqs[0][0];
	for (int i=0; i<n; i++)
		val ^= Eqs[1][i] & S(solutionHigh, solutionLow, i);
	int offset = 0;
	for (int i=0; i<n-1; i++) {
		for (int j=i+1; j<n; j++)
			val ^= Eqs[2][offset+j-(i+1)] & S(solutionHigh, solutionLow, i) & S(solutionHigh, solutionLow, j);
		offset += n - (i + 1);
	}
	return val;
}

// Define a function to randomly generate equations that fit the generated solution.
void Generate_Equation (int n, int e, uint64_t solutionHigh, uint64_t solutionLow, int ***Eqs) {
	for (int i=0; i<e; i++)
		for (int j=0; j<3; j++)
			for (int k=0; k<B(n, j); k++)
				Eqs[i][j][k] = rand() & 1;
	int val;
	for (int i=0; i<e; i++) {
		val = Evaluate_Solution(n, solutionHigh, solutionLow, Eqs[i]);
		Eqs[i][0][0] ^= val;
	}
}

// Define a function to print solutions obtained from exfes.
void Report_Solution (uint64_t solutionHigh, uint64_t solutionLow) {
	printf("    Solution = %016"PRIx64"%016"PRIx64"\n", solutionHigh, solutionLow);
}

// Define a function to check solution.
void Check_Solution (int n, int e, uint64_t solutionHigh, uint64_t solutionLow, int ***Eqs) {
	int count = 0;
	for (int i=0; i<e; i++) {
		int val = Evaluate_Solution(n, solutionHigh, solutionLow, Eqs[i]);
		if (val == 0)
			count += 1;
	}
	printf("    Fitting Equations = %d\n", count);
}

int main (int argc, char **argv) {

	uint32_t m = 17; // M variables will be fixed by exfes before calling libFES.
	uint32_t n = 80; // Test exfes with n variables.
	uint32_t e = 72; // Test exfes with e equations.

	int ch;
	struct option longopts[4] = {
		{ "m"	, required_argument	, NULL, 'm'	},
		{ "n"	, required_argument	, NULL, 'n'	},
		{ "e"	, required_argument	, NULL, 'e'	},
		{ NULL	, 0					, NULL,  0	}
	};

	while ((ch = getopt_long(argc, argv, "", longopts, NULL)) != -1) {
		switch (ch) {
			case 'm': m = atoi(optarg); break;
			case 'n': n = atoi(optarg); break;
			case 'e': e = atoi(optarg); break;
			default: ;
		}
	}

	int ***Eqs = Initialize_Equations(n, e); // Set all array elements to zero.

	uint64_t Mask[2] = {0xe0d3cf665fad7012, 0x000000000000e5eb}; // The solver starts searching from the value of mask.

	uint64_t solutionHigh = 0;
	uint64_t solutionLow = 0;

	// Generate a solution randomly.
	printf("Generate a solution randomly ...\n");
	uint64_t genSolutionHigh;
	uint64_t genSolutionLow;
	Generate_Solution(n, &genSolutionHigh, &genSolutionLow);
	printf("    Solution = ""%016"PRIx64"%016"PRIx64"\n", genSolutionHigh, genSolutionLow);

	// Generate equations randomly.
	//printf("Generate equations randomly ...\n");
	Generate_Equation(n, e, genSolutionHigh, genSolutionLow, Eqs);
	//Print_Equation(n, e, Eqs);

	// Check generated equations.
	Check_Solution(n, e, genSolutionHigh, genSolutionLow, Eqs);

	// Solve equations by exfes.
	uint8_t *coefficientsMatrix = (uint8_t *)calloc(e * (B(n, 2) + B(n, 1) + B(n, 0)), sizeof(uint8_t));
	Transform_Data_Structure_Temp(n, e, coefficientsMatrix, Eqs);
	printf("Solve equations by exfes ...\n");
	int resultCode = exfes(m, n, e, Mask[1], Mask[0], coefficientsMatrix, otherNodeReady, &solutionHigh, &solutionLow);
	printf("    exfes resultCode = %d\n", resultCode);

	// Report obtained solutions in uint256 format.
	Report_Solution(solutionHigh, solutionLow);

	// Check reported solution.
	Check_Solution(n, e, genSolutionHigh, genSolutionLow, Eqs);

	Free_Equations(e, Eqs);
	free(coefficientsMatrix);

	return 0;

}