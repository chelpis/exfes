#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "fes.h"
#include "idx_LUT.h"

#define THRESHOLD 9

// this ought to be optimized, unrolled, etc.
void BFS(pck_vector_t *A, int start, int n) {

  for(int i=0; i<n; i++) {
    
    int Sz = 1 << i;
    int Pos = start;

    while(Pos < start + (1 << n)) {
      for(int j=0; j<Sz; j++)
	A[Pos + Sz + j] ^= A[Pos + j];
      Pos += 2*Sz;
    }
  }
}


void hybrid_DFS_BFS(pck_vector_t *A, int from, int to) {
  if (from >= to-1) return;
 
  int center = (to+from)/2;
  int half_length = (to-from)/2;

  if (half_length == (1 << (THRESHOLD-1)))
    BFS(A, from, THRESHOLD);
  else {
    hybrid_DFS_BFS(A, from, center);
    hybrid_DFS_BFS(A, center, to);
    for(int i=0; i<half_length; i++)
      A[center + i] ^= A[from + i];
  }
}


void moebius_transform(int n, pck_vector_t F[], solution_callback_t callback, void* callback_state) {

  // compute the moebius transform
  hybrid_DFS_BFS(F, 0, (1ll << n));

  // check for solutions [could/should be integrated into BFS, at least to allow early abort, and to improve cache usage ?]
  uint64_t size = 1ll << n;
  for(uint64_t i=0; i<size; i++)
    if (F[i] == 0)
      if ((*callback)(callback_state, 1, &i))
	return;
}
