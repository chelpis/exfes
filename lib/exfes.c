// binomials.h provides
//
// external object declaration:
//      binomials

#ifndef BINOMIALS_H_INCLUDED
#define BINOMIALS_H_INCLUDED

#include <stdint.h>  // uint64_t

extern const uint64_t binomials[64][64];

#endif  // BINOMIALS_H_INCLUDED
// exfes.h provides
//
// function declarations:
//      exfes

#ifndef EXFES_H_INCLUDED
#define EXFES_H_INCLUDED

#include <stdbool.h>  // bool
#include <stdint.h>  // uint8_t, uint32_t, uint64_t

#ifdef __cplusplus
extern "C" {
#endif

int exfes(uint32_t numFixedVariables, uint32_t numVariables, uint32_t numEquations, uint64_t startPointHigh, uint64_t startPointLow, const uint8_t *coefficientsMatrix, bool (*shouldAbortNow)(void), uint64_t *solutionHigh, uint64_t *solutionLow);

#ifdef __cplusplus
}
#endif

#endif  // EXFES_H_INCLUDED
// my_memory.h provides
//
// function declarations:
//      exfes_calloc

#ifndef MY_MEMORY_H_INCLUDED
#define MY_MEMORY_H_INCLUDED

#include <stdlib.h>  // size_t

void *exfes_calloc(size_t num, size_t size, size_t max_num_retries);

#endif  // MY_MEMORY_H_INCLUDED
// exfes_internal.h provides
//
// types:
//      struct exfes_context

#ifndef EXFES_INTERNAL_H_INCLUDED
#define EXFES_INTERNAL_H_INCLUDED

#include <stdbool.h>  // bool
#include <stdint.h>  // uint64_t

struct exfes_context {
    int mcopy;
    int ncopy;
    uint64_t solm;
    uint64_t SolCount;
    uint64_t startPointHigh;
    uint64_t startPointLow;
    uint64_t *solutionHigh;
    uint64_t *solutionLow;
    bool (*shouldAbortNow)(void);
};

#endif  // EXFES_INTERNAL_H_INCLUDED
// idx_LUT.h provides
//
// macro definitions:
//      idx_0
//      idx_1
//      idx_2
//      idx_3
//      idx_4
//      idx_5
//      idx_6
//      idx_7
//      idx_8
//      idx_9
//      idx_10
//
// types:
//      LUT_int_t
//      LUT_t
//      idx_lut_t
//
// function declarations:
//      init_deginvlex_LUT
//      free_LUT
//      set2int
//      n_monomials

#ifndef IDX_LUT_H_INCLUDED
#define IDX_LUT_H_INCLUDED

#include <stdint.h>  // uint32_t, uint64_t

// These macros implement the same functionnality as set2int(), but
// they are much faster and lend themselves to compile-time
// optimisations. The `LUT` argument should be the LUT field of an
// idx_lut_r structure. The smallest index must be given first.
// for example (if the LUTs are generated with d=4):
//     idx_2(LUT, 1,3)   --> 10
//     idx_1(LUT, 4)     --> 16
//     idx_3(LUT, 0,2,3) --> 13
#define idx_0(LUT) (0)
#define idx_1(LUT, i0) ((LUT)[0][i0])
#define idx_2(LUT, i1, i0) (idx_1(LUT, i0) + (LUT)[1][i1])
#define idx_3(LUT, i2, i1, i0) (idx_2(LUT, i1, i0) + (LUT)[2][i2])
#define idx_4(LUT, i3, i2, i1, i0) (idx_3(LUT, i2, i1, i0) + (LUT)[3][i3])
#define idx_5(LUT, i4, i3, i2, i1, i0) (idx_4(LUT, i3, i2, i1, i0) + (LUT)[4][i4])
#define idx_6(LUT, i5, i4, i3, i2, i1, i0) (idx_5(LUT, i4, i3, i2, i1, i0) + (LUT)[5][i5])
#define idx_7(LUT, i6, i5, i4, i3, i2, i1, i0) (idx_6(LUT, i5, i4, i3, i2, i1, i0) + (LUT)[6][i6])
#define idx_8(LUT, i7, i6, i5, i4, i3, i2, i1, i0) (idx_7(LUT, i6, i5, i4, i3, i2, i1, i0) + (LUT)[7][i7])
#define idx_9(LUT, i8, i7, i6, i5, i4, i3, i2, i1, i0) (idx_8(LUT, i7, i6, i5, i4, i3, i2, i1, i0) + (LUT)[8][i8])
#define idx_10(LUT, i9, i8, i7, i6, i5, i4, i3, i2, i1, i0) (idx_9(LUT, i8, i7, i6, i5, i4, i3, i2, i1, i0) + (LUT)[9][i9])

// Type of the entries in the lookup tables. It must be
// sufficiently large to represent N(n,d)
typedef uint32_t LUT_int_t;

// Actual type of the lookup tables
typedef const LUT_int_t *const *const LUT_t;

// A "package" that wraps the actual lookup tables
// along with useful informations
typedef const struct {
    LUT_t LUT;
    const int n;
    const int d;
} idx_lut_t;

// initialise a lookup table that works with n variables, and
// up to degree d. It also works for smaller degree, but shouldn't be
// used with less variables (otherwise there will be "holes" in the
// indexing sequence
const idx_lut_t *init_deginvlex_LUT(int n, int d);

// reclaim the memory used by the lookup table
void free_LUT(const idx_lut_t *tab);

// generic (and somewhat slow) function that computes an integer
// index given a set of indices. It is understood that `set` is an
// array of at least `d` entries (where `d` was the argument given to
// the init_idx_LUT function). If d=5, then the monomial [1,4,6,7,8]
// should be represented by the array {8,7,6,4,1}, and the monomial
// [0,4,6] should be represented by {6,4,0,-1,-1}.
LUT_int_t set2int(const idx_lut_t *table, int *set);

uint64_t n_monomials(int n, int d);

#endif  // IDX_LUT_H_INCLUDED
// fes_interface.h provides
//
// function declarations:
//      exhaustive_search_wrapper

#ifndef FES_INTERFACE_H_INCLUDED
#define FES_INTERFACE_H_INCLUDED

int exhaustive_search_wrapper(const int n, int n_eqs, const int degree, int ***coeffs, struct exfes_context *exfes_ctx_ptr);

#endif  // FES_INTERFACE_H_INCLUDED
// fes.h provides
//
// macro definitions:
//      min
//      unlikely
//
// types:
//      vector_t
//      matrix_t
//      pck_vector_t
//      quadratic_form
//      wrapper_state_t
//      system_t
//
// function declarations:
//      free_vector
//      init_vector
//      pack
//      to_gray
//      packed_eval
//      variables_specialization
//      exhaustive_ia32_deg_2
//      packed_eval_deg_2

