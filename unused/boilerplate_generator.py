import sys
import string
import idx_LUT
import itertools
import get_idx_list
from copy import copy

def popcount(n):
    b=0
    while n>0:
        b += 1
        n &= n-1
    return b


def data_reference(array, indices):
    if indices == []:
        return array + "[ 0 ]"
    return "{array}[ idx_{i}(LUT, {stuff}) ]".format(array=array, i=len(indices), stuff=", ".join( indices ))
    

# generates the initialisation of the derivatives
def gen_init_code(degree):
    idx = [ 'i{0}'.format(j) for j in range(degree+1) ]
    print('  // computes the derivatives required by the enumeration kernel up to degree {d}'.format(d=degree))
    print('  // this is done in-place, meaning that if "F" described the coefficients of the')
    print('  // polynomials before, then afterwards, they describe the derivatives')
    print('')

    for updatee_degree in range(1, degree):
        for updater_degree in range(updatee_degree+1, min(degree,2*updatee_degree)+1):
            print('  // here, degree-{0} terms are affected by degree-{1} terms'.format(updatee_degree, updater_degree))
            print('  for(int {idx}={start}; {idx}<n; {idx}++) {{'.format(idx=idx[updatee_degree-1], start=updatee_degree))
            for i in range(updatee_degree-2, -1, -1):
                print('{pad}  for(int {idx}={start}; {idx}<{prev_idx}; {idx}++) {{'.format(pad='  '*(updatee_degree-i), \
                                                                               idx=idx[i], start=i, prev_idx=idx[i+1]))
            updatee = data_reference('F', idx[:updatee_degree])
            conditions = [ idx[0] + "  != 0 " ] + list(map( lambda i: idx[i] + "-1 > " + idx[i-1], range(1, updatee_degree)))

            for subset in itertools.combinations(range(updatee_degree), updater_degree-updatee_degree):
                update_condition = " && ".join( map(lambda i:conditions[i], subset) )
                updater_idx = idx[:updatee_degree]
                for foo,i in enumerate(subset):
                    updater_idx.insert(i+foo, idx[i]+"-1")
                updater = data_reference('F', updater_idx)
                print('{pad}    if ({cond}) {lhs} ^= {rhs};'.format(pad='  '*updatee_degree, cond=update_condition, lhs=updatee, rhs=updater))

            print('  {closing}'.format(closing='}'*updatee_degree))
            print('')
    print ('')
    print ('')



