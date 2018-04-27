#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <iostream>

#include "uint128_256.h"

extern "C" {
	#include "exfes.h"
}

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
	for (int i=0; i<n; i++) {
		Sol[0] = Sol[0] << 1;
		Sol[0] = Sol[0] ^ (rand() & 1);
	}
}

// Define a function to fix the costant term of equations so that they fit the generated solution.
int Evaluate_Solution (int n, uint64_t *Sol, int **Eqs) {
	int val = 0;
	val ^= Eqs[0][0];
	for (int i=0; i<n; i++)
		val ^= Eqs[1][i] & ((Sol[0] >> i) & 1);
	int offset = 0;
	for (int i=0; i<n-1; i++) {
		for (int j=i+1; j<n; j++)
			val ^= Eqs[2][offset+j-(i+1)] & ((Sol[0] >> i) & 1) & ((Sol[0] >> j) & 1);
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
	for (uint64_t i=0; i<maxsol; i++) {
		if (SolArray[i][0] == SolGlobal[0] && SolArray[i][1] == 0 && SolArray[i][2] == 0 && SolArray[i][3] == 0)
			printf("    Sol = %016"PRIx64"(O)\n", SolArray[i][0]);
		else
			printf("    Sol = %016"PRIx64"(X)\n", SolArray[i][0]);
	}
	return 0;
}

int main (int argc, char **argv) {

	int m = 0; // M variables will be fixed by exfes before calling libFES.
	int n = 24; // Test exfes with n variables.
	int e = 20; // Test exfes with e equations.

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
	uint64_t Mask[2] = {0xa740c932887dc0a3, 0x572afe39a8bc39d3}; // The solver starts searching from the value of mask.
	uint64_t maxsol = 16; // The solver only returns maxsol solutions. Other solutions will be discarded.
	uint64_t **SolArray = (uint64_t **)calloc(maxsol, sizeof(uint64_t *)); // Create an array for exfes to store solutions.
	Initialize_Array(maxsol, SolArray); // Set all array elements to zero.
	
	// Generate a solution randomly.
	printf("Generate a solution randomly ...\n");
	uint64_t *Sol = (uint64_t *)malloc(sizeof(uint64_t));
	Generate_Solution(n, Sol);
	printf("    Sol = ""%016"PRIx64"\n", Sol[0]);
	SolGlobal = Sol;

	// Generate equations randomly.
	printf("Generate equations randomly ...\n");
	Generate_Equation(n, e, Sol, Eqs);
	//Print_Equation(n, e, Eqs);

	// Solve equations by exfes.
	printf("Solve equations by exfes ...\n");
	exfes(m, n, e, Mask, maxsol, Eqs, SolArray);

	// Report obtained solutions in uint256 format.
	Report_Solution(maxsol, SolArray);

	return 0;

}
