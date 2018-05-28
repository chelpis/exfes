#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define idx_0(LUT) (0)
#define idx_1(LUT, i0) ((LUT)[0][i0])
#define idx_2(LUT, i1, i0) (idx_1(LUT, i0) + (LUT)[1][i1])
#define min(x, y) (((x) > (y)) ? (y) : (x))
#define unlikely(x) __builtin_expect(!!(x), 0)
#define PUSH_SOLUTION(current_solution)                                                             \
    {                                                                                               \
        packVectors_of_solution[current_solution_index] = current_solution;                         \
        current_solution_index++;                                                                   \
        if (current_solution_index == 1) {                                                          \
            if (testSolution(wrapper_state_ptr, current_solution_index, packVectors_of_solution)) { \
                return;                                                                             \
            }                                                                                       \
            current_solution_index = 0;                                                             \
        }                                                                                           \
    }
#define CHECK_SOLUTIONS()                                                \
    {                                                                    \
        for (uint64_t i = 0; i < n_solutions_found; i++) {               \
            if (solution_buffer[i].mask & 0xffff) {                      \
                PUSH_SOLUTION(encodeToGray(solution_buffer[i].int_idx)); \
            }                                                            \
        }                                                                \
        n_solutions_found = 0;                                           \
    }
#define STEP_0(i)                                             \
    {                                                         \
        if (unlikely(F[0] == 0)) {                            \
            solution_buffer[n_solutions_found].int_idx = i;   \
            solution_buffer[n_solutions_found].mask = 0x000f; \
            n_solutions_found++;                              \
        }                                                     \
    }
#define STEP_1(a, i)                                          \
    {                                                         \
        F[0] ^= F[a];                                         \
        if (unlikely(F[0] == 0)) {                            \
            solution_buffer[n_solutions_found].int_idx = i;   \
            solution_buffer[n_solutions_found].mask = 0x000f; \
            n_solutions_found++;                              \
        }                                                     \
    }
#define STEP_2(a, b, i)                                       \
    {                                                         \
        F[a] ^= F[b];                                         \
        F[0] ^= F[a];                                         \
        if (unlikely(F[0] == 0)) {                            \
            solution_buffer[n_solutions_found].int_idx = i;   \
            solution_buffer[n_solutions_found].mask = 0x000f; \
            n_solutions_found++;                              \
        }                                                     \
    }

typedef uint32_t TableInteger;
typedef TableInteger **Table;
typedef struct {
    Table LUT;
    int n;
} TableWithSize;
typedef int *Vector;
typedef uint32_t PackedVectors;
typedef struct {
    int mcopy;
    int ncopy;
    uint64_t solm;
    uint64_t SolCount;
    uint64_t startPointHigh;
    uint64_t startPointLow;
    uint64_t *solutionHigh;
    uint64_t *solutionLow;
    bool (*shouldAbortNow)(void);
} Settings;
typedef struct {
    int n;
    int n_batches;
    PackedVectors **G;
    TableWithSize *testing_LUT;
    Settings *exfes_ctx_ptr;
} States;
typedef struct {
    uint64_t int_idx;
    uint32_t mask;
} Solution;

static const uint32_t binomials[127][4] = { { 1, 0, 0, 1 }, { 1, 1, 0, 2 }, { 1, 2, 1, 4 }, { 1, 3, 3, 7 }, { 1, 4, 6, 11 }, { 1, 5, 10, 16 }, { 1, 6, 15, 22 }, { 1, 7, 21, 29 }, { 1, 8, 28, 37 }, { 1, 9, 36, 46 }, { 1, 10, 45, 56 }, { 1, 11, 55, 67 }, { 1, 12, 66, 79 }, { 1, 13, 78, 92 }, { 1, 14, 91, 106 }, { 1, 15, 105, 121 }, { 1, 16, 120, 137 }, { 1, 17, 136, 154 }, { 1, 18, 153, 172 }, { 1, 19, 171, 191 }, { 1, 20, 190, 211 }, { 1, 21, 210, 232 }, { 1, 22, 231, 254 }, { 1, 23, 253, 277 }, { 1, 24, 276, 301 }, { 1, 25, 300, 326 }, { 1, 26, 325, 352 }, { 1, 27, 351, 379 }, { 1, 28, 378, 407 }, { 1, 29, 406, 436 }, { 1, 30, 435, 466 }, { 1, 31, 465, 497 }, { 1, 32, 496, 529 }, { 1, 33, 528, 562 }, { 1, 34, 561, 596 }, { 1, 35, 595, 631 }, { 1, 36, 630, 667 }, { 1, 37, 666, 704 }, { 1, 38, 703, 742 }, { 1, 39, 741, 781 }, { 1, 40, 780, 821 }, { 1, 41, 820, 862 }, { 1, 42, 861, 904 }, { 1, 43, 903, 947 }, { 1, 44, 946, 991 }, { 1, 45, 990, 1036 }, { 1, 46, 1035, 1082 }, { 1, 47, 1081, 1129 }, { 1, 48, 1128, 1177 }, { 1, 49, 1176, 1226 }, { 1, 50, 1225, 1276 }, { 1, 51, 1275, 1327 }, { 1, 52, 1326, 1379 }, { 1, 53, 1378, 1432 }, { 1, 54, 1431, 1486 }, { 1, 55, 1485, 1541 }, { 1, 56, 1540, 1597 }, { 1, 57, 1596, 1654 }, { 1, 58, 1653, 1712 }, { 1, 59, 1711, 1771 }, { 1, 60, 1770, 1831 }, { 1, 61, 1830, 1892 }, { 1, 62, 1891, 1954 }, { 1, 63, 1953, 2017 }, { 1, 64, 2016, 2081 }, { 1, 65, 2080, 2146 }, { 1, 66, 2145, 2212 }, { 1, 67, 2211, 2279 }, { 1, 68, 2278, 2347 }, { 1, 69, 2346, 2416 }, { 1, 70, 2415, 2486 }, { 1, 71, 2485, 2557 }, { 1, 72, 2556, 2629 }, { 1, 73, 2628, 2702 }, { 1, 74, 2701, 2776 }, { 1, 75, 2775, 2851 }, { 1, 76, 2850, 2927 }, { 1, 77, 2926, 3004 }, { 1, 78, 3003, 3082 }, { 1, 79, 3081, 3161 }, { 1, 80, 3160, 3241 }, { 1, 81, 3240, 3322 }, { 1, 82, 3321, 3404 }, { 1, 83, 3403, 3487 }, { 1, 84, 3486, 3571 }, { 1, 85, 3570, 3656 }, { 1, 86, 3655, 3742 }, { 1, 87, 3741, 3829 }, { 1, 88, 3828, 3917 }, { 1, 89, 3916, 4006 }, { 1, 90, 4005, 4096 }, { 1, 91, 4095, 4187 }, { 1, 92, 4186, 4279 }, { 1, 93, 4278, 4372 }, { 1, 94, 4371, 4466 }, { 1, 95, 4465, 4561 }, { 1, 96, 4560, 4657 }, { 1, 97, 4656, 4754 }, { 1, 98, 4753, 4852 }, { 1, 99, 4851, 4951 }, { 1, 100, 4950, 5051 }, { 1, 101, 5050, 5152 }, { 1, 102, 5151, 5254 }, { 1, 103, 5253, 5357 }, { 1, 104, 5356, 5461 }, { 1, 105, 5460, 5566 }, { 1, 106, 5565, 5672 }, { 1, 107, 5671, 5779 }, { 1, 108, 5778, 5887 }, { 1, 109, 5886, 5996 }, { 1, 110, 5995, 6106 }, { 1, 111, 6105, 6217 }, { 1, 112, 6216, 6329 }, { 1, 113, 6328, 6442 }, { 1, 114, 6441, 6556 }, { 1, 115, 6555, 6671 }, { 1, 116, 6670, 6787 }, { 1, 117, 6786, 6904 }, { 1, 118, 6903, 7022 }, { 1, 119, 7021, 7141 }, { 1, 120, 7140, 7261 }, { 1, 121, 7260, 7382 }, { 1, 122, 7381, 7504 }, { 1, 123, 7503, 7627 }, { 1, 124, 7626, 7751 }, { 1, 125, 7750, 7876 }, { 1, 126, 7875, 8002 } };

