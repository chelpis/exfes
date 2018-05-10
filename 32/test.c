#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>

#include "exfes.h"

uint64_t *SolGlobal;

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
int S (uint64_t *Sol, int index) {
	if (index < 64)
		return (Sol[0] >> index) & 1;
	else
		return (Sol[1] >> (index - 64)) & 1;
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
void Initialize_Equation (int n, int e, int ***Eqs) {
	for (int i=0; i<e; i++) {
		Eqs[i] = (int **)calloc(3, sizeof(int *));
		for (int j=0; j<3; j++) {
			Eqs[i][j] = (int *)malloc(B(n, j) * sizeof(int));
			for (int k=0; k<B(n, j); k++)
				Eqs[i][j][k] = 0;
		}
	}
}

// Define a function set all array elements to zero.
void Initialize_Array (uint64_t maxsol, uint64_t **SolArray) {
	for (uint64_t i=0; i<maxsol; i++) {
		SolArray[i] = (uint64_t *)malloc(4 * sizeof(uint64_t));
		for (int j=0; j<4; j++)
			SolArray[i][j] = 0;
	}
}

// Transform coefficientsMatrix into the structure required by exfes.
void Transform_Data_Structure (int n, int e, int *coefficientsMatrix, int ***Eqs) {
	uint64_t offset = 0;
	for (int i=0; i<e; i++) {
		for (int j=0; j<B(n, 2); j++)
			Eqs[i][2][j] = coefficientsMatrix[offset+j];
		offset += B(n, 2);
		for (int j=0; j<n; j++)
			Eqs[i][1][j] = coefficientsMatrix[offset+j];
		offset += n;
		Eqs[i][0][0] = coefficientsMatrix[offset];
		offset += 1;
	}
}

// Define a function to generate a solution randomly.
void Generate_Solution (int n, uint64_t *Sol) {
	Sol[0] = 0;
	Sol[1] = 0;
	if (n <= 64)
		for (int i=0; i<n; i++) {
			Sol[0] = Sol[0] << 1;
			Sol[0] = Sol[0] ^ (rand() & 1);
		}
	else {
		for (int i=0; i<64; i++) {
			Sol[0] = Sol[0] << 1;
			Sol[0] = Sol[0] ^ (rand() & 1);
		}
		for (int i=0; i<n-64; i++) {
			Sol[1] = Sol[1] << 1;
			Sol[1] = Sol[1] ^ (rand() & 1);
		}
	}
}

// Define a function to fix the costant term of equations so that they fit the generated solution.
int Evaluate_Solution (int n, uint64_t *Sol, int **Eqs) {
	int val = 0;
	val ^= Eqs[0][0];
	for (int i=0; i<n; i++)
		val ^= Eqs[1][i] & S(Sol, i);
	int offset = 0;
	for (int i=0; i<n-1; i++) {
		for (int j=i+1; j<n; j++)
			val ^= Eqs[2][offset+j-(i+1)] & S(Sol, i) & S(Sol, j);
		offset += n - (i + 1);
	}
	return val;
}

// Define a function to randomly generate equations that fit the generated solution.
void Generate_Equation (int n, int e, uint64_t *Sol, int ***Eqs) {
	for (int i=0; i<e; i++)
		for (int j=0; j<3; j++)
			for (int k=0; k<B(n, j); k++)
				Eqs[i][j][k] = rand() & 1;
	int val;
	for (int i=0; i<e; i++) {
		val = Evaluate_Solution(n, Sol, Eqs[i]);
		Eqs[i][0][0] ^= val;
	}
}

// Define a function to print solutions obtained from exfes.
int Report_Solution (uint64_t maxsol, uint64_t **SolArray) {
	for (uint64_t i=0; i<maxsol; i++)
		printf("    Solution = %016"PRIx64"%016"PRIx64"\n", SolArray[i][1], SolArray[i][0]);
	return 0;
}

// Define a function to check solution.
void Check_Solution (int n, int e, uint64_t *Sol, int ***Eqs) {
	int count = 0;
	for (int i=0; i<e; i++) {
		int val = Evaluate_Solution(n, Sol, Eqs[i]);
		if (val == 0)
			count += 1;
	}
	printf("    Fitting Equations = %d\n", count);
}

int main (int argc, char **argv) {

	int m = 17; // M variables will be fixed by exfes before calling libFES.
	int n = 80; // Test exfes with n variables.
	int e = 72; // Test exfes with e equations.

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

	int ***Eqs = (int ***)calloc(e, sizeof(int **)); // Create an array for saving coefficients for exfes.
	Initialize_Equation(n, e, Eqs); // Set all array elements to zero.
	uint64_t Mask[2] = {0xe0d3cf665fad7012, 0x000000000000e5eb}; // The solver starts searching from the value of mask.
	uint64_t maxsol = 1; // The solver only returns maxsol solutions. Other solutions will be discarded.
	uint64_t **SolArray = (uint64_t **)calloc(maxsol, sizeof(uint64_t *)); // Create an array for exfes to store solutions.
	Initialize_Array(maxsol, SolArray); // Set all array elements to zero.

	// Generate a solution randomly.
	printf("Generate a solution randomly ...\n");
	uint64_t *Sol = (uint64_t *)malloc(sizeof(uint64_t));
	Generate_Solution(n, Sol);
	printf("    Solution = ""%016"PRIx64"%016"PRIx64"\n", Sol[1], Sol[0]);
	SolGlobal = Sol;

	// Generate equations randomly.
	//printf("Generate equations randomly ...\n");
	Generate_Equation(n, e, Sol, Eqs);
	//Print_Equation(n, e, Eqs);

	// Check generated equations.
	Check_Solution(n, e, Sol, Eqs);

	// Solve equations by exfes.
	printf("Solve equations by exfes ...\n");
	exfes(m, n, e, Mask, maxsol, Eqs, SolArray);

	// Report obtained solutions in uint256 format.
	Report_Solution(maxsol, SolArray);

	// Check reported solution.
	Check_Solution(n, e, SolArray[0], Eqs);

	return 0;

}