#ifndef FES_H_INCLUDED
#define FES_H_INCLUDED

#include <stdint.h>  // uint32_t, uint64_t

#define min(x, y) (((x) > (y)) ? (y) : (x))
#define unlikely(x) __builtin_expect(!!(x), 0)

typedef int *vector_t;
typedef vector_t *matrix_t;
typedef uint32_t pck_vector_t;

typedef struct {
    matrix_t quad;
    vector_t lin;
    int con;
} quadratic_form;

typedef quadratic_form *system_t;

typedef struct {
    int n;
    int degree;
    int n_batches;
    pck_vector_t **G;
    idx_lut_t *testing_LUT;
    struct exfes_context *exfes_ctx_ptr;
} wrapper_state_t;

void free_vector(vector_t x);
vector_t init_vector(int n_rows);
pck_vector_t pack(int n, const vector_t v);
uint64_t to_gray(uint64_t i);
pck_vector_t packed_eval(LUT_t LUT, int n, pck_vector_t *F, uint64_t i);
void variables_specialization(LUT_t LUT, int n, int d, pck_vector_t *A, int k, int i);
void exhaustive_ia32_deg_2(LUT_t LUT, int n, pck_vector_t *F, wrapper_state_t *wrapper_state_ptr);
pck_vector_t packed_eval_deg_2(LUT_t LUT, int n, pck_vector_t *F, uint64_t i);

#endif  // FES_H_INCLUDED
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint64_t int_idx;
    uint32_t mask;
} solution_t;

int solution_tester(wrapper_state_t *wrapper_state_ptr, uint64_t size, uint64_t *n_solutions);  // quick hack

