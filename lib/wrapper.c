#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "binomials.h"
#include "fes.h"
#include "my_memory.h"

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
int solution_tester(wrapper_state_t *state, uint64_t size, uint64_t *n_solutions)
{
    for (uint64_t i = 0; i < size; i++) {
        uint64_t current_solution = n_solutions[i];
        int is_correct = 1;
        int j = 0;
        while (is_correct && j < state->n_batches) {
            if (packed_eval(state->testing_LUT->LUT, state->n, state->G[j], current_solution) != 0) {
                is_correct = 0;
            }
            j++;
        }
        if (is_correct) {
            int num_correct_solutions = 1;
            uint64_t corrects_solutions[1];
            corrects_solutions[0] = current_solution;
            Merge_Solution(state->callback_state, num_correct_solutions, corrects_solutions);
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

    pck_vector_t *F = exfes_calloc(1, N * sizeof(pck_vector_t), 10);
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

    pck_vector_t **G = exfes_calloc(n_batches - 1, sizeof(pck_vector_t *), 10);
    if (!G) {
        free(F);
        free_LUT(idx_LUT);
        return -4;
    }

    int should_free_G_count = -1;
    for (int i = 1; i < n_batches; i++) {
        G[i - 1] = exfes_calloc(N, sizeof(pck_vector_t), 10);
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

    wrapper_state_t solution_tester_state;
    solution_tester_state.n = n;
    solution_tester_state.degree = degree;
    solution_tester_state.n_batches = n_batches - 1;
    solution_tester_state.G = G;
    solution_tester_state.testing_LUT = idx_LUT;
    solution_tester_state.callback_state = exfes_ctx_ptr;

    exhaustive_ia32_deg_2(idx_LUT->LUT, n, F, &solution_tester_state);

    for (int i = n_batches - 1; i >= 1; i--) {
        free(G[i - 1]);
    }
    free(G);
    free(F);
    free_LUT(idx_LUT);

    return 0;
}
