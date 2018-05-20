#include <stdint.h>
#include <stdlib.h>

#include "idx_LUT.h"

#include "binomials.h"
#include "my_memory.h"

typedef struct {
    LUT_int_t **LUT;
    int n;
    int d;
} nonconst_lut_t;

uint64_t n_monomials(int n, int d)
{
    uint64_t result = 0;
    for (int i = 0; i <= d; i++) {
        result += binomials[n][i];
    }
    return result;
}

const idx_lut_t *init_deginvlex_LUT(int n, int d)
{
    const int errors = n - 1 - d;  // possible errors to correct

    LUT_int_t **LUT = (LUT_int_t **)exfes_calloc(d, sizeof(LUT_int_t *), 10);
    if (!LUT) {
        return 0;
    }

    int count_i = -1;
    for (int i = 0; i < d; i++) {
        LUT[i] = (LUT_int_t *)exfes_calloc(n, sizeof(LUT_int_t), 10);
        if (!LUT[i]) {
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
        return 0;
    }

    // save pos[0] = 1
    LUT[0][0] = 1;

    // generate the first step values
    // this fixes the highest possible value correctly
    for (int i = 1; i < n; i++) {
        LUT[0][i] = (LUT[0][i - 1] << 1) - binomials[i - 1][d];
    }

    // generate remaining steps until depth-1
    // this corrects the offset for the highest possible value per step
    for (int step = 1; step < d - 1; step++) {
        // copy the values which are not modified
        for (int i = 0; i < n - step - errors; i++) {
            LUT[step][i] = LUT[step - 1][i];
        }

        // now correct the values
        // the first offset fix is (d - step) choose (d - step) = 1
        LUT_int_t correction = 1;
        for (int i = n - step - errors; i < n - step; i++) {
            LUT[step][i] = LUT[step - 1][i] - correction;
            correction += binomials[i][d - step];
        }
    }

    // last step uses (offset of the last number) + 1
    for (int i = 0; i < n - (d - 1); i++) {
        LUT[d - 1][i] = i + 1;
    }

    nonconst_lut_t *idx_lut = (nonconst_lut_t *)exfes_calloc(1, sizeof(idx_lut_t), 10);
    if (!idx_lut) {
        for (int i = d - 1; i >= 0; i--) {
            free(LUT[i]);
        }
        free(LUT);
        return 0;
    }

    idx_lut->n = n;
    idx_lut->d = d;
    idx_lut->LUT = LUT;

    // Note by JS:  WTF?  Is this a safe cast well-defined by the C11 standard?
    // (struct { uint32_t **; int; int; } *)
    // ->
    // (struct { uint32_t const *const *const; const int; const int; } const *)
    return (idx_lut_t *)idx_lut;
}

LUT_int_t set2int(const idx_lut_t *table, int *set)
{
    const int d = table->d;
    LUT_t LUT = table->LUT;
    LUT_int_t value = 0;

    for (int i = 0; i < d; i++) {
        const int j = i;
        if (set[j] != -1) {
            value += LUT[i][set[j]];
        } else {
            break;
        }
    }
    return value;
}

// pretty self-explanatory
void free_LUT(const idx_lut_t *table)
{
    if (table) {
        for (int i = 0; i < table->d; i++) {
            if (table->LUT[i]) {
                free((void *)table->LUT[i]);
            }
        }
        free((void *)table->LUT);
        free((void *)table);
    }
}
