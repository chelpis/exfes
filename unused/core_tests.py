#!/usr/bin/python
# this file tests the inner kernels, by short-cutting the wrapper

from subprocess import check_output

output = dict()
# outputs with all coefficients
output[19,16,2] = {0x1a9b7, 0x2ae60, 0x3270a, 0x337ae, 0x52869, 0x56bff, 0x6f999, 0x6fbd2, 0x73a6a}
output[17,15,3] = {0x00ce0, 0x01bdf, 0x0247d, 0x087e5, 0x1bf36}
output[19,16,3] = {0x01bdf, 0x0247d, 0x087e5, 0x20b04, 0x28fd3, 0x656f1}
output[16,13,4] = {0x01f5e, 0x02945, 0x05040, 0x07d09, 0x08789, 0x0a925, 0x0aea0, 0x0f1e5}
output[15,12,5] = {0x00189, 0x02bcd, 0x03111, 0x03c81, 0x072bb}
output[14,11,6] = {0x0021c, 0x01573, 0x03db3}
output[14,11,7] = {0x00108, 0x00cf1, 0x012f5, 0x0193f, 0x02345, 0x02947}

#outputs with eliminated coefficients
output[19,16,2,1] = {0x16683, 0x2ae60, 0x3270a, 0x337ae, 0x3e563, 0x52869, 0x6f999, 0x6fbd2, 0x73a6a}
output[19,16,2,2] = {0x0ec15, 0x16683, 0x28b4d, 0x2ae60, 0x3270a, 0x337ae, 0x3e563, 0x52869, 0x6f999, 0x6fbd2, 0x73a6a}
output[19,16,2,3] = {0x0ec15, 0x16683, 0x28b4d, 0x2ae60, 0x2cacf, 0x3270a, 0x3e563, 0x52869, 0x5efaf, 0x6f999, 0x6fbd2, 0x73a6a}
output[17,15,3,1] = {0x00ce0, 0x0247d, 0x087e5, 0x1bf36}
output[17,15,3,2] = {0x00ce0, 0x0247d, 0x087e5, 0x19c5b, 0x1bf36}
output[17,15,3,8] = {0x00ce0, 0x01bb3, 0x087e5, 0x17c59}
output[16,13,4,1] = {0x01f5e, 0x02945, 0x05040, 0x07d09, 0x08789, 0x0a925, 0x0aea0, 0x0f1e5}
output[16,13,4,2] = {0x01f5e, 0x02945, 0x05040, 0x0729f, 0x07d09, 0x08789, 0x0a925, 0x0aea0, 0x0f1e5}
output[16,13,4,8] = {0x02945, 0x05040, 0x07d09, 0x08789, 0x0a925, 0x0aea0, 0x0f1e5}



def run_test(i, program, args1, args2):
    args_dict = args1.copy()
    args_dict.update(args2)
    args = [program] + reduce(lambda x,y: x+y, map(lambda (name,value):['--{0}'.format(name), '{0}'.format(value)], args_dict.iteritems()))
    program_output = check_output(args)
    values = { int(line,16) for line in program_output.split("\n") if line != "" }
    if values == output[ (args_dict['n'],args_dict['e'],args_dict['degree'])  ]:
        print 'ok {0} - {1} {2}' . format(i, program, idx)
    else:
        print 'not ok {0} - {1} {2}'.format(i, program, idx)

##################

standard_tests =  [ {'n':19, 'e':16, 'degree':2},
                    {'n':17, 'e':15, 'degree':3},
                    {'n':16, 'e':13, 'degree':4},
                    {'n':15, 'e':12, 'degree':5},
                    {'n':14, 'e':11, 'degree':6} ]

test_list = [ ('NAIVE algorithm', 'semislow', standard_tests, {}),
              ('Fast enumeration, plain C', 'fast', standard_tests, {}),
              ('Fast enumeration, x64 asm, T=2', 'simd', standard_tests, {'simd_thread':2}),
              ('Fast enumeration, x64 asm, T=3', 'simd', standard_tests, {'simd_thread':3}),
              ('Boolean FFT, plain C', 'moebius', standard_tests, {}) ]


n_tests = reduce( lambda x, y: x + len(y[2]), test_list, 0 )

# TAP protocol (http://en.wikipedia.org/wiki/Test_Anything_Protocol)
print('1..{0}' . format(n_tests))
i = 1
for (name, executable, bench_list, args) in test_list:
    print('#', name)
    for idx in bench_list:
        run_test(i, './' + executable, idx, args)
        i += 1
