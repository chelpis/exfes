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
        (void) solutionHigh;
        (void) solutionLow;
	for (int i=0; i<e*(B(n,2)+B(n,1)+B(n,0)); i++)
		coefficientsMatrix[i] = rand() & 1;
	// int val;
	// for (int i=0; i<e; i++) {
	// 	val = Evaluate_Solution(n, solutionHigh, solutionLow, i, coefficientsMatrix);
	// 	coefficientsMatrix[(i+1)*(B(n,2)+B(n,1)+B(n,0))-1] ^= val;
	// }
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

int main (int argc, char **argv) {

        (void) argc;
        (void) argv;

	uint32_t mArray[4] = {0, 1, 2, 3};
	uint32_t nArray[12] = {17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28};
	uint32_t m, n, e = 0;
	uint8_t i, j = 0;

	uint64_t startPointHigh = 0;
	uint64_t startPointLow = 0;
	uint64_t solutionHigh = 0;
	uint64_t solutionLow = 0;

	uint8_t *coefficientsMatrix = 0;

	bool checkSolution = false;
	uint32_t successCount = 0;
	uint32_t failCount = 0;

	for (i = 0; i < 4; i += 1) {
		for (j = 0; j < 12; j += 1) {
			m = mArray[i];
			n = nArray[j];
			e = n;

			Generate_Solution(n, &startPointHigh, &startPointLow);
			coefficientsMatrix = (uint8_t *)calloc(e * (B(n, 2) + B(n, 1) + B(n, 0)), sizeof(uint8_t));
			Generate_Equation(n, e, 0, 0, coefficientsMatrix);

			int resultCode = exfes(m, n, e, startPointHigh, startPointLow, coefficientsMatrix, otherNodeReady, &solutionHigh, &solutionLow);

			if (resultCode == 0) {
				checkSolution = Check_Solution(n, e, solutionHigh, solutionLow, coefficientsMatrix);
				if (checkSolution) {
					successCount += 1;
				} else {
					failCount += 1;
          printf("Failed at test_%u_%u_%u!\n", m, n, e);
				}
			}
      else if (resultCode == -1) {
        successCount += 1;
      }
      else {
				failCount += 1;
        printf("Failed at test_%u_%u_%u!\n", m, n, e);
			}

			free(coefficientsMatrix);

		}
	}

	printf("\nsuccess = %u\n", successCount);
	printf("fail = %u\n", failCount);

	return 0;

}
