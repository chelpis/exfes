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
//      init_lex_LUT
//      print_idx_LUT
//      print_idx
//      free_LUT
//      set2int
//      int2set
//      idx_convert
//      n_monomials

#ifndef IDX_LUT_H
#define IDX_LUT_H

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

// this function initialize another indexing scheme, that turns any
// boolean monomial of GF(2)[x_1,...,x_n] (without degree
// restriction), into an integer between 0 and 2^n-1
const idx_lut_t *init_lex_LUT(int n);

// print the lookup table. Mainly useful for debugging
void print_idx_LUT(const idx_lut_t *table);

// print a set of indices.  Mainly useful for debugging
void print_idx(int d, int *set);

// reclaim the memory used by the lookup table
void free_LUT(const idx_lut_t *tab);

// generic (and somewhat slow) function that computes an integer
// index given a set of indices. It is understood that `set` is an
// array of at least `d` entries (where `d` was the argument given to
// the init_idx_LUT function). If d=5, then the monomial [1,4,6,7,8]
// should be represented by the array {8,7,6,4,1}, and the monomial
// [0,4,6] should be represented by {6,4,0,-1,-1}.
LUT_int_t set2int(const idx_lut_t *table, int *set);

void int2set(const idx_lut_t *table, LUT_int_t index, int *set);

LUT_int_t idx_convert(idx_lut_t *table_from, idx_lut_t *table_to, LUT_int_t i);

uint64_t n_monomials(int n, int d);

#endif
