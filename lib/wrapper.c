#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "binomials.h"
#include "fes.h"
#include "my_memory.h"

void init_settings(wrapper_settings_t *result)
{
    result->word_size = 32;
    result->algorithm = ALGO_AUTO;
    result->algo_auto_degree_bound = 10;

    result->algo_enum_self_tune = 1;

#ifdef HAVE_SSE2
    result->algo_enum_use_sse = 1;
#else
    result->algo_enum_use_sse = 0;
#endif

    result->verbose = 0;
}

void choose_settings(wrapper_settings_t *s, int n, int n_eqs, int degree)
{
    (void)n_eqs;
    // choose algorithm
    // assert(n_eqs > 0);
    if (s->algorithm == ALGO_AUTO) {
        if (degree < s->algo_auto_degree_bound) {
            s->algorithm = ALGO_ENUMERATION;
        } else {
            s->algorithm = ALGO_FFT;
        }

        if (s->algorithm == ALGO_ENUMERATION && s->algo_enum_self_tune) {
            if (degree == 2) {
                s->word_size = 16;
            }
            if (n < SIMD_CHUNK_SIZE + 2) {
                s->algo_enum_use_sse = 0;
            }
        }
    }
}

// ---------- convert the input given by the SAGE interface to the right format
void next_set(int n, int d, int *set)
{
    if (d == 0)
        return;
    set[0] += 1;
    if (set[0] == n) {
        next_set(n - 1, d - 1, &set[1]);
        if (d > 0)
            set[0] = set[1] + 1;
    }
}

