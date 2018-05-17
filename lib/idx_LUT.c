/*****************************************************************************
 *       Copyright (C) 2012 Ralf Zimmermann <Ralf.Zimmermann@rub.de>
 *       Copyright (C) 2012 Charles Bouillaguet <charles.bouillaguet@gmail.com>

 *
 * Distributed under the terms of the GNU General Public License (GPL)
 * as published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *                 http://www.gnu.org/licenses/
 *****************************************************************************/
#include <stdint.h>
#include <stdlib.h>

#include "idx_LUT.h"

#include "binomials.h"
#include "my_memory.h"

typedef struct {
  LUT_int_t ** LUT;
  int n;
  int d;
} nonconst_lut_t;



uint64_t n_monomials(int n, int d) {
  uint64_t result = 0;
  for(int i=0; i<=d; i++)
    result += binomials[n][i];
  return result;
}


const idx_lut_t *init_deginvlex_LUT(int n, int d) {
  const int errors = n - 1 - d; // possible errors to correct

  LUT_int_t **LUT = (LUT_int_t**)mycalloc(d, sizeof(LUT_int_t*), 10);
  if (LUT == NULL)
	  return NULL;
  int count_i = -1;
  for (int i = 0; i < d; i++) {
    LUT[i] = (LUT_int_t*)mycalloc(n, sizeof(LUT_int_t), 10);
    if (LUT[i] == NULL) {
      count_i = i;
		break;
	}
  }
  count_i -= 1;
  while (count_i >= 0) {
    free(LUT[count_i]);
	count_i -= 1;
  }
  if (count_i == -1) {
	  free(LUT);
	  return NULL;
  }

  // save pos[0] = 1
  LUT[0][0] = 1;

  // generate the first step values
  // this fixes the highest possible value correctly
  for (int i = 1; i < n; i++)
    LUT[0][i] = (LUT[0][i-1] << 1) - binomials[i-1][d];


  // generate remaining steps until depth-1
  // this corrects the offset for the highest possible value per step
  for (int step = 1; step < d-1; step++) {
    // copy the values which are not modified
    for (int i = 0; i < n - step - errors; i++)
      LUT[step][i] = LUT[step-1][i];

    // now correct the values
    // the first offset fix is (d - step) choose (d - step) = 1
    LUT_int_t correction = 1;
    for (int i=n - step - errors; i < n - step; i++) {
      LUT[step][i] = LUT[step-1][i] - correction;
      correction += binomials[i][d - step];
    }
  }

  // last step uses (offset of the last number) + 1
  for (int i = 0; i < n-(d-1); i++)
    LUT[d-1][i] = i+1;

  nonconst_lut_t *idx_lut = (nonconst_lut_t *)mycalloc(1, sizeof(idx_lut_t), 10);
  if (idx_lut == NULL)
	  return NULL;
  idx_lut->n = n;
  idx_lut->d = d;
  idx_lut->LUT = LUT;

  return (idx_lut_t *) idx_lut;
}



LUT_int_t set2int(const idx_lut_t *table, int *set) {
  const int d = table->d;
  LUT_t LUT = table->LUT;
  LUT_int_t value = 0;

  for (int i = 0; i < d; i++) {
    const int j = i;
    if (set[j] != -1)
      value += LUT[i][set[j]];
    else
      break;
  }
  return value;
}


void int2set(const idx_lut_t *table, LUT_int_t index, int *set) {
  const int d = table->d;
  const int n = table->n;
  LUT_t LUT = table->LUT;

  for(int i=0; i<d; i++)
    set[i] = -1;

  int i = 0;
  while (index > 1) {
    int j = n-1-i;
    while (j >= 0 && LUT[i][j] > index) j--;
    set[i] = j;
    index -= LUT[i][j];
    i++;
  }

  if (index == 1) set[i] = 0;
}


LUT_int_t idx_convert(idx_lut_t *table_from, idx_lut_t *table_to, LUT_int_t i) {
  int n = table_from->n;
  int set[n];
  for(int j=0; j<n; j++)
    set[j] = -1;

  int2set(table_from, i, set);
  return set2int(table_to, set);
}


// pretty self-explanatory
void free_LUT(const idx_lut_t *table) {
  if (table != NULL) {
    for (int i = 0; i < table->d; i++)
      if (table->LUT[i] != NULL)
	free((void *) table->LUT[i]);
    free((void *) table->LUT);
    free((void *) table);
  }
}


