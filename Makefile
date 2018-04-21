test: autogenerated_asm_deg_2_T_2_el_0.o autogenerated_asm_deg_2_T_3_el_0.o autogenerated_asm_deg_2_T_4_el_0.o autogenerated_c-sequential_deg_2.o autogenerated_c-simd-with-asm_deg_2_T_2_el_0.o autogenerated_c-simd-with-asm_deg_2_T_3_el_0.o autogenerated_c-simd-with-asm_deg_2_T_4_el_0.o autogenerated_c-tester_deg_2.o binomials.o common.o exfes.o idx_LUT.o moebius_transform.o rand.o wrapper.o
	g++ test.c autogenerated_asm_deg_2_T_2_el_0.o autogenerated_asm_deg_2_T_3_el_0.o autogenerated_asm_deg_2_T_4_el_0.o autogenerated_c-sequential_deg_2.o autogenerated_c-simd-with-asm_deg_2_T_2_el_0.o autogenerated_c-simd-with-asm_deg_2_T_3_el_0.o autogenerated_c-simd-with-asm_deg_2_T_4_el_0.o autogenerated_c-tester_deg_2.o binomials.o common.o exfes.o idx_LUT.o moebius_transform.o rand.o wrapper.o -O3 -lstdc++ -o test

test_wrapper: autogenerated_asm_deg_2_T_2_el_0.o autogenerated_asm_deg_2_T_3_el_0.o autogenerated_asm_deg_2_T_4_el_0.o autogenerated_c-sequential_deg_2.o autogenerated_c-simd-with-asm_deg_2_T_2_el_0.o autogenerated_c-simd-with-asm_deg_2_T_3_el_0.o autogenerated_c-simd-with-asm_deg_2_T_4_el_0.o autogenerated_c-tester_deg_2.o binomials.o common.o idx_LUT.o moebius_transform.o rand.o test_wrapper.o wrapper.o
	gcc test_wrapper.c autogenerated_asm_deg_2_T_2_el_0.o autogenerated_asm_deg_2_T_3_el_0.o autogenerated_asm_deg_2_T_4_el_0.o autogenerated_c-sequential_deg_2.o autogenerated_c-simd-with-asm_deg_2_T_2_el_0.o autogenerated_c-simd-with-asm_deg_2_T_3_el_0.o autogenerated_c-simd-with-asm_deg_2_T_4_el_0.o autogenerated_c-tester_deg_2.o binomials.o common.o idx_LUT.o moebius_transform.o rand.o wrapper.o -O3 -o test_wrapper

autogenerated_asm_deg_2_T_2_el_0.o:
	as autogenerated_asm_deg_2_T_2_el_0.s -o autogenerated_asm_deg_2_T_2_el_0.o

autogenerated_asm_deg_2_T_3_el_0.o:
	as autogenerated_asm_deg_2_T_3_el_0.s -o autogenerated_asm_deg_2_T_3_el_0.o

autogenerated_asm_deg_2_T_4_el_0.o:
	as autogenerated_asm_deg_2_T_4_el_0.s -o autogenerated_asm_deg_2_T_4_el_0.o

autogenerated_c-sequential_deg_2.o:
	gcc autogenerated_c-sequential_deg_2.c -o autogenerated_c-sequential_deg_2.o -c

autogenerated_c-simd-with-asm_deg_2_T_2_el_0.o:
	gcc autogenerated_c-simd-with-asm_deg_2_T_2_el_0.c -o autogenerated_c-simd-with-asm_deg_2_T_2_el_0.o -c

autogenerated_c-simd-with-asm_deg_2_T_3_el_0.o:
	gcc autogenerated_c-simd-with-asm_deg_2_T_3_el_0.c -o autogenerated_c-simd-with-asm_deg_2_T_3_el_0.o -c

autogenerated_c-simd-with-asm_deg_2_T_4_el_0.o:
	gcc autogenerated_c-simd-with-asm_deg_2_T_4_el_0.c -o autogenerated_c-simd-with-asm_deg_2_T_4_el_0.o -c

autogenerated_c-tester_deg_2.o:
	gcc autogenerated_c-tester_deg_2.c -o autogenerated_c-tester_deg_2.o -c

exfes.o:
	gcc exfes.c -o exfes.o -c

binomials.o:
	gcc binomials.c -o binomials.o -c

common.o:
	gcc common.c -o common.o -c

idx_LUT.o:
	gcc idx_LUT.c -o idx_LUT.o -c

moebius_transform.o:
	gcc moebius_transform.c -o moebius_transform.o -c

rand.o:
	gcc rand.c -o rand.o -c

wrapper.o:
	gcc wrapper.c -o wrapper.o -c

clean:
	rm -f test test_wrapper
	rm -f *.o