// generated with L = 9
void exhaustive_ia32_deg_2(LUT_t LUT, int n, pck_vector_t *F, wrapper_state_t *wrapper_state_ptr)
{
    struct exfes_context *ctx = wrapper_state_ptr->exfes_ctx_ptr;

    // computes the derivatives required by the enumeration kernel up to degree 2
    // this is done in-place, meaning that if "F" described the coefficients of
    // the polynomials before, then afterwards, they describe the derivatives

    // here, degree-1 terms are affected by degree-2 terms
    for (int i0 = 1; i0 < n; i0++) {
        if (i0 != 0) {
            F[idx_1(LUT, i0)] ^= F[idx_2(LUT, i0 - 1, i0)];
        }
    }

    uint64_t n_solutions_found = 0;
    uint64_t current_solution_index = 0;
    uint64_t pack_of_solution[1];
    solution_t solution_buffer[516];

#define PUSH_SOLUTION(current_solution)                                                         \
    {                                                                                           \
        pack_of_solution[current_solution_index] = current_solution;                            \
        current_solution_index++;                                                               \
        if (current_solution_index == 1) {                                                      \
            /* FLUSH_SOLUTIONS */                                                               \
            if (solution_tester(wrapper_state_ptr, current_solution_index, pack_of_solution)) { \
                return;                                                                         \
            }                                                                                   \
            current_solution_index = 0;                                                         \
        }                                                                                       \
    }

#define CHECK_SOLUTIONS()                                           \
    {                                                               \
        for (uint64_t i = 0; i < n_solutions_found; i++) {          \
            if (solution_buffer[i].mask & 0xffff) {                 \
                PUSH_SOLUTION(to_gray(solution_buffer[i].int_idx)); \
            }                                                       \
        }                                                           \
        n_solutions_found = 0;                                      \
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

    // special case for i=0
    const uint64_t weight_0_start = 0;
    STEP_0(0);

    // from now on, hamming weight is >= 1
    for (int idx_0 = 0; idx_0 < n; idx_0++) {

        // special case when i has hamming weight exactly 1
        const uint64_t weight_1_start = weight_0_start + (1ULL << idx_0);
        STEP_1(idx_1(LUT, idx_0), weight_1_start);

        // we are now inside the critical part where the hamming weight is known to
        // be >= 2 Thus, there are no special cases from now on

        // Because of the last step, the current iteration counter is a multiple of
        // 512 plus one This loop sets it to `rolled_end`, which is a multiple of
        // 512, if possible

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

        // Here, the number of iterations to perform is (supposedly) sufficiently
        // large We will therefore unroll the loop 512 times

        // unrolled critical section where the hamming weight is >= 2
        for (uint64_t j = 512; j < (1ull << idx_0); j += 512) {

            if (ctx->shouldAbortNow()) {
                ctx->SolCount = 2;
                return;  // early abort because some other node found an answer
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

    /* FLUSH_SOLUTIONS */
    solution_tester(wrapper_state_ptr, current_solution_index, pack_of_solution);
}
#include <stdint.h>
#include <stdlib.h>

pck_vector_t packed_eval_deg_2(LUT_t LUT, int n, pck_vector_t *F, uint64_t i)
{
    // first expand the values of the variables from `i`
    pck_vector_t v[n];
    for (int k = 0; k < n; k++) {
        v[k] = 0;
        if (i & 0x0001) {
            v[k] = 0xffffffff;
        }
        i = (i >> 1ll);
    }

    pck_vector_t y = F[0];

    for (int idx_0 = 0; idx_0 < n; idx_0++) {
        const pck_vector_t v_0 = v[idx_0];

        // computes the contribution of degree-1 terms
        y ^= F[idx_1(LUT, idx_0)] & v_0;

        for (int idx_1 = 0; idx_1 < idx_0; idx_1++) {
            const pck_vector_t v_1 = v_0 & v[idx_1];

            // computes the contribution of degree-2 terms
            y ^= F[idx_2(LUT, idx_1, idx_0)] & v_1;
        }
    }

    return y;
}
#include <stdint.h>  // uint64_t

const uint64_t binomials[64][64] = { { 1 }, { 1, 1 }, { 1, 2, 1 }, { 1, 3, 3, 1 }, { 1, 4, 6, 4, 1 }, { 1, 5, 10, 10, 5, 1 }, { 1, 6, 15, 20, 15, 6, 1 }, { 1, 7, 21, 35, 35, 21, 7, 1 }, { 1, 8, 28, 56, 70, 56, 28, 8, 1 }, { 1, 9, 36, 84, 126, 126, 84, 36, 9, 1 }, { 1, 10, 45, 120, 210, 252, 210, 120, 45, 10, 1 }, { 1, 11, 55, 165, 330, 462, 462, 330, 165, 55, 11, 1 }, { 1, 12, 66, 220, 495, 792, 924, 792, 495, 220, 66, 12, 1 }, { 1, 13, 78, 286, 715, 1287, 1716, 1716, 1287, 715, 286, 78, 13, 1 }, { 1, 14, 91, 364, 1001, 2002, 3003, 3432, 3003, 2002, 1001, 364, 91, 14, 1 }, { 1, 15, 105, 455, 1365, 3003, 5005, 6435, 6435, 5005, 3003, 1365, 455, 105, 15, 1 }, { 1, 16, 120, 560, 1820, 4368, 8008, 11440, 12870, 11440, 8008, 4368, 1820, 560, 120, 16, 1 }, { 1, 17, 136, 680, 2380, 6188, 12376, 19448, 24310, 24310, 19448, 12376, 6188, 2380, 680, 136, 17, 1 }, { 1, 18, 153, 816, 3060, 8568, 18564, 31824, 43758, 48620, 43758, 31824, 18564, 8568, 3060, 816, 153, 18, 1 }, { 1, 19, 171, 969, 3876, 11628, 27132, 50388, 75582, 92378, 92378, 75582, 50388, 27132, 11628, 3876, 969, 171, 19, 1 }, { 1, 20, 190, 1140, 4845, 15504, 38760, 77520, 125970, 167960, 184756, 167960, 125970, 77520, 38760, 15504, 4845, 1140, 190, 20, 1 }, { 1, 21, 210, 1330, 5985, 20349, 54264, 116280, 203490, 293930, 352716, 352716, 293930, 203490, 116280, 54264, 20349, 5985, 1330, 210, 21, 1 }, { 1, 22, 231, 1540, 7315, 26334, 74613, 170544, 319770, 497420, 646646, 705432, 646646, 497420, 319770, 170544, 74613, 26334, 7315, 1540, 231, 22, 1 }, { 1, 23, 253, 1771, 8855, 33649, 100947, 245157, 490314, 817190, 1144066, 1352078, 1352078, 1144066, 817190, 490314, 245157, 100947, 33649, 8855, 1771, 253, 23, 1 }, { 1, 24, 276, 2024, 10626, 42504, 134596, 346104, 735471, 1307504, 1961256, 2496144, 2704156, 2496144, 1961256, 1307504, 735471, 346104, 134596, 42504, 10626, 2024, 276, 24, 1 }, { 1, 25, 300, 2300, 12650, 53130, 177100, 480700, 1081575, 2042975, 3268760, 4457400, 5200300, 5200300, 4457400, 3268760, 2042975, 1081575, 480700, 177100, 53130, 12650, 2300, 300, 25, 1 }, { 1, 26, 325, 2600, 14950, 65780, 230230, 657800, 1562275, 3124550, 5311735, 7726160, 9657700, 10400600, 9657700, 7726160, 5311735, 3124550, 1562275, 657800, 230230, 65780, 14950, 2600, 325, 26, 1 }, { 1, 27, 351, 2925, 17550, 80730, 296010, 888030, 2220075, 4686825, 8436285, 13037895, 17383860, 20058300, 20058300, 17383860, 13037895, 8436285, 4686825, 2220075, 888030, 296010, 80730, 17550, 2925, 351, 27, 1 }, { 1, 28, 378, 3276, 20475, 98280, 376740, 1184040, 3108105, 6906900, 13123110, 21474180, 30421755, 37442160, 40116600, 37442160, 30421755, 21474180, 13123110, 6906900, 3108105, 1184040, 376740, 98280, 20475, 3276, 378, 28, 1 }, { 1, 29, 406, 3654, 23751, 118755, 475020, 1560780, 4292145, 10015005, 20030010, 34597290, 51895935, 67863915, 77558760, 77558760, 67863915, 51895935, 34597290, 20030010, 10015005, 4292145, 1560780, 475020, 118755, 23751, 3654, 406, 29, 1 }, { 1, 30, 435, 4060, 27405, 142506, 593775, 2035800, 5852925, 14307150, 30045015, 54627300, 86493225, 119759850, 145422675, 155117520, 145422675, 119759850, 86493225, 54627300, 30045015, 14307150, 5852925, 2035800, 593775, 142506, 27405, 4060, 435, 30, 1 }, { 1, 31, 465, 4495, 31465, 169911, 736281, 2629575, 7888725, 20160075, 44352165, 84672315, 141120525, 206253075, 265182525, 300540195, 300540195, 265182525, 206253075, 141120525, 84672315, 44352165, 20160075, 7888725, 2629575, 736281, 169911, 31465, 4495, 465, 31, 1 }, { 1, 32, 496, 4960, 35960, 201376, 906192, 3365856, 10518300, 28048800, 64512240, 129024480, 225792840, 347373600, 471435600, 565722720, 601080390, 565722720, 471435600, 347373600, 225792840, 129024480, 64512240, 28048800, 10518300, 3365856, 906192, 201376, 35960, 4960, 496, 32, 1 }, { 1, 33, 528, 5456, 40920, 237336, 1107568, 4272048, 13884156, 38567100, 92561040, 193536720, 354817320, 573166440, 818809200, 1037158320, 1166803110, 1166803110, 1037158320, 818809200, 573166440, 354817320, 193536720, 92561040, 38567100, 13884156, 4272048, 1107568, 237336, 40920, 5456, 528, 33, 1 }, { 1, 34, 561, 5984, 46376, 278256, 1344904, 5379616, 18156204, 52451256, 131128140, 286097760, 548354040, 927983760, 1391975640, 1855967520, 2203961430, 2333606220, 2203961430, 1855967520, 1391975640, 927983760, 548354040, 286097760, 131128140, 52451256, 18156204, 5379616, 1344904, 278256, 46376, 5984, 561, 34, 1 }, { 1, 35, 595, 6545, 52360, 324632, 1623160, 6724520, 23535820, 70607460, 183579396, 417225900, 834451800, 1476337800, 2319959400, 3247943160, 4059928950, 4537567650, 4537567650, 4059928950, 3247943160, 2319959400, 1476337800, 834451800, 417225900, 183579396, 70607460, 23535820, 6724520, 1623160, 324632, 52360, 6545, 595, 35, 1 }, { 1, 36, 630, 7140, 58905, 376992, 1947792, 8347680, 30260340, 94143280, 254186856, 600805296, 1251677700, 2310789600, 3796297200, 5567902560, 7307872110, 8597496600, 9075135300, 8597496600, 7307872110, 5567902560, 3796297200, 2310789600, 1251677700, 600805296, 254186856, 94143280, 30260340, 8347680, 1947792, 376992, 58905, 7140, 630, 36, 1 }, { 1, 37, 666, 7770, 66045, 435897, 2324784, 10295472, 38608020, 124403620, 348330136, 854992152, 1852482996, 3562467300, 6107086800, 9364199760, 12875774670, 15905368710, 17672631900, 17672631900, 15905368710, 12875774670, 9364199760, 6107086800, 3562467300, 1852482996, 854992152, 348330136, 124403620, 38608020, 10295472, 2324784, 435897, 66045, 7770, 666, 37, 1 }, { 1, 38, 703, 8436, 73815, 501942, 2760681, 12620256, 48903492, 163011640, 472733756, 1203322288, 2707475148, 5414950296, 9669554100, 15471286560, 22239974430, 28781143380, 33578000610, 35345263800, 33578000610, 28781143380, 22239974430, 15471286560, 9669554100, 5414950296, 2707475148, 1203322288, 472733756, 163011640, 48903492, 12620256, 2760681, 501942, 73815, 8436, 703, 38, 1 }, { 1, 39, 741, 9139, 82251, 575757, 3262623, 15380937, 61523748, 211915132, 635745396, 1676056044, 3910797436, 8122425444, 15084504396, 25140840660, 37711260990, 51021117810, 62359143990, 68923264410, 68923264410, 62359143990, 51021117810, 37711260990, 25140840660, 15084504396, 8122425444, 3910797436, 1676056044, 635745396, 211915132, 61523748, 15380937, 3262623, 575757, 82251, 9139, 741, 39, 1 }, { 1, 40, 780, 9880, 91390, 658008, 3838380, 18643560, 76904685, 273438880, 847660528, 2311801440, 5586853480, 12033222880, 23206929840, 40225345056, 62852101650, 88732378800, 113380261800, 131282408400, 137846528820, 131282408400, 113380261800, 88732378800, 62852101650, 40225345056, 23206929840, 12033222880, 5586853480, 2311801440, 847660528, 273438880, 76904685, 18643560, 3838380, 658008, 91390, 9880, 780, 40, 1 }, { 1, 41, 820, 10660, 101270, 749398, 4496388, 22481940, 95548245, 350343565, 1121099408, 3159461968, 7898654920, 17620076360, 35240152720, 63432274896, 103077446706, 151584480450, 202112640600, 244662670200, 269128937220, 269128937220, 244662670200, 202112640600, 151584480450, 103077446706, 63432274896, 35240152720, 17620076360, 7898654920, 3159461968, 1121099408, 350343565, 95548245, 22481940, 4496388, 749398, 101270, 10660, 820, 41, 1 }, { 1, 42, 861, 11480, 111930, 850668, 5245786, 26978328, 118030185, 445891810, 1471442973, 4280561376, 11058116888, 25518731280, 52860229080, 98672427616, 166509721602, 254661927156, 353697121050, 446775310800, 513791607420, 538257874440, 513791607420, 446775310800, 353697121050, 254661927156, 166509721602, 98672427616, 52860229080, 25518731280, 11058116888, 4280561376, 1471442973, 445891810, 118030185, 26978328, 5245786, 850668, 111930, 11480, 861, 42, 1 }, { 1, 43, 903, 12341, 123410, 962598, 6096454, 32224114, 145008513, 563921995, 1917334783, 5752004349, 15338678264, 36576848168, 78378960360, 151532656696, 265182149218, 421171648758, 608359048206, 800472431850, 960566918220, 1052049481860, 1052049481860, 960566918220, 800472431850, 608359048206, 421171648758, 265182149218, 151532656696, 78378960360, 36576848168, 15338678264, 5752004349, 1917334783, 563921995, 145008513, 32224114, 6096454, 962598, 123410, 12341, 903, 43, 1 }, { 1, 44, 946, 13244, 135751, 1086008, 7059052, 38320568, 177232627, 708930508, 2481256778, 7669339132, 21090682613, 51915526432, 114955808528, 229911617056, 416714805914, 686353797976, 1029530696964, 1408831480056, 1761039350070, 2012616400080, 2104098963720, 2012616400080, 1761039350070, 1408831480056, 1029530696964, 686353797976, 416714805914, 229911617056, 114955808528, 51915526432, 21090682613, 7669339132, 2481256778, 708930508, 177232627, 38320568, 7059052, 1086008, 135751, 13244, 946, 44, 1 }, { 1, 45, 990, 14190, 148995, 1221759, 8145060, 45379620, 215553195, 886163135, 3190187286, 10150595910, 28760021745, 73006209045, 166871334960, 344867425584, 646626422970, 1103068603890, 1715884494940, 2438362177020, 3169870830126, 3773655750150, 4116715363800, 4116715363800, 3773655750150, 3169870830126, 2438362177020, 1715884494940, 1103068603890, 646626422970, 344867425584, 166871334960, 73006209045, 28760021745, 10150595910, 3190187286, 886163135, 215553195, 45379620, 8145060, 1221759, 148995, 14190, 990, 45, 1 }, { 1, 46, 1035, 15180, 163185, 1370754, 9366819, 53524680, 260932815, 1101716330, 4076350421, 13340783196, 38910617655, 101766230790, 239877544005, 511738760544, 991493848554, 1749695026860, 2818953098830, 4154246671960, 5608233007146, 6943526580276, 7890371113950, 8233430727600, 7890371113950, 6943526580276, 5608233007146, 4154246671960, 2818953098830, 1749695026860, 991493848554, 511738760544, 239877544005, 101766230790, 38910617655, 13340783196, 4076350421, 1101716330, 260932815, 53524680, 9366819, 1370754, 163185, 15180, 1035, 46, 1 }, { 1, 47, 1081, 16215, 178365, 1533939, 10737573, 62891499, 314457495, 1362649145, 5178066751, 17417133617, 52251400851, 140676848445, 341643774795, 751616304549, 1503232609098, 2741188875414, 4568648125690, 6973199770790, 9762479679106, 12551759587422, 14833897694226, 16123801841550, 16123801841550, 14833897694226, 12551759587422, 9762479679106, 6973199770790, 4568648125690, 2741188875414, 1503232609098, 751616304549, 341643774795, 140676848445, 52251400851, 17417133617, 5178066751, 1362649145, 314457495, 62891499, 10737573, 1533939, 178365, 16215, 1081, 47, 1 }, { 1, 48, 1128, 17296, 194580, 1712304, 12271512, 73629072, 377348994, 1677106640, 6540715896, 22595200368, 69668534468, 192928249296, 482320623240, 1093260079344, 2254848913647, 4244421484512, 7309837001104, 11541847896480, 16735679449896, 22314239266528, 27385657281648, 30957699535776, 32247603683100, 30957699535776, 27385657281648, 22314239266528, 16735679449896, 11541847896480, 7309837001104, 4244421484512, 2254848913647, 1093260079344, 482320623240, 192928249296, 69668534468, 22595200368, 6540715896, 1677106640, 377348994, 73629072, 12271512, 1712304, 194580, 17296, 1128, 48, 1 }, { 1, 49, 1176, 18424, 211876, 1906884, 13983816, 85900584, 450978066, 2054455634, 8217822536, 29135916264, 92263734836, 262596783764, 675248872536, 1575580702584, 3348108992991, 6499270398159, 11554258485616, 18851684897584, 28277527346376, 39049918716424, 49699896548176, 58343356817424, 63205303218876, 63205303218876, 58343356817424, 49699896548176, 39049918716424, 28277527346376, 18851684897584, 11554258485616, 6499270398159, 3348108992991, 1575580702584, 675248872536, 262596783764, 92263734836, 29135916264, 8217822536, 2054455634, 450978066, 85900584, 13983816, 1906884, 211876, 18424, 1176, 49, 1 }, { 1, 50, 1225, 19600, 230300, 2118760, 15890700, 99884400, 536878650, 2505433700, 10272278170, 37353738800, 121399651100, 354860518600, 937845656300, 2250829575120, 4923689695575, 9847379391150, 18053528883775, 30405943383200, 47129212243960, 67327446062800, 88749815264600, 108043253365600, 121548660036300, 126410606437752, 121548660036300, 108043253365600, 88749815264600, 67327446062800, 47129212243960, 30405943383200, 18053528883775, 9847379391150, 4923689695575, 2250829575120, 937845656300, 354860518600, 121399651100, 37353738800, 10272278170, 2505433700, 536878650, 99884400, 15890700, 2118760, 230300, 19600, 1225, 50, 1 }, { 1, 51, 1275, 20825, 249900, 2349060, 18009460, 115775100, 636763050, 3042312350, 12777711870, 47626016970, 158753389900, 476260169700, 1292706174900, 3188675231420, 7174519270695, 14771069086725, 27900908274925, 48459472266975, 77535155627160, 114456658306760, 156077261327400, 196793068630200, 229591913401900, 247959266474052, 247959266474052, 229591913401900, 196793068630200, 156077261327400, 114456658306760, 77535155627160, 48459472266975, 27900908274925, 14771069086725, 7174519270695, 3188675231420, 1292706174900, 476260169700, 158753389900, 47626016970, 12777711870, 3042312350, 636763050, 115775100, 18009460, 2349060, 249900, 20825, 1275, 51, 1 }, { 1, 52, 1326, 22100, 270725, 2598960, 20358520, 133784560, 752538150, 3679075400, 15820024220, 60403728840, 206379406870, 635013559600, 1768966344600, 4481381406320, 10363194502115, 21945588357420, 42671977361650, 76360380541900, 125994627894135, 191991813933920, 270533919634160, 352870329957600, 426384982032100, 477551179875952, 495918532948104, 477551179875952, 426384982032100, 352870329957600, 270533919634160, 191991813933920, 125994627894135, 76360380541900, 42671977361650, 21945588357420, 10363194502115, 4481381406320, 1768966344600, 635013559600, 206379406870, 60403728840, 15820024220, 3679075400, 752538150, 133784560, 20358520, 2598960, 270725, 22100, 1326, 52, 1 }, { 1, 53, 1378, 23426, 292825, 2869685, 22957480, 154143080, 886322710, 4431613550, 19499099620, 76223753060, 266783135710, 841392966470, 2403979904200, 6250347750920, 14844575908435, 32308782859535, 64617565719070, 119032357903550, 202355008436035, 317986441828055, 462525733568080, 623404249591760, 779255311989700, 903936161908052, 973469712824056, 973469712824056, 903936161908052, 779255311989700, 623404249591760, 462525733568080, 317986441828055, 202355008436035, 119032357903550, 64617565719070, 32308782859535, 14844575908435, 6250347750920, 2403979904200, 841392966470, 266783135710, 76223753060, 19499099620, 4431613550, 886322710, 154143080, 22957480, 2869685, 292825, 23426, 1378, 53, 1 }, { 1, 54, 1431, 24804, 316251, 3162510, 25827165, 177100560, 1040465790, 5317936260, 23930713170, 95722852680, 343006888770, 1108176102180, 3245372870670, 8654327655120, 21094923659355, 47153358767970, 96926348578605, 183649923622620, 321387366339585, 520341450264090, 780512175396135, 1085929983159840, 1402659561581460, 1683191473897752, 1877405874732108, 1946939425648112, 1877405874732108, 1683191473897752, 1402659561581460, 1085929983159840, 780512175396135, 520341450264090, 321387366339585, 183649923622620, 96926348578605, 47153358767970, 21094923659355, 8654327655120, 3245372870670, 1108176102180, 343006888770, 95722852680, 23930713170, 5317936260, 1040465790, 177100560, 25827165, 3162510, 316251, 24804, 1431, 54, 1 }, { 1, 55, 1485, 26235, 341055, 3478761, 28989675, 202927725, 1217566350, 6358402050, 29248649430, 119653565850, 438729741450, 1451182990950, 4353548972850, 11899700525790, 29749251314475, 68248282427325, 144079707346575, 280576272201225, 505037289962205, 841728816603675, 1300853625660225, 1866442158555975, 2488589544741300, 3085851035479212, 3560597348629860, 3824345300380220, 3824345300380220, 3560597348629860, 3085851035479212, 2488589544741300, 1866442158555975, 1300853625660225, 841728816603675, 505037289962205, 280576272201225, 144079707346575, 68248282427325, 29749251314475, 11899700525790, 4353548972850, 1451182990950, 438729741450, 119653565850, 29248649430, 6358402050, 1217566350, 202927725, 28989675, 3478761, 341055, 26235, 1485, 55, 1 }, { 1, 56, 1540, 27720, 367290, 3819816, 32468436, 231917400, 1420494075, 7575968400, 35607051480, 148902215280, 558383307300, 1889912732400, 5804731963800, 16253249498640, 41648951840265, 97997533741800, 212327989773900, 424655979547800, 785613562163430, 1346766106565880, 2142582442263900, 3167295784216200, 4355031703297275, 5574440580220512, 6646448384109072, 7384942649010080, 7648690600760440, 7384942649010080, 6646448384109072, 5574440580220512, 4355031703297275, 3167295784216200, 2142582442263900, 1346766106565880, 785613562163430, 424655979547800, 212327989773900, 97997533741800, 41648951840265, 16253249498640, 5804731963800, 1889912732400, 558383307300, 148902215280, 35607051480, 7575968400, 1420494075, 231917400, 32468436, 3819816, 367290, 27720, 1540, 56, 1 }, { 1, 57, 1596, 29260, 395010, 4187106, 36288252, 264385836, 1652411475, 8996462475, 43183019880, 184509266760, 707285522580, 2448296039700, 7694644696200, 22057981462440, 57902201338905, 139646485582065, 310325523515700, 636983969321700, 1210269541711230, 2132379668729310, 3489348548829780, 5309878226480100, 7522327487513475, 9929472283517787, 12220888964329584, 14031391033119152, 15033633249770520, 15033633249770520, 14031391033119152, 12220888964329584, 9929472283517787, 7522327487513475, 5309878226480100, 3489348548829780, 2132379668729310, 1210269541711230, 636983969321700, 310325523515700, 139646485582065, 57902201338905, 22057981462440, 7694644696200, 2448296039700, 707285522580, 184509266760, 43183019880, 8996462475, 1652411475, 264385836, 36288252, 4187106, 395010, 29260, 1596, 57, 1 }, { 1, 58, 1653, 30856, 424270, 4582116, 40475358, 300674088, 1916797311, 10648873950, 52179482355, 227692286640, 891794789340, 3155581562280, 10142940735900, 29752626158640, 79960182801345, 197548686920970, 449972009097765, 947309492837400, 1847253511032930, 3342649210440540, 5621728217559090, 8799226775309880, 12832205713993575, 17451799771031262, 22150361247847371, 26252279997448736, 29065024282889672, 30067266499541040, 29065024282889672, 26252279997448736, 22150361247847371, 17451799771031262, 12832205713993575, 8799226775309880, 5621728217559090, 3342649210440540, 1847253511032930, 947309492837400, 449972009097765, 197548686920970, 79960182801345, 29752626158640, 10142940735900, 3155581562280, 891794789340, 227692286640, 52179482355, 10648873950, 1916797311, 300674088, 40475358, 4582116, 424270, 30856, 1653, 58, 1 }, { 1, 59, 1711, 32509, 455126, 5006386, 45057474, 341149446, 2217471399, 12565671261, 62828356305, 279871768995, 1119487075980, 4047376351620, 13298522298180, 39895566894540, 109712808959985, 277508869722315, 647520696018735, 1397281501935165, 2794563003870330, 5189902721473470, 8964377427999630, 14420954992868970, 21631432489303455, 30284005485024837, 39602161018878633, 48402641245296107, 55317304280338408, 59132290782430712, 59132290782430712, 55317304280338408, 48402641245296107, 39602161018878633, 30284005485024837, 21631432489303455, 14420954992868970, 8964377427999630, 5189902721473470, 2794563003870330, 1397281501935165, 647520696018735, 277508869722315, 109712808959985, 39895566894540, 13298522298180, 4047376351620, 1119487075980, 279871768995, 62828356305, 12565671261, 2217471399, 341149446, 45057474, 5006386, 455126, 32509, 1711, 59, 1 }, { 1, 60, 1770, 34220, 487635, 5461512, 50063860, 386206920, 2558620845, 14783142660, 75394027566, 342700125300, 1399358844975, 5166863427600, 17345898649800, 53194089192720, 149608375854525, 387221678682300, 925029565741050, 2044802197953900, 4191844505805495, 7984465725343800, 14154280149473100, 23385332420868600, 36052387482172425, 51915437974328292, 69886166503903470, 88004802264174740, 103719945525634515, 114449595062769120, 118264581564861424, 114449595062769120, 103719945525634515, 88004802264174740, 69886166503903470, 51915437974328292, 36052387482172425, 23385332420868600, 14154280149473100, 7984465725343800, 4191844505805495, 2044802197953900, 925029565741050, 387221678682300, 149608375854525, 53194089192720, 17345898649800, 5166863427600, 1399358844975, 342700125300, 75394027566, 14783142660, 2558620845, 386206920, 50063860, 5461512, 487635, 34220, 1770, 60, 1 }, { 1, 61, 1830, 35990, 521855, 5949147, 55525372, 436270780, 2944827765, 17341763505, 90177170226, 418094152866, 1742058970275, 6566222272575, 22512762077400, 70539987842520, 202802465047245, 536830054536825, 1312251244423350, 2969831763694950, 6236646703759395, 12176310231149295, 22138745874816900, 37539612570341700, 59437719903041025, 87967825456500717, 121801604478231762, 157890968768078210, 191724747789809255, 218169540588403635, 232714176627630544, 232714176627630544, 218169540588403635, 191724747789809255, 157890968768078210, 121801604478231762, 87967825456500717, 59437719903041025, 37539612570341700, 22138745874816900, 12176310231149295, 6236646703759395, 2969831763694950, 1312251244423350, 536830054536825, 202802465047245, 70539987842520, 22512762077400, 6566222272575, 1742058970275, 418094152866, 90177170226, 17341763505, 2944827765, 436270780, 55525372, 5949147, 521855, 35990, 1830, 61, 1 }, { 1, 62, 1891, 37820, 557845, 6471002, 61474519, 491796152, 3381098545, 20286591270, 107518933731, 508271323092, 2160153123141, 8308281242850, 29078984349975, 93052749919920, 273342452889765, 739632519584070, 1849081298960175, 4282083008118300, 9206478467454345, 18412956934908690, 34315056105966195, 59678358445158600, 96977332473382725, 147405545359541742, 209769429934732479, 279692573246309972, 349615716557887465, 409894288378212890, 450883717216034179, 465428353255261088, 450883717216034179, 409894288378212890, 349615716557887465, 279692573246309972, 209769429934732479, 147405545359541742, 96977332473382725, 59678358445158600, 34315056105966195, 18412956934908690, 9206478467454345, 4282083008118300, 1849081298960175, 739632519584070, 273342452889765, 93052749919920, 29078984349975, 8308281242850, 2160153123141, 508271323092, 107518933731, 20286591270, 3381098545, 491796152, 61474519, 6471002, 557845, 37820, 1891, 62, 1 }, { 1, 63, 1953, 39711, 595665, 7028847, 67945521, 553270671, 3872894697, 23667689815, 127805525001, 615790256823, 2668424446233, 10468434365991, 37387265592825, 122131734269895, 366395202809685, 1012974972473835, 2588713818544245, 6131164307078475, 13488561475572645, 27619435402363035, 52728013040874885, 93993414551124795, 156655690918541325, 244382877832924467, 357174975294274221, 489462003181042451, 629308289804197437, 759510004936100355, 860778005594247069, 916312070471295267, 916312070471295267, 860778005594247069, 759510004936100355, 629308289804197437, 489462003181042451, 357174975294274221, 244382877832924467, 156655690918541325, 93993414551124795, 52728013040874885, 27619435402363035, 13488561475572645, 6131164307078475, 2588713818544245, 1012974972473835, 366395202809685, 122131734269895, 37387265592825, 10468434365991, 2668424446233, 615790256823, 127805525001, 23667689815, 3872894697, 553270671, 67945521, 7028847, 595665, 39711, 1953, 63, 1 } };
#include <stdint.h>
#include <stdlib.h>

void free_vector(vector_t x)
{
    free(x);
}

vector_t init_vector(int n_rows)
{
    return (vector_t)exfes_calloc(n_rows, sizeof(int), 10);
}

pck_vector_t pack(int n, const vector_t v)
{
    pck_vector_t r = 0;

    for (int i = n - 1; i >= 0; i--) {
        r = r << 1;
        r |= v[i] & 1;
    }

    return r;
}

uint64_t to_gray(uint64_t i)
{
    return i ^ (i >> 1);
}

pck_vector_t packed_eval(LUT_t LUT, int n, pck_vector_t *F, uint64_t i)
{
    return packed_eval_deg_2(LUT, n, F, i);
}

// Compute the internal state, with the last k variables specialized
// to the value i (in binary)
// This function assumes that A = F.
// this function has been partially auto-generated by the gen_specialization
// part of the python code generator
void variables_specialization(LUT_t LUT, int n, int d, pck_vector_t *A, int k, int i)
{

    pck_vector_t v[k];
    for (int l = 0; l < k; l++) {
        v[l] = 0;
        if (i & 1) {
            v[l] = 0xffffffff;
        }
        i = i >> 1;
    }

    // updates degree-0 term with degree-1 terms
    for (int a = 0; a < k; a++) {
        A[0] ^= A[idx_1(LUT, n - k + a)] & v[a];
    }

    if (d < 2) {
        return;
    }

    // updates degree-0 term with degree-2 terms
    for (int b = 0; b < k; b++) {
        for (int a = 0; a < b; a++) {
            A[0] ^= A[idx_2(LUT, n - k + a, n - k + b)] & (v[a] & v[b]);
        }
    }

    // updates degree-1 term with degree-2 terms
    for (int idx_0 = 0; idx_0 < n - k; idx_0++) {
        for (int a = 0; a < k; a++) {
            A[idx_1(LUT, idx_0)] ^= A[idx_2(LUT, idx_0, n - k + a)] & v[a];
        }
    }
}
#include <stdint.h>
#include <stdlib.h>

int C(int n, int m)
{
    if (m == 0) {
        return 1;
    } else if (m == 1) {
        return n;
    } else if (m == 2) {
        return n * (n - 1) >> 1;
    } else {
        return 0;
    }
}

int M(uint64_t startPointHigh, uint64_t startPointLow, int index)
{
    if (index < 64) {
        return (startPointLow >> index) & 1;
    } else {
        return (startPointHigh >> (index - 64)) & 1;
    }
}

void freeEqs(int ***Eqs, int i, int j)
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

int initEqs(int n, int e, int ****EqsPtr)
{
    EqsPtr[0] = (int ***)exfes_calloc(e, sizeof(int **), 10);
    if (!EqsPtr[0]) {
        return -4;
    }
    for (int i = 0; i < e; i++) {
        EqsPtr[0][i] = (int **)exfes_calloc(3, sizeof(int *), 10);
        if (!EqsPtr[0][i]) {
            freeEqs(EqsPtr[0], i, -1);
            return -4;
        }
        for (int j = 0; j < 3; j++) {
            EqsPtr[0][i][j] = (int *)exfes_calloc(C(n, j), sizeof(int), 10);
            if (!EqsPtr[0][i][j]) {
                freeEqs(EqsPtr[0], i, j);
                return -4;
            }
        }
    }
    return 0;
}

void Transform_Data_Structure(int n, int e, const uint8_t *coefficientsMatrix, int ***Eqs)
{
    uint64_t offset = 0;
    for (int i = 0; i < e; i++) {
        for (int j = 0; j < C(n, 2); j++) {
            Eqs[i][2][j] = (int)coefficientsMatrix[offset + j];
        }
        offset += C(n, 2);
        for (int j = 0; j < n; j++) {
            Eqs[i][1][j] = (int)coefficientsMatrix[offset + j];
        }
        offset += n;
        Eqs[i][0][0] = (int)coefficientsMatrix[offset];
        offset += 1;
    }
}

void Merge_Solution(struct exfes_context *p, uint64_t count, uint64_t *Sol)
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

int exfes(uint32_t numFixedVariables, uint32_t numVariables, uint32_t numEquations, uint64_t startPointHigh, uint64_t startPointLow, const uint8_t *coefficientsMatrix, bool (*shouldAbortNow)(void), uint64_t *solutionHigh, uint64_t *solutionLow)
{
    int m = numFixedVariables;
    int n = numVariables;
    int e = numEquations;

    if (numVariables == 0 || numVariables >= 127) {
        return -3;
    }
    if (numVariables - numFixedVariables <= 0 || numVariables - numFixedVariables >= 64) {
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

    struct exfes_context exfes_ctx;
    exfes_ctx.mcopy = m;
    exfes_ctx.ncopy = n;
    exfes_ctx.solm = 0;
    exfes_ctx.SolCount = 0;
    exfes_ctx.startPointHigh = startPointHigh;
    exfes_ctx.startPointLow = startPointLow;
    exfes_ctx.solutionHigh = solutionHigh;
    exfes_ctx.solutionLow = solutionLow;
    exfes_ctx.shouldAbortNow = shouldAbortNow;

    // Make a copy of EqsUnmask for masking.
    int ***Eqs;
    if (initEqs(n, e, &Eqs) != 0) {
        return -4;
    }
    Transform_Data_Structure(n, e, coefficientsMatrix, Eqs);

    // Mask Eqs for a random start point.
    for (int i = 0; i < e; i++) {
        for (int j = 0; j < n; j++) {
            Eqs[i][0][0] ^= Eqs[i][1][j] & M(startPointHigh, startPointLow, j);
        }
        int offset = 0;
        for (int j = 0; j < n - 1; j++) {
            for (int k = j + 1; k < n; k++) {
                Eqs[i][0][0] ^= Eqs[i][2][offset] & M(startPointHigh, startPointLow, j) & M(startPointHigh, startPointLow, k);
                Eqs[i][1][j] ^= Eqs[i][2][offset] & M(startPointHigh, startPointLow, k);
                Eqs[i][1][k] ^= Eqs[i][2][offset] & M(startPointHigh, startPointLow, j);
                offset += 1;
            }
        }
    }

    // Make a copy of Eqs for evaluating fixed variables.
    int ***EqsCopy;
    if (initEqs(n, e, &EqsCopy) != 0) {
        freeEqs(Eqs, e, -1);
        return -4;
    }

    // Partition problem into (1<<n_fixed) sub_problems.
    int p = n - m;
    int npartial;
    int fixvalue;
    for (exfes_ctx.solm = 0; exfes_ctx.solm < (uint64_t)1 << m; exfes_ctx.solm++) {
        // Initialize npartial and EqsCopy.
        npartial = n;
        for (int i = 0; i < e; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < C(n, j); k++) {
                    EqsCopy[i][j][k] = Eqs[i][j][k];
                }
            }
        }

        // Fix m variables.
        while (npartial != p) {
            fixvalue = (exfes_ctx.solm >> (n - npartial)) & 1;
            for (int i = 0; i < e; i++) {
                // Fix a variable.
                for (int j = 0; j < npartial - 1; j++) {
                    EqsCopy[i][1][j + 1] ^= EqsCopy[i][2][j] & fixvalue;
                }
                EqsCopy[i][0][0] ^= EqsCopy[i][1][0] & fixvalue;
                // Shrink EqsCopy.
                for (int j = 0; j < npartial - 1; j++) {
                    EqsCopy[i][1][j] = EqsCopy[i][1][j + 1];
                }
                for (int j = 0; j < C(npartial - 1, 2); j++) {
                    EqsCopy[i][2][j] = EqsCopy[i][2][j + npartial - 1];
                }
            }
            npartial -= 1;
        }

        if (exhaustive_search_wrapper(npartial, e, 2, EqsCopy, &exfes_ctx) != 0) {
            freeEqs(EqsCopy, e, -1);
            freeEqs(Eqs, e, -1);
            return -4;
        }

        // Determine to early abort or not.
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
#include <stdint.h>
#include <stdlib.h>

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
#include <stdlib.h>

void *exfes_calloc(size_t num, size_t size, size_t max_num_retries)
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
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

void Merge_Solution(struct exfes_context *p, uint64_t count, uint64_t *Sol);  // quick hack

void next_set(int n, int d, int *set)
{
    if (!d) {
        return;
    }
    set[0] += 1;
    if (set[0] == n) {
        next_set(n - 1, d - 1, &set[1]);
        if (d > 0) {
            set[0] = set[1] + 1;
        }
    }
}

int convert_input_equations(const int n, const int degree, int from, int to, int ***coeffs, idx_lut_t *idx_LUT, pck_vector_t *F)
{
    vector_t x = init_vector(to - from);  // this is used to pack the equations in memory words
    if (!x) {
        return -4;
    }
    int set[n];  // represent the monomial `m` enumerated below
    for (int j = 0; j < n; j++) {
        set[j] = -1;
    }
    for (int d = 0; d < degree + 1; d++) {  // collect degree-d terms
        for (int j = 0; j < d; j++) {
            set[j] = d - 1 - j;
        }
        for (uint64_t m = 0; m < binomials[n][d]; m++) {  // iterates over all monomials of degree d
            // loop invariant: `set` describes the m-th monomial
            for (int e = from; e < to; e++) {  // skim through all the equations
                x[e - from] = coeffs[e][d][m];
            }
            F[set2int(idx_LUT, set)] = pack(to - from, x);
            next_set(n, n, &set[0]);  // maintain invariant
        }
    }
    free_vector(x);
    return 0;
}

// this callback is used when there are more than 32 equations
int solution_tester(wrapper_state_t *wrapper_state_ptr, uint64_t size, uint64_t *n_solutions)
{
    for (uint64_t i = 0; i < size; i++) {
        uint64_t current_solution = n_solutions[i];
        int is_correct = 1;
        int j = 0;
        while (is_correct && j < wrapper_state_ptr->n_batches) {
            if (packed_eval(wrapper_state_ptr->testing_LUT->LUT, wrapper_state_ptr->n, wrapper_state_ptr->G[j], current_solution) != 0) {
                is_correct = 0;
            }
            j++;
        }
        if (is_correct) {
            int num_correct_solutions = 1;
            uint64_t corrects_solutions[1];
            corrects_solutions[0] = current_solution;
            Merge_Solution(wrapper_state_ptr->exfes_ctx_ptr, num_correct_solutions, corrects_solutions);
            return 1;
        }
    }
    return 0;
}

int exhaustive_search_wrapper(const int n, int n_eqs, const int degree, int ***coeffs, struct exfes_context *exfes_ctx_ptr)
{
    const uint64_t N = n_monomials(n, degree);
    const int word_size = 16;

    idx_lut_t *idx_LUT = init_deginvlex_LUT(n, degree);
    if (!idx_LUT) {
        return -4;
    }

    pck_vector_t *F = (pck_vector_t *)exfes_calloc(1, N * sizeof(pck_vector_t), 10);
    if (!F) {
        free_LUT(idx_LUT);
        return -4;
    }

    if (convert_input_equations(n, degree, 0, word_size, coeffs, idx_LUT, F) != 0) {
        free(F);
        free_LUT(idx_LUT);
        return -4;
    }

    int n_batches = n_eqs / word_size;
    if (n_eqs % word_size) {
        n_batches++;
    }

    pck_vector_t **G = (pck_vector_t **)exfes_calloc(n_batches - 1, sizeof(pck_vector_t *), 10);
    if (!G) {
        free(F);
        free_LUT(idx_LUT);
        return -4;
    }

    int should_free_G_count = -1;
    for (int i = 1; i < n_batches; i++) {
        G[i - 1] = (pck_vector_t *)exfes_calloc(N, sizeof(pck_vector_t), 10);
        if (!G[i - 1]) {
            should_free_G_count = i - 1;
            break;
        }
        if (convert_input_equations(n, degree, word_size * i, min(n_eqs, word_size * (i + 1)), coeffs, idx_LUT, G[i - 1]) != 0) {
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
        free_LUT(idx_LUT);
        return -4;
    }

    wrapper_state_t wrapper_state;
    wrapper_state.n = n;
    wrapper_state.degree = degree;
    wrapper_state.n_batches = n_batches - 1;
    wrapper_state.G = G;
    wrapper_state.testing_LUT = idx_LUT;
    wrapper_state.exfes_ctx_ptr = exfes_ctx_ptr;

    exhaustive_ia32_deg_2(idx_LUT->LUT, n, F, &wrapper_state);

    for (int i = n_batches - 1; i >= 1; i--) {
        free(G[i - 1]);
    }
    free(G);
    free(F);
    free_LUT(idx_LUT);

    return 0;
}