# generate the "steps" macros
def gen_steps(degree,simd=False, T=2):
    idx = [ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'] + [ "i{0}".format(j) for j in range(degree) ]

    for w in range(degree+1):
        args = idx[:w]
        
        print('#define STEP_{w}({args}) {{ \\'.format(w=w, args=",".join( args + ["i"] )))
        for i in range(w-1,0,-1):
            print('  F[ {LHS} ] ^= F [ {RHS} ]; \\'.format(LHS=idx[i-1], RHS=idx[i]))
        if w > 0:
            print('  F[ 0 ] ^= F [ {RHS} ]; \\'.format(RHS=idx[0]))
        if simd:
            print('  const simd_t Mask = _mm_cmpeq_epi{}(F[ 0 ], simd_zero); \\'.format(128>>T))
            print('  const int mask = _mm_movemask_epi8(Mask);  \\')
            print('  if (unlikely(mask)) { \\')
        else:
            print('  if (unlikely(F[ 0 ] == 0)) { \\')
        print('      solution_buffer[n_solutions_found].int_idx = i; \\')
        if simd:
            print('      solution_buffer[n_solutions_found].mask = mask; \\')
        else:
            print('      solution_buffer[n_solutions_found].mask = 0x000f; \\')
        print('      n_solutions_found++; \\')
        print('   }\\')
        print('}')
        print('')



def b_k(k, n):
    result = 0
    for i in range(k):
        while (n & 0x01) == 0:
            if n == 0:
                return -1
            n >>= 1
            result += 1
        n >>= 1
        result += 1
    return result-1


def gen_specialization(degree, T):
    print('  // we want to specialize each one of the {T2} SIMD-thread to a combination of the last {T} variables'.format(T2=1<<T,T=T))
    print('  simd_t v[{T}];'.format(T=T))
    assert (T >= 2 and T <= 4)
    size = 32 >> T
    s = ['0'*size, 'f'*size]
    m = 2
    for vi in range(T):
        v = '  v[{0}] = _mm_set_epi32(0x'.format(vi)
        for j in range(1 << T):
            if ((j*size)%8 == 0 and j != 0):
                v += ',0x'
            v += s[j%m // (m >> 1)]
        m = m << 1
        print(v + ');')
    print('')

    idx = [ "idx_{0}".format(j) for j in range(degree) ] # indices for the loops
    letters = [ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'o', 'p', 'q', 'r', 's', 't' ]

    for updater_degree in range(1, degree+1):
        for updatee_degree in range(updater_degree):

            print('  // updates degree-{0} term with degree-{1} terms'.format(updatee_degree, updater_degree))
            
            # loops over the updatee indices
            if updatee_degree > 0:
                print('  for(int {idx}=0; {idx}<n-{T}; {idx}++)'.format(idx=idx[updatee_degree-1], T=2))
            for i in range(updatee_degree-2, -1, -1):
                print('{pad}for(int {idx}=0; {idx}<{prev_idx}; {idx}++)'.format(pad='  '*(updatee_degree-i), idx=idx[i], prev_idx=idx[i+1]))

            # loops over the updater indices
            print('  {pad}for(int {idx}=0; {idx}<{T}; {idx}++)'.format(idx=letters[updater_degree-updatee_degree-1], T=T, pad='  '*updatee_degree))
            for i in range(updater_degree-updatee_degree-1):
                print('  {pad}for(int {idx}=0; {idx}<{prev_idx}; {idx}++)'.format(pad='  '*(updatee_degree+i+1), idx=letters[updater_degree-updatee_degree-2-i], prev_idx=letters[updater_degree-updatee_degree-i-1],T=T))
            updatee = data_reference('F', idx[:updatee_degree])
            updater = data_reference('F', idx[:updatee_degree] + list(map(lambda x:"n-{T}+{x}".format(T=T,x=x), letters[:updater_degree-updatee_degree])))
            monomial =  " & ".join(map( lambda i:'v[{0}]'.format(i), letters[:updater_degree-updatee_degree]))
            print('  {pad}{LHS} ^= {RHS} & ({monomial});'.format(pad='  '*updater_degree, LHS=updatee, RHS=updater, monomial=monomial))
            print('')


def gen_main_function_name(degree, simd=False, T=2, n_eliminated_derivatives=0):
    if simd:
        return 'void exhaustive_sse2_deg_{d}_T_{T}_el_{el}(LUT_t LUT, int n, pck_vector_t nonsimd_F[], solution_callback_t callback, void* callback_state, int verbose)'.format(d=degree, el=n_eliminated_derivatives,T=T)
    else:
        return 'void exhaustive_ia32_deg_{d}(LUT_t LUT, int n, pck_vector_t F[], solution_callback_t callback, void* callback_state, int verbose)'.format(d=degree)


### generates the main function
def gen_main(degree, L, unrolling=True, simd=False, T=0, with_assembly=False, n_eliminated_derivatives=0):

    k = [ "k_{0}".format(j) for j in range(degree+1) ]   # variables names to store the results of the b_k function calls
    idx = [ "idx_{0}".format(j) for j in range(degree) ] # indices for the loops
    greek = [ "alpha", "beta", "gamma", "delta", "epsilon", "zeta", "eta", "iota", "kappa", "lambda", "mu", \
              "nu", "xi", "omicron", "pi", "rho", "sigma", "tau", "upsilon", "phi", "chi", "psi", "omega" ]

    LUT = idx_LUT.init(128, degree)

    def invoke_step(indices, i_idx, reverse=False):      # writes down STEP_k(.....)
        w = len(indices)
        reverse_indices = indices
        if reverse:
            reverse_indices.reverse()
        lut_indices = map( lambda i: 'idx_{i}(LUT, {indices})'.format(i=i+1, indices = ", ".join(reverse_indices[:i+1] )), range(w))
        print('    {pad}STEP_{w}( {indices}, {i_idx} );'.format( i_idx=i_idx, pad='  '*w, w=w, indices=", ".join(lut_indices )))
        

  # this has been moved to the all_functions.h (autogenerated) file
  #  if with_assembly:
  #      print 'extern void func_deg_{d}_el_{el}(__m128i *F, __m128i **F_sp, void *buf, uint64_t *num, uint64_t idx);\n'.format(d=degree, el=n_eliminated_derivatives);

    print('')
    print('typedef struct {')
    print('  uint64_t int_idx;')
    print('  uint32_t mask;')
    print('} solution_t;')
    print('')
 
    print('// generated with L = {L}'.format(L=L))
    print( gen_main_function_name(degree, simd, T, n_eliminated_derivatives) + ' {' )
    if simd:
        print( '  const __m128i simd_zero = _mm_setzero_si128();')
        print( '')
        print( '  // *****initialise an SIMD version of the constants and internal state')
        print( '  const int N = n_monomials(n, {0});'.format(degree))
        print( '  simd_t * F = NULL;')
        print( '  int foo = posix_memalign((void **) &F, sizeof(simd_t), N*sizeof(simd_t));')
        print( '  if (foo != 0 || F == NULL) {')
        print( '    perror("[fes/enumeration/simd]");')
        print( '    return;')
        print( '  }')

        print( '  for(int i=0; i<N; i++)')
        print( '    F[i] = _mm_set1_epi{}(nonsimd_F[i]);  // expand all the coefficients into SIMD vectors'.format(128 >> T))
        print( '')
        print( '  uint64_t everything_start_time = rdtsc();')
        gen_specialization(T=T, degree=degree)
        print( '  if (verbose) printf("fes: specialization = %" PRIu64 " cycles\\n", rdtsc()-everything_start_time);')

    print( '  #define QUIT() { \\')
    if simd:
        print( '    if (F != NULL) free(F); \\' )
    print( '    return; \\')
    print( '  }')
    print( '')

    print( '  uint64_t init_start_time = rdtsc();')
    gen_init_code(degree)
    print( '  if (verbose) printf("fes: initialisation = %" PRIu64 " cycles\\n", rdtsc()-init_start_time);')
    
    size_pack_of_solution = 65536
    print( '  uint64_t enumeration_start_time = rdtsc();')
    print( '  uint64_t n_solutions_found = 0;')
    print( '  uint64_t current_solution_index = 0;')
    print( '  uint64_t pack_of_solution[{0}];'.format(size_pack_of_solution))
    print( '  solution_t solution_buffer[{0}];'.format( (1 << L) + (1 << degree) ))
    print( '')
    print( '  #define FLUSH_SOLUTIONS() { \\')
    print( '    if ((*callback)(callback_state, current_solution_index, pack_of_solution)) \\')
    print( '      QUIT(); \\')
    print( '    }')
    print( '')
    print( '  #define PUSH_SOLUTION(current_solution) { \\')
    print( '    pack_of_solution[current_solution_index] = current_solution; \\')
    print( '    current_solution_index++; \\')
    print( '    if (current_solution_index == {0})'.format(size_pack_of_solution) + '{ \\')
    print( '      FLUSH_SOLUTIONS(); \\')
    print( '      current_solution_index = 0; \\')
    print( '    } \\')
    print( '  }')
    print( '')
    size = 1 << (4 - T)
    unit = (1 << size)-1
    print( '  #define CHECK_SOLUTIONS() { \\')
    print( '    for(uint64_t i=0; i<n_solutions_found; i++){ \\')
    for i in range(1 << T):
        print( '      if (solution_buffer[i].mask & 0x{0:04x}) \\'.format(unit << size*i))
        print( '        PUSH_SOLUTION(to_gray(solution_buffer[i].int_idx) + {0}*(1ll << (n-{T}))); \\'.format((1 << T) - 1 - i, T=T))
    print( '    } \\')
    print( '    n_solutions_found = 0; \\')
    print( '  }')

    print( '')
    print( '  // special case for i=0')
    print( '  const uint64_t weight_0_start = 0;')
    print( '  STEP_0(0);')
    print( '')
    print( '  // from now on, hamming weight is >= 1')
    for w in range(degree-1):
        if w == 0:
            if simd:
                print( '  {pad}for(int {idx}=0; {idx}<n-{T}; {idx}++) {{'.format(pad='  '*w, idx=idx[w],T=T))
            else:
                print( '  {pad}for(int {idx}=0; {idx}<n    ; {idx}++) {{'.format(pad='  '*w, idx=idx[w]))

        else:
            print( '  {pad}for(int {idx}=0; {idx}<{prev_idx}; {idx}++) {{'.format(pad='  '*w, idx=idx[w], prev_idx=idx[w-1]))
        print( '')
        print( '    {pad}// special case when i has hamming weight exactly {w}'.format(pad='  '*w,w=w+1))
        print( '    {pad}const uint64_t weight_{w}_start = weight_{wm1}_start + (1ll << idx_{wm1});'.format(pad='  '*w, w=w+1, wm1 = w))
        invoke_step(idx[:w+1], "weight_{w}_start".format(w=w+1), reverse=True)
        print( '')

    print( '  {pad}// we are now inside the critical part where the hamming weight is known to be >= {d}'.format(pad='  '*degree, d=degree))
    print( '  {pad}// Thus, there are no special cases from now on'.format(pad='  '*degree))
    print( '')
    print( '  {pad}// Because of the last step, the current iteration counter is a multiple of {L} plus one'.format( pad='  '*degree, L=1<<L))
    print( '  {pad}// This loop sets it to `rolled_end`, which is a multiple of {L}, if possible'.format( pad='  '*degree, L=1<<L))
    print( '')
    print( '{pad}const uint64_t rolled_end = weight_{w}_start + (1ll << min({L}, idx_{d}));'.format( pad='  '*degree, d=degree-2, w=degree-1,L=L ))
    print( '{pad}for(uint64_t i=1 + weight_{w}_start; i< rolled_end; i++) {{'.format( pad='  '*degree, w=degree-1 ))
    print( '')

    def gen_k_i_s():
        print('{pad}      int pos = 0;'.format(pad='  '*degree))
        print('{pad}      uint64_t _i = i;'.format(pad='  '*degree))

        for i in range(1, degree+1):
            print('{pad}      while ((_i & 0x0001) == 0) {{ _i >>= 1; pos++; }}'.format( pad='  '*(degree)))
            print('{pad}      const int {k} = pos;'.format( pad='  '*(degree), k=k[i] ))
            if i != degree:
                print('{pad}      _i >>= 1; pos++;'.format( pad='  '*(degree)))


    gen_k_i_s()
    invoke_step(k[1:], "i")
    print('{pad}}}'.format( pad='  '*degree ))

    print('')
    print('{pad}CHECK_SOLUTIONS();'.format( pad='  '*degree ))
    print('')

    print('{pad}// Here, the number of iterations to perform is (supposedly) sufficiently large'.format( pad='  '*degree ))
    print('{pad}// We will therefore unroll the loop {L} times'.format( pad='  '*degree,L=1<<L ))
    print('')
    print('{pad}// unrolled critical section where the hamming weight is >= {degree}'.format( pad='  '*degree, degree=degree))
    print('{pad}for(uint64_t j={L}; j<(1ull << idx_{d}); j+={L}) {{'.format( pad='  '*degree, d=degree-2, L=1<<L ) )

    # this rolled version of the critical loop is known to work properly
    # it is used for debugging
    if not unrolling:
        print('{pad}  for(int k=0; k<{L}; k++) {{ // pretend-unroll'.format( pad='  '*degree, L=1<<L))
        print('{pad}      const uint64_t i = j + weight_{w}_start + k;'.format(pad='  '*degree, w=degree-1))
        gen_k_i_s()
        invoke_step(k[1:], "i")
        print('{pad}    }}'.format( pad='  '*degree ))

    else: # this is the actual, fast, unrolled version
        print('{pad}      const uint64_t i = j + weight_{w}_start;'.format(pad='  '*degree, w=degree-1))
        gen_k_i_s()

        eliminated_indices = get_idx_list.get_degD_idx_list(degree, L, n_eliminated_derivatives)
        if eliminated_indices != []:
            print('{pad}      // this version of the code assumes that: '.format(pad='  '*degree))
            for i in eliminated_indices:
                print('{pad}      // F[ {idx} ] = 0'.format(pad='  '*degree, idx=i))

        # set the "greek letters shortcuts"
        unknown_indices = []
        for foo in range(degree):
            unknown_indices.append( k[foo+1] )
            unlookable_part = [ "LUT[{tab}][{idx}]".format(tab=len(unknown_indices)-1-i, idx=unknown_indices[i]) for i in range(len(unknown_indices)) ]
            print('{pad}      const int {greek} = {stuff};'.format( pad='  '*(degree), greek=greek[foo], stuff="+".join(unlookable_part )))

        # each time a greek letter is used, the offset is increased by one. This table stores the offsets
        current_counter = [0] * degree

        #this actually unrols the loop
        for unroll_step in range(1 << L):   

            hw = min( degree, popcount(unroll_step) )

            # computes the set of offsets for this step
            # the first `hw` ones can be determined at compile-time (i.e. b_k(unroll_step) is defined for k<=hw),
            known_ks = [ b_k(i, unroll_step) for i in range(1, hw+1) ]
            known_indices = [ idx_LUT.idx(LUT, known_ks[:i]) for i in range(1,hw+1) ]
            indices = [ str(i) for i in known_indices]

            # but b_i( unroll_step ) is not defined when i>hw, so that we have to rely on the (runtime) value of the k_i variable
            for k in range(hw, degree):
                indices.append( '{counter} + {greek}'.format(counter=current_counter[k-hw], greek=greek[k-hw]) )
                current_counter[k-hw] += 1
                   
            #outputs the step
            if (not with_assembly) or unroll_step == 0:
                if hw == degree and known_indices[hw-1] in eliminated_indices:
                    print('{pad}      STEP_{d}({indices}, i + {k});'.format( pad='  '*degree, d=degree-1, indices=', '.join(indices[:hw-1],), k=unroll_step))
                else:
                    print('{pad}      STEP_{d}({indices}, i + {k});'.format( pad='  '*degree, d=degree, indices=', '.join(indices), k=unroll_step))

                if with_assembly:
#                    sp_list = [ 'F + ' + a for a in greek[:degree-1] ] ## known to work
                    sp_list = [ a+'*16' for a in greek[:degree-1] ]
                    msg = '{pad}      uint64_t F_sp[ {d} ] = {{ {sp} }};'.format( pad='  '*degree, d=degree-1, sp=', '.join(sp_list ) )
                    print( msg )
                    print('{pad}      func_deg_{d}_T_{T}_el_{el}(F, &F_sp[0], solution_buffer, &n_solutions_found, i);'.format(pad='  '*degree, d=degree, el=n_eliminated_derivatives,T=T))
       
# unrolling is over

    print('')
    print('{pad}      CHECK_SOLUTIONS();'.format( pad='  '*degree ))
    print('{pad}  }}'.format( pad='  '*degree ))
    print('')

    ## end unrolled loop
    for w in range(degree-1):
        print(('{pad}}}'.format( pad='  '*(degree-w-1) )))
    
    print( '  FLUSH_SOLUTIONS();')
    print( '  uint64_t end_time = rdtsc();')
    print(  ' if (verbose) printf("fes: enumeration+check = %" PRIu64 " cycles\\n", end_time - enumeration_start_time);')

    print( '  QUIT();')
    print( '}')



def gen_boilerplate_header(simd=False):
    print("#include <stdio.h>")
    print("#include <inttypes.h>")
    print("#include <stdlib.h>")
    if simd:
        print('#include <emmintrin.h>')
        print('typedef __m128i simd_t;')
    print('')
    print('#include "fes.h"')
    print('#include "idx_LUT.h"')
    print('')

def gen_tester(degree):
    idx = [ "idx_{0}".format(j) for j in range(degree+1) ] # indices for the loop
    vs = [ "v_{0}".format(j) for j in range(degree+1) ] # indices for the loop

    print( 'pck_vector_t packed_eval_deg_{0}(LUT_t LUT, int n, pck_vector_t F[], uint64_t i) {{'.format(degree))
    print( '  // first expand the values of the variables from `i`')
    print( '  pck_vector_t v[n];')
    print( '  for(int k=0; k<n; k++) {')
    print( '    v[k] = 0;')
    print( '    if (i & 0x0001) v[k] = 0xffffffff;')
    print( '    i = (i >> 1ll);')
    print( '  }')
    print( '')
    print( '  pck_vector_t y = F[0];')
    print( '')
    print( '  for(int {idx}=0; {idx}<n; {idx}++) {{'.format(idx=idx[0]))
    print( '    const pck_vector_t {v} = v[{idx}];'.format(v=vs[0], idx=idx[0]))
    print( '')
    for i in range(1,degree+1):
        print( '  {pad}// computes the contribution of degree-{i} terms'.format(pad='  '*i, i=i))
        print( '  {pad}y ^= {coeff} & {v};'.format(pad='  '*i, coeff=data_reference('F', list(reversed(idx[:i]))), v=vs[i-1]))
        print( '')
        if i < degree:
            print( '  {pad}for(int {idx}=0; {idx}<{prev_idx}; {idx}++) {{'.format(pad='  '*i, idx=idx[i], prev_idx=idx[i-1]))
            print( '    {pad}const pck_vector_t {v} = {prev_v} & v[{idx}];'.format(pad='  '*i, idx=idx[i], v=vs[i], prev_v=vs[i-1]))
        print( '')
    print( ' ' + '}'*(degree))
    print( '')
    print( '  return y;')
    print( '}')


if len(sys.argv) < 2:
    print("usage: enumeration_generator.py [filename]")
    exit(1)


#try:
filename = sys.argv[1].split(".")
assert(filename[1] == 'c')
parts = filename[0].split("_")
assert (parts[0] == "autogenerated")
mode = parts[1] 
assert (parts[2] == "deg")
degree = int(parts[3])

if mode == "c-tester":
    # we generate the testers
    gen_boilerplate_header()
    gen_tester(degree)
elif mode == "c-sequential":
    gen_boilerplate_header()
    gen_steps(degree)
    gen_main(degree, 10-degree//2, unrolling=True)

else:  # we generate the actual enumerations
    assert (parts[4] == "T")
    T = int(parts[5])
    assert(T >= 2 and T <= 4)
    assert (parts[6] == "el")
    eliminated_terms = int( parts[7] )

    if mode == "c-simd":
        gen_boilerplate_header(simd=True)
        gen_steps(degree,simd=True)
        gen_main(degree, 10-degree//2, unrolling=True, simd=True, T=T, n_eliminated_derivatives=eliminated_terms)
    elif mode == "c-simd-with-asm":
        gen_boilerplate_header(simd=True)
        gen_steps(degree,simd=True, T=T)
        gen_main(degree, 10-degree//2, unrolling=True, simd=True, T=T, with_assembly=True, n_eliminated_derivatives=eliminated_terms)
    else:
        raise ValueError
  
#except ValueError:
#    print("the name of the file should follow the template 'autogenerated_[mode]_deg_[degree]_el_[number of eliminated coeffs].c'")
#    exit(1)
