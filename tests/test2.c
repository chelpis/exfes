#define _POSIX_C_SOURCE 200809L

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

// Define a function to find the index-th bit of solution.
int S (uint64_t solutionHigh, uint64_t solutionLow, int index) {
	if (index < 64)
		return (solutionLow >> index) & 1;
	else
		return (solutionHigh >> (index - 64)) & 1;
}

// Define a function to check if other nodes have found solution or not.
bool otherNodeReady(void) {
	return 0;
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
int Evaluate_Solution (int n, uint64_t solutionHigh, uint64_t solutionLow, int index, uint8_t *coefficientsMatrix) {
	int val = 0;
	int offset = index * (B(n, 2) + B(n, 1) + B(n, 0));
	for (int i=0; i<n-1; i++) {
		for (int j=i+1; j<n; j++)
			val ^= coefficientsMatrix[offset+j-(i+1)] & S(solutionHigh, solutionLow, i) & S(solutionHigh, solutionLow, j);
		offset += n - (i + 1);
	}
	for (int i=0; i<n; i++)
		val ^= coefficientsMatrix[offset+i] & S(solutionHigh, solutionLow, i);
	offset += n;
	val ^= coefficientsMatrix[offset];
	return val;
}

// Define a function to randomly generate equations that fit the generated solution.
void Generate_Equation (int n, int e, uint64_t solutionHigh, uint64_t solutionLow, uint8_t *coefficientsMatrix) {
	for (int i=0; i<e*(B(n,2)+B(n,1)+B(n,0)); i++)
		coefficientsMatrix[i] = rand() & 1;
	int val;
	for (int i=0; i<e; i++) {
		val = Evaluate_Solution(n, solutionHigh, solutionLow, i, coefficientsMatrix);
		coefficientsMatrix[(i+1)*(B(n,2)+B(n,1)+B(n,0))-1] ^= val;
	}
}

// Define a function to print solutions obtained from exfes.
void Report_Solution (uint64_t solutionHigh, uint64_t solutionLow) {
	printf("    Solution = %016" PRIx64 "%016" PRIx64 "\n", solutionHigh, solutionLow);
}

// Define a function to check solution.
bool Check_Solution (int n, int e, uint64_t solutionHigh, uint64_t solutionLow, uint8_t *coefficientsMatrix) {
	int count = 0;
	for (int i=0; i<e; i++) {
		int val = Evaluate_Solution(n, solutionHigh, solutionLow, i, coefficientsMatrix);
		if (val == 0)
			count += 1;
	}
	if (count == e)
		return 1;
	else
		return 0;
}

// Encode Function
uint64_t Encode (uint64_t word) {
	uint64_t codeword = word ^ (word >> 1);
	return codeword;
}

// Decode Function
uint64_t Decode (uint64_t codeword) {
	uint64_t word = 0;
	for (int i=0; i<64; i++) {
		word ^= codeword;
		codeword = codeword >> 1;
	}
	return word;
}

// Define a function to generate special startPoint.
void Generate_Special_StartPoint (int m, int n, uint64_t solutionHigh, uint64_t solutionLow, int hardness, uint64_t *startPointHigh, uint64_t *startPointLow) {
	startPointHigh[0] = solutionHigh;
	startPointLow[0] = solutionLow;
	if ((hardness >= 64 - m) || (hardness > n) || (hardness < 0)) {
		printf("Invalid value of hardness!\n");
		return;
	}
	uint64_t tmp = startPointLow[0] % ((uint64_t)1 << m);
	uint64_t word = Decode(startPointLow[0] >> m);
	word = (word >> hardness) << hardness;
	startPointLow[0] = (Encode(word) << m) ^ tmp;
	printf("    StartPnt = %016" PRIx64 "%016" PRIx64 "\n", startPointHigh[0], startPointLow[0]);
}

int main (int argc, char **argv) {
        (void) argc;
        (void) argv;

	uint32_t mArray[4] = {0, 1, 9, 17};
	uint32_t nArray[4] = {56, 64, 72, 80};
	uint32_t m, n, e = 0;
	uint8_t j = 0;

	uint64_t startPointHigh = 0;
	uint64_t startPointLow = 0;
	uint64_t genSolutionHigh = 0;
	uint64_t genSolutionLow = 0;
	uint64_t solutionHigh = 0;
	uint64_t solutionLow = 0;

	uint8_t *coefficientsMatrix = 0;

	bool checkSolution = false;
	uint32_t successCount = 0;
	uint32_t failCount = 0;
	struct timespec last_spec, now_spec;
	float totalseconds = 0.0, miliseconds = 0.0;

	for (j = 0; j < 4; ) {
		clock_gettime(CLOCK_MONOTONIC, &last_spec);
		m = mArray[j];
		n = nArray[j];
		e = n - 8;

		Generate_Solution(n, &genSolutionHigh, &genSolutionLow);
		coefficientsMatrix = (uint8_t *)calloc(e * (B(n, 2) + B(n, 1) + B(n, 0)), sizeof(uint8_t));
		Generate_Equation(n, e, genSolutionHigh, genSolutionLow, coefficientsMatrix);
		Generate_Special_StartPoint(m, n, genSolutionHigh, genSolutionLow, 32, &startPointHigh, &startPointLow);

		printf("Solve equations by exfes ...\n");
		printf("numFixedVariables, numVariables, numEquations = %u, %u, %u\n", m, n, e);
		int resultCode = exfes(m, n, e, startPointHigh, startPointLow, coefficientsMatrix, otherNodeReady, &solutionHigh, &solutionLow);

		if (resultCode == 0) {
			printf("    Found One Solution (resultCode = 0)\n");
			Report_Solution(solutionHigh, solutionLow);
			checkSolution = Check_Solution(n, e, solutionHigh, solutionLow, coefficientsMatrix);
			printf("    Check_Solution = %d\n", checkSolution);
			if (checkSolution) {
				successCount += 1;
			} else {
				printf("    Wrong Solution!!\n");
				failCount += 1;
			}
		} else {
			failCount += 1;
			if (resultCode == -1)
				printf("    No Possible Solution (resultCode = -1)\n");
			else if (resultCode == -2)
				printf("    Interrupted By Other Nodes (resultCode = -2)\n");
			else if (resultCode == -3)
				printf("    Invalid Parameters (resultCode = -3)\n");
			else if (resultCode == -4)
				printf("    calloc / alloc Failure (resultCode = -4)\n");
			else
				printf("    Undefined Results\n");
		}

		free(coefficientsMatrix);

		clock_gettime(CLOCK_MONOTONIC, &now_spec);
		miliseconds = ((now_spec.tv_nsec - last_spec.tv_nsec) / 1.0e6) + (now_spec.tv_sec - last_spec.tv_sec) * 1000;
		totalseconds += miliseconds;
		printf("    Elapsed time(ms) = %.3f / %.3f\n\n", miliseconds, totalseconds);

		if (totalseconds >= 9000000.0) {
			totalseconds = 0.0;
			j += 1;
		}
	}

	printf("\nsuccess = %u\n", successCount);
	printf("fail = %u\n", failCount);

	return 0;

}

