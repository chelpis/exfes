#!/usr/bin/python
# this file tests the wrapper

import subprocess

def run_test(i, cmd):
    if subprocess.call(cmd, shell=True) == 0:
        print('ok {0} - {1}'.format(i, cmd))
    else:
        print('not ok {0} - {1}'.format(i, cmd))

##################

standard_tests =  [ #'./test_wrapper --n 5 --e 3',
                    #'./test_wrapper --n 5 --e 3 --fft',
					#'./test_wrapper --n 5 --e 3 --degree 4',
					#'./test_wrapper --n 5 --e 3 --degree 4 --fft',
					#'./test_wrapper --n 15 --e 15 --degree 3',
					#'./test_wrapper --n 16 --e 16 --degree 7',
					#'./test_wrapper --n 17 --e 17 --degree 6',
					#'./test_wrapper --n 17 --e 17 --degree 6 --fft',
					#'./test_wrapper --n 31 --e 31 --degree 2',
					#'./test_wrapper --n 33 --e 33 --degree 2',
					#'./test_wrapper --n 24 --e 1000 --degree 5',
					#'./test_wrapper --n 24 --e 1000 --degree 5 --fft',
					#'./test_wrapper --n 7  --e 1000 --degree 5',
					#'./test_wrapper --n 7  --e 1000 --degree 3',
					#'./test_wrapper --n 7  --e 1000 --degree 2',
					#'./test_wrapper --n 7  --e 1000 --degree 2 --fft',
					# Beware that m >= (n-degree) if fft mode applied!
					'./test_wrapper --m  1 --n 18 --e 10',
					'./test_wrapper --m  2 --n 19 --e 11',
					'./test_wrapper --m  3 --n 20 --e 12',
					'./test_wrapper --m  4 --n 21 --e 13',
					'./test_wrapper --m  4 --n 21 --e 13 --degree 3',
					'./test_wrapper --m  4 --n 21 --e 13 --degree 4',
					'./test_wrapper --m  4 --n 21 --e 13 --degree 5',
					'./test_wrapper --m 17 --n 21 --e 13',
                ]

# TAP protocol (http://en.wikipedia.org/wiki/Test_Anything_Protocol)
i = 0
for cmd in standard_tests:
    i += 1
    run_test(i, cmd)
print('1..{0}'.format(i))