#ifdef __cplusplus
extern "C" {
#endif

int exfes(uint32_t numFixedVariables, uint32_t numVariables, uint32_t numEquations, uint64_t startPointHigh, uint64_t startPointLow, const uint8_t *coefficientsMatrix, bool (*shouldAbortNow)(void), uint64_t *solutionHigh, uint64_t *solutionLow);

#ifdef __cplusplus
}
#endif

static PackedVectors secondaryEvaluation(Table LUT, int n, PackedVectors *F, uint64_t i);
static void freeVector(Vector x);
static Vector initVector(int n_rows);
static PackedVectors packVectors(int n, const Vector v);
static uint64_t encodeToGray(uint64_t i);
static int selectOneBit(uint64_t startPointHigh, uint64_t startPointLow, int index);
static void freeEqs(int ***Eqs, int i, int j);
static int initEqs(int n, int e, int ****EqsPtr);
static void convert1DTo3D(int n, int e, const uint8_t *coefficientsMatrix, int ***Eqs);
static void mergeSolution(Settings *p, uint64_t count, uint64_t *Sol);
static TableWithSize *initTable(int n);
static TableInteger computeIntegerFromSet(const TableWithSize *table, int *set);
static void freeTable(const TableWithSize *tab);
static void *exfesCalloc(size_t num, size_t size, size_t max_num_retries);
static void computeNextSet(int n, int d, int *set);
static int convert3DToPackedVectors(const int n, int from, int to, int ***coeffs, TableWithSize *idx_LUT, PackedVectors *F);
static int testSolution(States *wrapper_state_ptr, uint64_t size, uint64_t *n_solutions);
static int fes(const int n, int n_eqs, int ***coeffs, Settings *exfes_ctx_ptr);
static void primarySearch(Table LUT, int n, PackedVectors *F, States *wrapper_state_ptr);

int exfes(uint32_t numFixedVariables, uint32_t numVariables, uint32_t numEquations, uint64_t startPointHigh, uint64_t startPointLow, const uint8_t *coefficientsMatrix, bool (*shouldAbortNow)(void), uint64_t *solutionHigh, uint64_t *solutionLow)
{
    int m = numFixedVariables;
    int n = numVariables;
    int e = numEquations;
    if (numVariables == 0 || numVariables >= 127) {
        return -3;
    }
    if (numVariables - numFixedVariables <= 1 || numVariables - numFixedVariables >= 64) {
        return -3;
    }
    if (numEquations <= 16 || numEquations >= 256) {
        return -3;
    }
    if (!coefficientsMatrix) {
        return -3;
    }
    if (!shouldAbortNow) {
        return -3;
    }
    if (!(solutionHigh && solutionLow)) {
        return -3;
    }
    Settings exfes_ctx;
    exfes_ctx.mcopy = m;
    exfes_ctx.ncopy = n;
    exfes_ctx.solm = 0;
    exfes_ctx.SolCount = 0;
    exfes_ctx.startPointHigh = startPointHigh;
    exfes_ctx.startPointLow = startPointLow;
    exfes_ctx.solutionHigh = solutionHigh;
    exfes_ctx.solutionLow = solutionLow;
    exfes_ctx.shouldAbortNow = shouldAbortNow;
    int ***Eqs;
    if (initEqs(n, e, &Eqs) != 0) {
        return -4;
    }
    convert1DTo3D(n, e, coefficientsMatrix, Eqs);
    for (int i = 0; i < e; i++) {
        for (int j = 0; j < n; j++) {
            Eqs[i][0][0] ^= Eqs[i][1][j] & selectOneBit(startPointHigh, startPointLow, j);
        }
        int offset = 0;
        for (int j = 0; j < n - 1; j++) {
            for (int k = j + 1; k < n; k++) {
                Eqs[i][0][0] ^= Eqs[i][2][offset] & selectOneBit(startPointHigh, startPointLow, j) & selectOneBit(startPointHigh, startPointLow, k);
                Eqs[i][1][j] ^= Eqs[i][2][offset] & selectOneBit(startPointHigh, startPointLow, k);
                Eqs[i][1][k] ^= Eqs[i][2][offset] & selectOneBit(startPointHigh, startPointLow, j);
                offset += 1;
            }
        }
    }
    int ***EqsCopy;
    if (initEqs(n, e, &EqsCopy) != 0) {
        freeEqs(Eqs, e, -1);
        return -4;
    }
    int p = n - m;
    int npartial;
    int fixvalue;
    for (exfes_ctx.solm = 0; exfes_ctx.solm < (uint64_t)1 << m; exfes_ctx.solm++) {
        npartial = n;
        for (int i = 0; i < e; i++) {
            for (int j = 0; j < 3; j++) {
                for (uint32_t k = 0; k < binomials[n][j]; k++) {
                    EqsCopy[i][j][k] = Eqs[i][j][k];
                }
            }
        }
        while (npartial != p) {
            fixvalue = (exfes_ctx.solm >> (n - npartial)) & 1;
            for (int i = 0; i < e; i++) {
                for (int j = 0; j < npartial - 1; j++) {
                    EqsCopy[i][1][j + 1] ^= EqsCopy[i][2][j] & fixvalue;
                }
                EqsCopy[i][0][0] ^= EqsCopy[i][1][0] & fixvalue;
                for (int j = 0; j < npartial - 1; j++) {
                    EqsCopy[i][1][j] = EqsCopy[i][1][j + 1];
                }
                for (uint32_t j = 0; j < binomials[npartial - 1][2]; j++) {
                    EqsCopy[i][2][j] = EqsCopy[i][2][j + npartial - 1];
                }
            }
            npartial -= 1;
        }
        if (fes(npartial, e, EqsCopy, &exfes_ctx) != 0) {
            freeEqs(EqsCopy, e, -1);
            freeEqs(Eqs, e, -1);
            return -4;
        }
        if (exfes_ctx.SolCount == 1) {
            break;
        }
        if (exfes_ctx.SolCount == 2) {
            break;
        }
    }
    freeEqs(EqsCopy, e, -1);
    freeEqs(Eqs, e, -1);
    if (exfes_ctx.SolCount == 1) {
        return 0;
    } else if (exfes_ctx.SolCount == 0) {
        return -1;
    } else {
        return -2;
    }
}