//  assumes that F (the target array) is already allocated
int convert_input_equations(const int n, const int degree, int from, int to, int ***coeffs, idx_lut_t *idx_LUT, pck_vector_t *F)
{

    // assert(to-from <= (int) (8*sizeof(pck_vector_t)));
    vector_t x = init_vector(
            to - from);  // this is used to pack the equations in memory words
    if (x == NULL)
        return -4;

    int set[n];  // represent the monomial `m` enumerated below
    for (int j = 0; j < n; j++) {
        set[j] = -1;
    }
    for (int d = 0; d < degree + 1; d++) {  // collect degree-d terms
        for (int j = 0; j < d; j++) {
            set[j] = d - 1 - j;
        }
        for (uint64_t m = 0; m < binomials[n][d];
                m++) {  // iterates over all monomials of degree d
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

// --------------------------------------------------------------------------------------------------

// this callback is used when there are more than 32 equations
int solution_tester(void *_state, uint64_t size, uint64_t *n_solutions)
{
    wrapper_state_t *state = (wrapper_state_t *)_state;

    uint64_t corrects_solutions[1];
    uint64_t current_solution;
    int index_correct_solution = 0;
    int is_correct;
    int j;

    for (uint64_t i = 0; i < size; i++) {
        is_correct = 1;
        j = 0;
        current_solution = n_solutions[i];
        while (is_correct && j < (state->n_batches)) {
            if (packed_eval(state->testing_LUT->LUT, state->n, state->degree,
                        state->G[j], current_solution)
                    != 0)
                is_correct = 0;
            j++;
        }
        if (is_correct) {
            corrects_solutions[0] = current_solution;
            index_correct_solution++;
            break;  // Early abort.
        }
    }

    int answer_found = 0;

    if (index_correct_solution) {
        // report solution to the actual callback, and ask whether it wants to keep
        // going
        answer_found = (*(state->callback))(
                state->callback_state, index_correct_solution, corrects_solutions);
    }

    return answer_found;
}

// ------------------------------------------------
void enumeration_wrapper(LUT_t LUT, int n, int d, pck_vector_t *F, solution_callback_t callback, void *callback_state, wrapper_settings_t *settings)
{
    (void)d;
    exhaustive_ia32_deg_2(LUT, n, F, callback, callback_state, settings->verbose);
}

// -------------------------------------

int exhaustive_search_wrapper(const int n, int n_eqs, const int degree, int ***coeffs, solution_callback_t callback, void *callback_state)
{

    wrapper_settings_t settings[1];
    init_settings(settings);
    choose_settings(settings, n, n_eqs, degree);

    // bool must_free_tester_state = false;
    const uint64_t N = n_monomials(n, degree);
    //  int enumerated_equations = 128 >> (T);

    // --------- allocate/initialize some of our data structures
    pck_vector_t *F = NULL;
    idx_lut_t *idx_LUT = NULL;
    size_t F_size = -1;

    bool should_free_LUT = 0;
    idx_LUT = init_deginvlex_LUT(n, degree);
    if (idx_LUT == NULL) {
        return -4;
    }
    should_free_LUT = 1;
    F_size = N;

    bool should_free_F = 0;
    F = exfes_calloc(1, F_size * sizeof(pck_vector_t), 10);
    if (F == NULL) {
        if (should_free_LUT)
            free_LUT(idx_LUT);
        return -4;
    }
    should_free_F = 1;

    // ---------- deal where the case where there is more equations than what the
    // kernel(s) deals with

    pck_vector_t **G = NULL;

    wrapper_state_t *tester_state = NULL;

    // if there are more equations that what we can enumerate simultaneously,
    // we just deal with the first `enumerated_equations`, and then check
    // any eventual solutions of these against the remaining equations

    // we split the equations into "batches" of `settings->word_size` each
    int n_batches = n_eqs / settings->word_size;
    if ((n_eqs % settings->word_size) > 0) {
        n_batches++;
    }

    // the first batch goes into the enumeration code
    // prepare the input for the enumeration
    if (convert_input_equations(n, degree, 0, settings->word_size, coeffs,
                idx_LUT, F)
            != 0) {
        if (should_free_F)
            free(F);
        if (should_free_LUT)
            free_LUT(idx_LUT);
        return -4;
    }

    // the next batches will be used by the tester. They must be in deginvlex
    // order
    idx_lut_t *testing_LUT = idx_LUT;

    bool should_free_G = 0;
    G = exfes_calloc(n_batches - 1, sizeof(pck_vector_t *), 10);
    if (G == NULL) {
        if (should_free_F)
            free(F);
        if (should_free_LUT)
            free_LUT(idx_LUT);
        return -4;
    }
    should_free_G = 1;

    int should_free_G_count = -1;
    for (int i = 1; i < n_batches; i++) {
        G[i - 1] = exfes_calloc(N, sizeof(pck_vector_t), 10);
        if (G[i - 1] == NULL) {
            should_free_G_count = i - 1;
            break;
        }
        if (convert_input_equations(n, degree, settings->word_size * i,
                min(n_eqs, settings->word_size * (i + 1)), coeffs,
                testing_LUT, G[i - 1]) != 0) {
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
        if (should_free_G)
            free(G);
        if (should_free_F)
            free(F);
        if (should_free_LUT)
            free_LUT(idx_LUT);
        return -4;
    }

    // the "tester" needs some internal state
    bool should_free_tester_state = 0;
    if ((tester_state = exfes_calloc(1, sizeof(wrapper_state_t), 10)) == NULL) {
        if (should_free_G) {
            for (int i = n_batches - 1; i >= 1; i--) {
                free(G[i - 1]);
            }
            free(G);
        }
        if (should_free_F)
            free(F);
        if (should_free_LUT)
            free_LUT(idx_LUT);
        return -4;
    }
    should_free_tester_state = 1;

    tester_state->n = n;
    tester_state->degree = degree;
    tester_state->n_batches = n_batches - 1;
    tester_state->G = G;
    tester_state->testing_LUT = testing_LUT;

    tester_state->callback = callback;
    tester_state->callback_state = callback_state;
    // must_free_tester_state = true;

    callback = solution_tester;
    callback_state = (void *)tester_state;

    // ------------ start actual computation

    enumeration_wrapper(idx_LUT->LUT, n, degree, F, callback, callback_state,
            settings);

    // ----------- clean up

    if (should_free_tester_state)
        free(tester_state);
    if (should_free_G) {
        for (int i = n_batches - 1; i >= 1; i--) {
            free(G[i - 1]);
        }
        free(G);
    }
    if (should_free_F)
        free(F);
    if (should_free_LUT)
        free_LUT(idx_LUT);

    return 0;
}