static PackedVectors secondaryEvaluation(Table LUT, int n, PackedVectors *F, uint64_t i)
{
    PackedVectors v[n];
    for (int k = 0; k < n; k++) {
        v[k] = 0;
        if (i & 0x0001) {
            v[k] = 0xffffffff;
        }
        i = (i >> 1ll);
    }
    PackedVectors y = F[0];
    for (int idx_0 = 0; idx_0 < n; idx_0++) {
        const PackedVectors v_0 = v[idx_0];
        y ^= F[idx_1(LUT, idx_0)] & v_0;
        for (int idx_1 = 0; idx_1 < idx_0; idx_1++) {
            const PackedVectors v_1 = v_0 & v[idx_1];
            y ^= F[idx_2(LUT, idx_1, idx_0)] & v_1;
        }
    }
    return y;
}

static void freeVector(Vector x)
{
    free(x);
}

static Vector initVector(int n_rows)
{
    return (Vector)exfesCalloc(n_rows, sizeof(int), 10);
}

static PackedVectors packVectors(int n, const Vector v)
{
    PackedVectors r = 0;

    for (int i = n - 1; i >= 0; i--) {
        r = r << 1;
        r |= v[i] & 1;
    }

    return r;
}

static uint64_t encodeToGray(uint64_t i)
{
    return i ^ (i >> 1);
}

static int selectOneBit(uint64_t startPointHigh, uint64_t startPointLow, int index)
{
    if (index < 64) {
        return (startPointLow >> index) & 1;
    } else {
        return (startPointHigh >> (index - 64)) & 1;
    }
}

static void freeEqs(int ***Eqs, int i, int j)
{
    if (j > -1) {
        j -= 1;
    } else {
        j = 2;
        i -= 1;
    }
    while (i >= 0) {
        if (j > -1) {
            free(Eqs[i][j]);
            j -= 1;
        } else {
            free(Eqs[i]);
            j = 2;
            i -= 1;
        }
    }
    free(Eqs);
}

static int initEqs(int n, int e, int ****EqsPtr)
{
    EqsPtr[0] = (int ***)exfesCalloc(e, sizeof(int **), 10);
    if (!EqsPtr[0]) {
        return -4;
    }
    for (int i = 0; i < e; i++) {
        EqsPtr[0][i] = (int **)exfesCalloc(3, sizeof(int *), 10);
        if (!EqsPtr[0][i]) {
            freeEqs(EqsPtr[0], i, -1);
            return -4;
        }
        for (int j = 0; j < 3; j++) {
            EqsPtr[0][i][j] = (int *)exfesCalloc(binomials[n][j], sizeof(int), 10);
            if (!EqsPtr[0][i][j]) {
                freeEqs(EqsPtr[0], i, j);
                return -4;
            }
        }
    }
    return 0;
}

static void convert1DTo3D(int n, int e, const uint8_t *coefficientsMatrix, int ***Eqs)
{
    uint32_t offset = 0;
    for (int i = 0; i < e; i++) {
        for (uint32_t j = 0; j < binomials[n][2]; j++) {
            Eqs[i][2][j] = (int)coefficientsMatrix[offset + j];
        }
        offset += binomials[n][2];
        for (int j = 0; j < n; j++) {
            Eqs[i][1][j] = (int)coefficientsMatrix[offset + j];
        }
        offset += n;
        Eqs[i][0][0] = (int)coefficientsMatrix[offset];
        offset += 1;
    }
}

static void mergeSolution(Settings *p, uint64_t count, uint64_t *Sol)
{
    const int mcopy = p->mcopy;
    const int ncopy = p->ncopy;
    const uint64_t solm = p->solm;
    const uint64_t startPointHigh = p->startPointHigh;
    const uint64_t startPointLow = p->startPointLow;
    uint64_t *solutionHigh = p->solutionHigh;
    uint64_t *solutionLow = p->solutionLow;
    solutionLow[0] = (Sol[count - 1] << mcopy) ^ solm;
    if (mcopy > 0) {
        solutionHigh[0] = Sol[count - 1] >> (64 - mcopy);
    }
    if (ncopy < 64) {
        solutionLow[0] ^= (startPointLow << (64 - ncopy)) >> (64 - ncopy);
    } else {
        solutionLow[0] ^= startPointLow;
        solutionHigh[0] ^= (startPointHigh << (128 - ncopy)) >> (128 - ncopy);
    }
    p->SolCount = 1;
}

static TableWithSize *initTable(int n)
{
    const int errors = n - 1 - 2;
    TableInteger **LUT = (TableInteger **)exfesCalloc(2, sizeof(TableInteger *), 10);
    if (!LUT) {
        return 0;
    }
    int count_i = -1;
    for (int i = 0; i < 2; i++) {
        LUT[i] = (TableInteger *)exfesCalloc(n, sizeof(TableInteger), 10);
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
    LUT[0][0] = 1;
    for (int i = 1; i < n; i++) {
        LUT[0][i] = (LUT[0][i - 1] << 1) - binomials[i - 1][2];
    }
    for (int step = 1; step < 2 - 1; step++) {
        for (int i = 0; i < n - step - errors; i++) {
            LUT[step][i] = LUT[step - 1][i];
        }
        TableInteger correction = 1;
        for (int i = n - step - errors; i < n - step; i++) {
            LUT[step][i] = LUT[step - 1][i] - correction;
            correction += binomials[i][2 - step];
        }
    }
    for (int i = 0; i < n - (2 - 1); i++) {
        LUT[2 - 1][i] = i + 1;
    }
    TableWithSize *idx_lut = (TableWithSize *)exfesCalloc(1, sizeof(TableWithSize), 10);
    if (!idx_lut) {
        for (int i = 2 - 1; i >= 0; i--) {
            free(LUT[i]);
        }
        free(LUT);
        return 0;
    }
    idx_lut->n = n;
    idx_lut->LUT = LUT;
    return (TableWithSize *)idx_lut;
}

static TableInteger computeIntegerFromSet(const TableWithSize *table, int *set)
{
    const int d = 2;
    Table LUT = table->LUT;
    TableInteger value = 0;

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

static void freeTable(const TableWithSize *table)
{
    if (table) {
        for (int i = 0; i < 2; i++) {
            if (table->LUT[i]) {
                free((void *)table->LUT[i]);
            }
        }
        free((void *)table->LUT);
        free((void *)table);
    }
}

static void *exfesCalloc(size_t num, size_t size, size_t max_num_retries)
{
    while (1) {
        void *p = calloc(num, size);
        if (p) {
            return p;
        }
        if (max_num_retries == 0) {
            return 0;
        }
        max_num_retries -= 1;
    }
}

static void computeNextSet(int n, int d, int *set)
{
    if (!d) {
        return;
    }
    set[0] += 1;
    if (set[0] == n) {
        computeNextSet(n - 1, d - 1, &set[1]);
        if (d > 0) {
            set[0] = set[1] + 1;
        }
    }
}

static int convert3DToPackedVectors(const int n, int from, int to, int ***coeffs, TableWithSize *idx_LUT, PackedVectors *F)
{
    Vector x = initVector(to - from);
    if (!x) {
        return -4;
    }
    int set[n];
    for (int j = 0; j < n; j++) {
        set[j] = -1;
    }
    for (int d = 0; d < 2 + 1; d++) {
        for (int j = 0; j < d; j++) {
            set[j] = d - 1 - j;
        }
        for (uint32_t m = 0; m < binomials[n][d]; m++) {
            for (int e = from; e < to; e++) {
                x[e - from] = coeffs[e][d][m];
            }
            F[computeIntegerFromSet(idx_LUT, set)] = packVectors(to - from, x);
            computeNextSet(n, n, &set[0]);
        }
    }
    freeVector(x);
    return 0;
}

static int testSolution(States *wrapper_state_ptr, uint64_t size, uint64_t *n_solutions)
{
    for (uint64_t i = 0; i < size; i++) {
        uint64_t current_solution = n_solutions[i];
        int is_correct = 1;
        int j = 0;
        while (is_correct && j < wrapper_state_ptr->n_batches) {
            if (secondaryEvaluation(wrapper_state_ptr->testing_LUT->LUT, wrapper_state_ptr->n, wrapper_state_ptr->G[j], current_solution) != 0) {
                is_correct = 0;
            }
            j++;
        }
        if (is_correct) {
            int num_correct_solutions = 1;
            uint64_t corrects_solutions[1];
            corrects_solutions[0] = current_solution;
            mergeSolution(wrapper_state_ptr->exfes_ctx_ptr, num_correct_solutions, corrects_solutions);
            return 1;
        }
    }
    return 0;
}

static int fes(const int n, int n_eqs, int ***coeffs, Settings *exfes_ctx_ptr)
{
    const uint64_t N = binomials[n][3];
    const int word_size = 16;
    TableWithSize *idx_LUT = initTable(n);
    if (!idx_LUT) {
        return -4;
    }
    PackedVectors *F = (PackedVectors *)exfesCalloc(1, N * sizeof(PackedVectors), 10);
    if (!F) {
        freeTable(idx_LUT);
        return -4;
    }
    if (convert3DToPackedVectors(n, 0, word_size, coeffs, idx_LUT, F) != 0) {
        free(F);
        freeTable(idx_LUT);
        return -4;
    }
    int n_batches = n_eqs / word_size;
    if (n_eqs % word_size) {
        n_batches++;
    }
    PackedVectors **G = (PackedVectors **)exfesCalloc(n_batches - 1, sizeof(PackedVectors *), 10);
    if (!G) {
        free(F);
        freeTable(idx_LUT);
        return -4;
    }
    int should_free_G_count = -1;
    for (int i = 1; i < n_batches; i++) {
        G[i - 1] = (PackedVectors *)exfesCalloc(N, sizeof(PackedVectors), 10);
        if (!G[i - 1]) {
            should_free_G_count = i - 1;
            break;
        }
        if (convert3DToPackedVectors(n, word_size * i, min(n_eqs, word_size * (i + 1)), coeffs, idx_LUT, G[i - 1]) != 0) {
            should_free_G_count = i;
            break;
        }
    }
    should_free_G_count -= 1;
    while (should_free_G_count >= 0) {
        free(G[should_free_G_count]);
        should_free_G_count -= 1;
    }
    if (should_free_G_count == -1) {
        free(G);
        free(F);
        freeTable(idx_LUT);
        return -4;
    }
    States wrapper_state;
    wrapper_state.n = n;
    wrapper_state.n_batches = n_batches - 1;
    wrapper_state.G = G;
    wrapper_state.testing_LUT = idx_LUT;
    wrapper_state.exfes_ctx_ptr = exfes_ctx_ptr;
    primarySearch(idx_LUT->LUT, n, F, &wrapper_state);
    for (int i = n_batches - 1; i >= 1; i--) {
        free(G[i - 1]);
    }
    free(G);
    free(F);
    freeTable(idx_LUT);
    return 0;
}

static void primarySearch(Table LUT, int n, PackedVectors *F, States *wrapper_state_ptr)
{
    Settings *ctx = wrapper_state_ptr->exfes_ctx_ptr;
    for (int i0 = 1; i0 < n; i0++) {
        if (i0 != 0) {
            F[idx_1(LUT, i0)] ^= F[idx_2(LUT, i0 - 1, i0)];
        }
    }
    uint64_t n_solutions_found = 0;
    uint64_t current_solution_index = 0;
    uint64_t packVectors_of_solution[1];
    Solution solution_buffer[516];
    const uint64_t weight_0_start = 0;
    STEP_0(0);
    for (int idx_0 = 0; idx_0 < n; idx_0++) {
        const uint64_t weight_1_start = weight_0_start + (1ULL << idx_0);
        STEP_1(idx_1(LUT, idx_0), weight_1_start);
        const uint64_t rolled_end = weight_1_start + (1ULL << min(9, idx_0));
        for (uint64_t i = 1 + weight_1_start; i < rolled_end; i++) {
            int pos = 0;
            uint64_t _i = i;
            while ((_i & 0x0001) == 0) {
                _i >>= 1;
                pos++;
            }
            const int k_1 = pos;
            _i >>= 1;
            pos++;
            while ((_i & 0x0001) == 0) {
                _i >>= 1;
                pos++;
            }
            const int k_2 = pos;
            STEP_2(idx_1(LUT, k_1), idx_2(LUT, k_1, k_2), i);
        }
        CHECK_SOLUTIONS();
        for (uint64_t j = 512; j < (1ull << idx_0); j += 512) {
            if (ctx->shouldAbortNow()) {
                ctx->SolCount = 2;
                return;
            }
            const uint64_t i = j + weight_1_start;
            int pos = 0;
            uint64_t _i = i;
            while ((_i & 0x0001) == 0) {
                _i >>= 1;
                pos++;
            }
            const int k_1 = pos;
            _i >>= 1;
            pos++;
            while ((_i & 0x0001) == 0) {
                _i >>= 1;
                pos++;
            }
            const int k_2 = pos;
            const int alpha = LUT[0][k_1];
            const int beta = LUT[1][k_1] + LUT[0][k_2];
            STEP_2(0 + alpha, 0 + beta, i + 0);
            STEP_2(1, 1 + alpha, i + 1);
            STEP_2(2, 2 + alpha, i + 2);
            STEP_2(1, 3, i + 3);
            STEP_2(4, 3 + alpha, i + 4);
            STEP_2(1, 5, i + 5);
            STEP_2(2, 6, i + 6);
            STEP_2(1, 3, i + 7);
            STEP_2(7, 4 + alpha, i + 8);
            STEP_2(1, 8, i + 9);
            STEP_2(2, 9, i + 10);
            STEP_2(1, 3, i + 11);
            STEP_2(4, 10, i + 12);
            STEP_2(1, 5, i + 13);
            STEP_2(2, 6, i + 14);
            STEP_2(1, 3, i + 15);
            STEP_2(11, 5 + alpha, i + 16);
            STEP_2(1, 12, i + 17);
            STEP_2(2, 13, i + 18);
            STEP_2(1, 3, i + 19);
            STEP_2(4, 14, i + 20);
            STEP_2(1, 5, i + 21);
            STEP_2(2, 6, i + 22);
            STEP_2(1, 3, i + 23);
            STEP_2(7, 15, i + 24);
            STEP_2(1, 8, i + 25);
            STEP_2(2, 9, i + 26);
            STEP_2(1, 3, i + 27);
            STEP_2(4, 10, i + 28);
            STEP_2(1, 5, i + 29);
            STEP_2(2, 6, i + 30);
            STEP_2(1, 3, i + 31);
            STEP_2(16, 6 + alpha, i + 32);
            STEP_2(1, 17, i + 33);
            STEP_2(2, 18, i + 34);
            STEP_2(1, 3, i + 35);
            STEP_2(4, 19, i + 36);
            STEP_2(1, 5, i + 37);
            STEP_2(2, 6, i + 38);
            STEP_2(1, 3, i + 39);
            STEP_2(7, 20, i + 40);
            STEP_2(1, 8, i + 41);
            STEP_2(2, 9, i + 42);
            STEP_2(1, 3, i + 43);
            STEP_2(4, 10, i + 44);
            STEP_2(1, 5, i + 45);
            STEP_2(2, 6, i + 46);
            STEP_2(1, 3, i + 47);
            STEP_2(11, 21, i + 48);
            STEP_2(1, 12, i + 49);
            STEP_2(2, 13, i + 50);
            STEP_2(1, 3, i + 51);
            STEP_2(4, 14, i + 52);
            STEP_2(1, 5, i + 53);
            STEP_2(2, 6, i + 54);
            STEP_2(1, 3, i + 55);
            STEP_2(7, 15, i + 56);
            STEP_2(1, 8, i + 57);
            STEP_2(2, 9, i + 58);
            STEP_2(1, 3, i + 59);
            STEP_2(4, 10, i + 60);
            STEP_2(1, 5, i + 61);
            STEP_2(2, 6, i + 62);
            STEP_2(1, 3, i + 63);
            STEP_2(22, 7 + alpha, i + 64);
            STEP_2(1, 23, i + 65);
            STEP_2(2, 24, i + 66);
            STEP_2(1, 3, i + 67);
            STEP_2(4, 25, i + 68);
            STEP_2(1, 5, i + 69);
            STEP_2(2, 6, i + 70);
            STEP_2(1, 3, i + 71);
            STEP_2(7, 26, i + 72);
            STEP_2(1, 8, i + 73);
            STEP_2(2, 9, i + 74);
            STEP_2(1, 3, i + 75);
            STEP_2(4, 10, i + 76);
            STEP_2(1, 5, i + 77);
            STEP_2(2, 6, i + 78);
            STEP_2(1, 3, i + 79);
            STEP_2(11, 27, i + 80);
            STEP_2(1, 12, i + 81);
            STEP_2(2, 13, i + 82);
            STEP_2(1, 3, i + 83);
            STEP_2(4, 14, i + 84);
            STEP_2(1, 5, i + 85);
            STEP_2(2, 6, i + 86);
            STEP_2(1, 3, i + 87);
            STEP_2(7, 15, i + 88);
            STEP_2(1, 8, i + 89);
            STEP_2(2, 9, i + 90);
            STEP_2(1, 3, i + 91);
            STEP_2(4, 10, i + 92);
            STEP_2(1, 5, i + 93);
            STEP_2(2, 6, i + 94);
            STEP_2(1, 3, i + 95);
            STEP_2(16, 28, i + 96);
            STEP_2(1, 17, i + 97);
            STEP_2(2, 18, i + 98);
            STEP_2(1, 3, i + 99);
            STEP_2(4, 19, i + 100);
            STEP_2(1, 5, i + 101);
            STEP_2(2, 6, i + 102);
            STEP_2(1, 3, i + 103);
            STEP_2(7, 20, i + 104);
            STEP_2(1, 8, i + 105);
            STEP_2(2, 9, i + 106);
            STEP_2(1, 3, i + 107);
            STEP_2(4, 10, i + 108);
            STEP_2(1, 5, i + 109);
            STEP_2(2, 6, i + 110);
            STEP_2(1, 3, i + 111);
            STEP_2(11, 21, i + 112);
            STEP_2(1, 12, i + 113);
            STEP_2(2, 13, i + 114);
            STEP_2(1, 3, i + 115);
            STEP_2(4, 14, i + 116);
            STEP_2(1, 5, i + 117);
            STEP_2(2, 6, i + 118);
            STEP_2(1, 3, i + 119);
            STEP_2(7, 15, i + 120);
            STEP_2(1, 8, i + 121);
            STEP_2(2, 9, i + 122);
            STEP_2(1, 3, i + 123);
            STEP_2(4, 10, i + 124);
            STEP_2(1, 5, i + 125);
            STEP_2(2, 6, i + 126);
            STEP_2(1, 3, i + 127);
            STEP_2(29, 8 + alpha, i + 128);
            STEP_2(1, 30, i + 129);
            STEP_2(2, 31, i + 130);
            STEP_2(1, 3, i + 131);
            STEP_2(4, 32, i + 132);
            STEP_2(1, 5, i + 133);
            STEP_2(2, 6, i + 134);
            STEP_2(1, 3, i + 135);
            STEP_2(7, 33, i + 136);
            STEP_2(1, 8, i + 137);
            STEP_2(2, 9, i + 138);
            STEP_2(1, 3, i + 139);
            STEP_2(4, 10, i + 140);
            STEP_2(1, 5, i + 141);
            STEP_2(2, 6, i + 142);
            STEP_2(1, 3, i + 143);
            STEP_2(11, 34, i + 144);
            STEP_2(1, 12, i + 145);
            STEP_2(2, 13, i + 146);
            STEP_2(1, 3, i + 147);
            STEP_2(4, 14, i + 148);
            STEP_2(1, 5, i + 149);
            STEP_2(2, 6, i + 150);
            STEP_2(1, 3, i + 151);
            STEP_2(7, 15, i + 152);
            STEP_2(1, 8, i + 153);
            STEP_2(2, 9, i + 154);
            STEP_2(1, 3, i + 155);
            STEP_2(4, 10, i + 156);
            STEP_2(1, 5, i + 157);
            STEP_2(2, 6, i + 158);
            STEP_2(1, 3, i + 159);
            STEP_2(16, 35, i + 160);
            STEP_2(1, 17, i + 161);
            STEP_2(2, 18, i + 162);
            STEP_2(1, 3, i + 163);
            STEP_2(4, 19, i + 164);
            STEP_2(1, 5, i + 165);
            STEP_2(2, 6, i + 166);
            STEP_2(1, 3, i + 167);
            STEP_2(7, 20, i + 168);
            STEP_2(1, 8, i + 169);
            STEP_2(2, 9, i + 170);
            STEP_2(1, 3, i + 171);
            STEP_2(4, 10, i + 172);
            STEP_2(1, 5, i + 173);
            STEP_2(2, 6, i + 174);
            STEP_2(1, 3, i + 175);
            STEP_2(11, 21, i + 176);
            STEP_2(1, 12, i + 177);
            STEP_2(2, 13, i + 178);
            STEP_2(1, 3, i + 179);
            STEP_2(4, 14, i + 180);
            STEP_2(1, 5, i + 181);
            STEP_2(2, 6, i + 182);
            STEP_2(1, 3, i + 183);
            STEP_2(7, 15, i + 184);
            STEP_2(1, 8, i + 185);
            STEP_2(2, 9, i + 186);
            STEP_2(1, 3, i + 187);
            STEP_2(4, 10, i + 188);
            STEP_2(1, 5, i + 189);
            STEP_2(2, 6, i + 190);
            STEP_2(1, 3, i + 191);
            STEP_2(22, 36, i + 192);
            STEP_2(1, 23, i + 193);
            STEP_2(2, 24, i + 194);
            STEP_2(1, 3, i + 195);
            STEP_2(4, 25, i + 196);
            STEP_2(1, 5, i + 197);
            STEP_2(2, 6, i + 198);
            STEP_2(1, 3, i + 199);
            STEP_2(7, 26, i + 200);
            STEP_2(1, 8, i + 201);
            STEP_2(2, 9, i + 202);
            STEP_2(1, 3, i + 203);
            STEP_2(4, 10, i + 204);
            STEP_2(1, 5, i + 205);
            STEP_2(2, 6, i + 206);
            STEP_2(1, 3, i + 207);
            STEP_2(11, 27, i + 208);
            STEP_2(1, 12, i + 209);
            STEP_2(2, 13, i + 210);
            STEP_2(1, 3, i + 211);
            STEP_2(4, 14, i + 212);
            STEP_2(1, 5, i + 213);
            STEP_2(2, 6, i + 214);
            STEP_2(1, 3, i + 215);
            STEP_2(7, 15, i + 216);
            STEP_2(1, 8, i + 217);
            STEP_2(2, 9, i + 218);
            STEP_2(1, 3, i + 219);
            STEP_2(4, 10, i + 220);
            STEP_2(1, 5, i + 221);
            STEP_2(2, 6, i + 222);
            STEP_2(1, 3, i + 223);
            STEP_2(16, 28, i + 224);
            STEP_2(1, 17, i + 225);
            STEP_2(2, 18, i + 226);
            STEP_2(1, 3, i + 227);
            STEP_2(4, 19, i + 228);
            STEP_2(1, 5, i + 229);
            STEP_2(2, 6, i + 230);
            STEP_2(1, 3, i + 231);
            STEP_2(7, 20, i + 232);
            STEP_2(1, 8, i + 233);
            STEP_2(2, 9, i + 234);
            STEP_2(1, 3, i + 235);
            STEP_2(4, 10, i + 236);
            STEP_2(1, 5, i + 237);
            STEP_2(2, 6, i + 238);
            STEP_2(1, 3, i + 239);
            STEP_2(11, 21, i + 240);
            STEP_2(1, 12, i + 241);
            STEP_2(2, 13, i + 242);
            STEP_2(1, 3, i + 243);
            STEP_2(4, 14, i + 244);
            STEP_2(1, 5, i + 245);
            STEP_2(2, 6, i + 246);
            STEP_2(1, 3, i + 247);
            STEP_2(7, 15, i + 248);
            STEP_2(1, 8, i + 249);
            STEP_2(2, 9, i + 250);
            STEP_2(1, 3, i + 251);
            STEP_2(4, 10, i + 252);
            STEP_2(1, 5, i + 253);
            STEP_2(2, 6, i + 254);
            STEP_2(1, 3, i + 255);
            STEP_2(37, 9 + alpha, i + 256);
            STEP_2(1, 38, i + 257);
            STEP_2(2, 39, i + 258);
            STEP_2(1, 3, i + 259);
            STEP_2(4, 40, i + 260);
            STEP_2(1, 5, i + 261);
            STEP_2(2, 6, i + 262);
            STEP_2(1, 3, i + 263);
            STEP_2(7, 41, i + 264);
            STEP_2(1, 8, i + 265);
            STEP_2(2, 9, i + 266);
            STEP_2(1, 3, i + 267);
            STEP_2(4, 10, i + 268);
            STEP_2(1, 5, i + 269);
            STEP_2(2, 6, i + 270);
            STEP_2(1, 3, i + 271);
            STEP_2(11, 42, i + 272);
            STEP_2(1, 12, i + 273);
            STEP_2(2, 13, i + 274);
            STEP_2(1, 3, i + 275);
            STEP_2(4, 14, i + 276);
            STEP_2(1, 5, i + 277);
            STEP_2(2, 6, i + 278);
            STEP_2(1, 3, i + 279);
            STEP_2(7, 15, i + 280);
            STEP_2(1, 8, i + 281);
            STEP_2(2, 9, i + 282);
            STEP_2(1, 3, i + 283);
            STEP_2(4, 10, i + 284);
            STEP_2(1, 5, i + 285);
            STEP_2(2, 6, i + 286);
            STEP_2(1, 3, i + 287);
            STEP_2(16, 43, i + 288);
            STEP_2(1, 17, i + 289);
            STEP_2(2, 18, i + 290);
            STEP_2(1, 3, i + 291);
            STEP_2(4, 19, i + 292);
            STEP_2(1, 5, i + 293);
            STEP_2(2, 6, i + 294);
            STEP_2(1, 3, i + 295);
            STEP_2(7, 20, i + 296);
            STEP_2(1, 8, i + 297);
            STEP_2(2, 9, i + 298);
            STEP_2(1, 3, i + 299);
            STEP_2(4, 10, i + 300);
            STEP_2(1, 5, i + 301);
            STEP_2(2, 6, i + 302);
            STEP_2(1, 3, i + 303);
            STEP_2(11, 21, i + 304);
            STEP_2(1, 12, i + 305);
            STEP_2(2, 13, i + 306);
            STEP_2(1, 3, i + 307);
            STEP_2(4, 14, i + 308);
            STEP_2(1, 5, i + 309);
            STEP_2(2, 6, i + 310);
            STEP_2(1, 3, i + 311);
            STEP_2(7, 15, i + 312);
            STEP_2(1, 8, i + 313);
            STEP_2(2, 9, i + 314);
            STEP_2(1, 3, i + 315);
            STEP_2(4, 10, i + 316);
            STEP_2(1, 5, i + 317);
            STEP_2(2, 6, i + 318);
            STEP_2(1, 3, i + 319);
            STEP_2(22, 44, i + 320);
            STEP_2(1, 23, i + 321);
            STEP_2(2, 24, i + 322);
            STEP_2(1, 3, i + 323);
            STEP_2(4, 25, i + 324);
            STEP_2(1, 5, i + 325);
            STEP_2(2, 6, i + 326);
            STEP_2(1, 3, i + 327);
            STEP_2(7, 26, i + 328);
            STEP_2(1, 8, i + 329);
            STEP_2(2, 9, i + 330);
            STEP_2(1, 3, i + 331);
            STEP_2(4, 10, i + 332);
            STEP_2(1, 5, i + 333);
            STEP_2(2, 6, i + 334);
            STEP_2(1, 3, i + 335);
            STEP_2(11, 27, i + 336);
            STEP_2(1, 12, i + 337);
            STEP_2(2, 13, i + 338);
            STEP_2(1, 3, i + 339);
            STEP_2(4, 14, i + 340);
            STEP_2(1, 5, i + 341);
            STEP_2(2, 6, i + 342);
            STEP_2(1, 3, i + 343);
            STEP_2(7, 15, i + 344);
            STEP_2(1, 8, i + 345);
            STEP_2(2, 9, i + 346);
            STEP_2(1, 3, i + 347);
            STEP_2(4, 10, i + 348);
            STEP_2(1, 5, i + 349);
            STEP_2(2, 6, i + 350);
            STEP_2(1, 3, i + 351);
            STEP_2(16, 28, i + 352);
            STEP_2(1, 17, i + 353);
            STEP_2(2, 18, i + 354);
            STEP_2(1, 3, i + 355);
            STEP_2(4, 19, i + 356);
            STEP_2(1, 5, i + 357);
            STEP_2(2, 6, i + 358);
            STEP_2(1, 3, i + 359);
            STEP_2(7, 20, i + 360);
            STEP_2(1, 8, i + 361);
            STEP_2(2, 9, i + 362);
            STEP_2(1, 3, i + 363);
            STEP_2(4, 10, i + 364);
            STEP_2(1, 5, i + 365);
            STEP_2(2, 6, i + 366);
            STEP_2(1, 3, i + 367);
            STEP_2(11, 21, i + 368);
            STEP_2(1, 12, i + 369);
            STEP_2(2, 13, i + 370);
            STEP_2(1, 3, i + 371);
            STEP_2(4, 14, i + 372);
            STEP_2(1, 5, i + 373);
            STEP_2(2, 6, i + 374);
            STEP_2(1, 3, i + 375);
            STEP_2(7, 15, i + 376);
            STEP_2(1, 8, i + 377);
            STEP_2(2, 9, i + 378);
            STEP_2(1, 3, i + 379);
            STEP_2(4, 10, i + 380);
            STEP_2(1, 5, i + 381);
            STEP_2(2, 6, i + 382);
            STEP_2(1, 3, i + 383);
            STEP_2(29, 45, i + 384);
            STEP_2(1, 30, i + 385);
            STEP_2(2, 31, i + 386);
            STEP_2(1, 3, i + 387);
            STEP_2(4, 32, i + 388);
            STEP_2(1, 5, i + 389);
            STEP_2(2, 6, i + 390);
            STEP_2(1, 3, i + 391);
            STEP_2(7, 33, i + 392);
            STEP_2(1, 8, i + 393);
            STEP_2(2, 9, i + 394);
            STEP_2(1, 3, i + 395);
            STEP_2(4, 10, i + 396);
            STEP_2(1, 5, i + 397);
            STEP_2(2, 6, i + 398);
            STEP_2(1, 3, i + 399);
            STEP_2(11, 34, i + 400);
            STEP_2(1, 12, i + 401);
            STEP_2(2, 13, i + 402);
            STEP_2(1, 3, i + 403);
            STEP_2(4, 14, i + 404);
            STEP_2(1, 5, i + 405);
            STEP_2(2, 6, i + 406);
            STEP_2(1, 3, i + 407);
            STEP_2(7, 15, i + 408);
            STEP_2(1, 8, i + 409);
            STEP_2(2, 9, i + 410);
            STEP_2(1, 3, i + 411);
            STEP_2(4, 10, i + 412);
            STEP_2(1, 5, i + 413);
            STEP_2(2, 6, i + 414);
            STEP_2(1, 3, i + 415);
            STEP_2(16, 35, i + 416);
            STEP_2(1, 17, i + 417);
            STEP_2(2, 18, i + 418);
            STEP_2(1, 3, i + 419);
            STEP_2(4, 19, i + 420);
            STEP_2(1, 5, i + 421);
            STEP_2(2, 6, i + 422);
            STEP_2(1, 3, i + 423);
            STEP_2(7, 20, i + 424);
            STEP_2(1, 8, i + 425);
            STEP_2(2, 9, i + 426);
            STEP_2(1, 3, i + 427);
            STEP_2(4, 10, i + 428);
            STEP_2(1, 5, i + 429);
            STEP_2(2, 6, i + 430);
            STEP_2(1, 3, i + 431);
            STEP_2(11, 21, i + 432);
            STEP_2(1, 12, i + 433);
            STEP_2(2, 13, i + 434);
            STEP_2(1, 3, i + 435);
            STEP_2(4, 14, i + 436);
            STEP_2(1, 5, i + 437);
            STEP_2(2, 6, i + 438);
            STEP_2(1, 3, i + 439);
            STEP_2(7, 15, i + 440);
            STEP_2(1, 8, i + 441);
            STEP_2(2, 9, i + 442);
            STEP_2(1, 3, i + 443);
            STEP_2(4, 10, i + 444);
            STEP_2(1, 5, i + 445);
            STEP_2(2, 6, i + 446);
            STEP_2(1, 3, i + 447);
            STEP_2(22, 36, i + 448);
            STEP_2(1, 23, i + 449);
            STEP_2(2, 24, i + 450);
            STEP_2(1, 3, i + 451);
            STEP_2(4, 25, i + 452);
            STEP_2(1, 5, i + 453);
            STEP_2(2, 6, i + 454);
            STEP_2(1, 3, i + 455);
            STEP_2(7, 26, i + 456);
            STEP_2(1, 8, i + 457);
            STEP_2(2, 9, i + 458);
            STEP_2(1, 3, i + 459);
            STEP_2(4, 10, i + 460);
            STEP_2(1, 5, i + 461);
            STEP_2(2, 6, i + 462);
            STEP_2(1, 3, i + 463);
            STEP_2(11, 27, i + 464);
            STEP_2(1, 12, i + 465);
            STEP_2(2, 13, i + 466);
            STEP_2(1, 3, i + 467);
            STEP_2(4, 14, i + 468);
            STEP_2(1, 5, i + 469);
            STEP_2(2, 6, i + 470);
            STEP_2(1, 3, i + 471);
            STEP_2(7, 15, i + 472);
            STEP_2(1, 8, i + 473);
            STEP_2(2, 9, i + 474);
            STEP_2(1, 3, i + 475);
            STEP_2(4, 10, i + 476);
            STEP_2(1, 5, i + 477);
            STEP_2(2, 6, i + 478);
            STEP_2(1, 3, i + 479);
            STEP_2(16, 28, i + 480);
            STEP_2(1, 17, i + 481);
            STEP_2(2, 18, i + 482);
            STEP_2(1, 3, i + 483);
            STEP_2(4, 19, i + 484);
            STEP_2(1, 5, i + 485);
            STEP_2(2, 6, i + 486);
            STEP_2(1, 3, i + 487);
            STEP_2(7, 20, i + 488);
            STEP_2(1, 8, i + 489);
            STEP_2(2, 9, i + 490);
            STEP_2(1, 3, i + 491);
            STEP_2(4, 10, i + 492);
            STEP_2(1, 5, i + 493);
            STEP_2(2, 6, i + 494);
            STEP_2(1, 3, i + 495);
            STEP_2(11, 21, i + 496);
            STEP_2(1, 12, i + 497);
            STEP_2(2, 13, i + 498);
            STEP_2(1, 3, i + 499);
            STEP_2(4, 14, i + 500);
            STEP_2(1, 5, i + 501);
            STEP_2(2, 6, i + 502);
            STEP_2(1, 3, i + 503);
            STEP_2(7, 15, i + 504);
            STEP_2(1, 8, i + 505);
            STEP_2(2, 9, i + 506);
            STEP_2(1, 3, i + 507);
            STEP_2(4, 10, i + 508);
            STEP_2(1, 5, i + 509);
            STEP_2(2, 6, i + 510);
            STEP_2(1, 3, i + 511);
            CHECK_SOLUTIONS();
        }
    }
    testSolution(wrapper_state_ptr, current_solution_index, packVectors_of_solution);
}
