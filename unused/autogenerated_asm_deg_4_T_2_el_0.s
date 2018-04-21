.text
.p2align 5

.globl _func_deg_4_T_2_el_0
.globl func_deg_4_T_2_el_0
### void func_deg_4_T_2_el_0(__m128i *F, __m128i **F_sp, void *buf, uint64_t *num, uint64_t idx);

# the ABI (http://www.x86-64.org/documentation/abi.pdf) says that...
# A) we should preserve the values of %rbx, %rbp, %r12...%r15 [callee-save registers]
# B) We will receive the arguments of the function in registers :
#       the pointer to F should be in %rdi
#       the pointer to *F_sp should be in %rsi
#       the pointer to buf should be in %rdx
#       the pointer to num should be in %rcx
#       idx should be in %r8

_func_deg_4_T_2_el_0:
func_deg_4_T_2_el_0:

# intialize our stack frame
mov %rsp, %r11
and $31, %r11
add $64, %r11
sub %r11, %rsp

# save the callee-save registers
movq %r11, 0(%rsp)
movq %r12, 8(%rsp)
movq %r13, 16(%rsp)
movq %r14, 24(%rsp)
movq %r15, 32(%rsp)
movq %rbx, 40(%rsp)
movq %rbp, 48(%rsp)
# variable F maps to %rdi
# variable F_sp maps to %rsi
# variable buf maps to %rdx
# variable num_ptr maps to %rcx
# variable idx maps to %r8
# variable num maps to %r9
# variable ('F', 0) maps to %xmm0
# variable ('F', 1) maps to %xmm1
# variable ('F', 2) maps to %xmm2
# variable ('F', 3) maps to %xmm3
# variable ('F', 4) maps to %xmm4
# variable ('F', 5) maps to %xmm5
# variable ('F', 6) maps to %xmm6
# variable ('F', 7) maps to %xmm7
# variable ('F', 8) maps to %xmm8
# variable ('F', 9) maps to %xmm9
# variable ('F', 10) maps to %xmm10
# variable ('F', 11) maps to %xmm11
# variable ('F', 12) maps to %xmm12
# variable ('F', 13) maps to %xmm13
# variable alpha maps to %rax
# variable beta maps to %r10
# variable gamma maps to %r11
# variable sum maps to %xmm14
# variable zero maps to %xmm15
# variable mask maps to %r12d

# load the most-frequently used derivatives (F[0]...F[13]) into %xmm registers
movdqa 0(%rdi), %xmm0   ## %xmm0 = F[0]
movdqa 16(%rdi), %xmm1   ## %xmm1 = F[1]
movdqa 32(%rdi), %xmm2   ## %xmm2 = F[2]
movdqa 48(%rdi), %xmm3   ## %xmm3 = F[3]
movdqa 64(%rdi), %xmm4   ## %xmm4 = F[4]
movdqa 80(%rdi), %xmm5   ## %xmm5 = F[5]
movdqa 96(%rdi), %xmm6   ## %xmm6 = F[6]
movdqa 112(%rdi), %xmm7   ## %xmm7 = F[7]
movdqa 128(%rdi), %xmm8   ## %xmm8 = F[8]
movdqa 144(%rdi), %xmm9   ## %xmm9 = F[9]
movdqa 160(%rdi), %xmm10   ## %xmm10 = F[10]
movdqa 176(%rdi), %xmm11   ## %xmm11 = F[11]
movdqa 192(%rdi), %xmm12   ## %xmm12 = F[12]
movdqa 208(%rdi), %xmm13   ## %xmm13 = F[13]

# loads the greek letters, i.e. indices of the derivatives that do not fit into registers
movq 0(%rsi), %rax   ## %rax = alpha
movq 8(%rsi), %r10   ## %r10 = beta
movq 16(%rsi), %r11   ## %r11 = gamma

# note that at this point, the register holding `F_sp` [%rsi] could be used for something else

# initialize the last things that remains to be intialized...
movq (%rcx), %r9  ## num = *num_ptr
pxor %xmm15, %xmm15   ## zero = 0


##### step 1 [hw=1]
##### F[ 1 ] ^= ( F[ 1 ] ^= ( F[ alpha + 1 ] ^= ( F[ beta + 1 ] ^= ( F[ gamma + 1 ])))))

movdqa 16(%rdi,%r11), %xmm14
pxor 16(%rdi,%r10), %xmm14
movdqa %xmm14, 16(%rdi,%r10)
pxor 16(%rdi,%rax), %xmm14
movdqa %xmm14, 16(%rdi,%rax)
pxor %xmm14, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_1
._step_1_end:

##### step 2 [hw=1]
##### F[ 1 ] ^= ( F[ 1 ] ^= ( F[ alpha + 1 ] ^= ( F[ beta + 1 ] ^= ( F[ gamma + 1 ])))))

movdqa 32(%rdi,%r11), %xmm14
pxor 32(%rdi,%r10), %xmm14
movdqa %xmm14, 32(%rdi,%r10)
pxor 32(%rdi,%rax), %xmm14
movdqa %xmm14, 32(%rdi,%rax)
pxor %xmm14, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_2
._step_2_end:

##### step 3 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 48(%rdi,%r10), %xmm14
pxor 48(%rdi,%rax), %xmm14
movdqa %xmm14, 48(%rdi,%rax)
pxor %xmm14, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_3
._step_3_end:

##### step 4 [hw=1]
##### F[ 1 ] ^= ( F[ 1 ] ^= ( F[ alpha + 1 ] ^= ( F[ beta + 1 ] ^= ( F[ gamma + 1 ])))))

movdqa 48(%rdi,%r11), %xmm14
pxor 64(%rdi,%r10), %xmm14
movdqa %xmm14, 64(%rdi,%r10)
pxor 64(%rdi,%rax), %xmm14
movdqa %xmm14, 64(%rdi,%rax)
pxor %xmm14, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_4
._step_4_end:

##### step 5 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 80(%rdi,%r10), %xmm14
pxor 80(%rdi,%rax), %xmm14
movdqa %xmm14, 80(%rdi,%rax)
pxor %xmm14, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_5
._step_5_end:

##### step 6 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 96(%rdi,%r10), %xmm14
pxor 96(%rdi,%rax), %xmm14
movdqa %xmm14, 96(%rdi,%rax)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_6
._step_6_end:

##### step 7 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

pxor 112(%rdi,%rax), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_7
._step_7_end:

##### step 8 [hw=1]
##### F[ 1 ] ^= ( F[ 1 ] ^= ( F[ alpha + 1 ] ^= ( F[ beta + 1 ] ^= ( F[ gamma + 1 ])))))

movdqa 64(%rdi,%r11), %xmm14
pxor 112(%rdi,%r10), %xmm14
movdqa %xmm14, 112(%rdi,%r10)
pxor 128(%rdi,%rax), %xmm14
movdqa %xmm14, 128(%rdi,%rax)
pxor %xmm14, %xmm8
pxor %xmm8, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_8
._step_8_end:

##### step 9 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 128(%rdi,%r10), %xmm14
pxor 144(%rdi,%rax), %xmm14
movdqa %xmm14, 144(%rdi,%rax)
pxor %xmm14, %xmm9
pxor %xmm9, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_9
._step_9_end:

##### step 10 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 144(%rdi,%r10), %xmm14
pxor 160(%rdi,%rax), %xmm14
movdqa %xmm14, 160(%rdi,%rax)
pxor %xmm14, %xmm10
pxor %xmm10, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_10
._step_10_end:

##### step 11 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

pxor 176(%rdi,%rax), %xmm11
pxor %xmm11, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_11
._step_11_end:

##### step 12 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 160(%rdi,%r10), %xmm14
pxor 192(%rdi,%rax), %xmm14
movdqa %xmm14, 192(%rdi,%rax)
pxor %xmm14, %xmm12
pxor %xmm12, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_12
._step_12_end:

##### step 13 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

pxor 208(%rdi,%rax), %xmm13
pxor %xmm13, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_13
._step_13_end:

##### step 14 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 224(%rdi,%rax), %xmm14
pxor 224(%rdi), %xmm14
movdqa %xmm14, 224(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_14
._step_14_end:

##### step 15 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 240(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_15
._step_15_end:

##### step 16 [hw=1]
##### F[ 1 ] ^= ( F[ 1 ] ^= ( F[ alpha + 1 ] ^= ( F[ beta + 1 ] ^= ( F[ gamma + 1 ])))))

movdqa 80(%rdi,%r11), %xmm14
pxor 176(%rdi,%r10), %xmm14
movdqa %xmm14, 176(%rdi,%r10)
pxor 240(%rdi,%rax), %xmm14
movdqa %xmm14, 240(%rdi,%rax)
pxor 256(%rdi), %xmm14
movdqa %xmm14, 256(%rdi)
pxor %xmm14, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_16
._step_16_end:

##### step 17 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 192(%rdi,%r10), %xmm14
pxor 256(%rdi,%rax), %xmm14
movdqa %xmm14, 256(%rdi,%rax)
pxor 272(%rdi), %xmm14
movdqa %xmm14, 272(%rdi)
pxor %xmm14, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_17
._step_17_end:

##### step 18 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 208(%rdi,%r10), %xmm14
pxor 272(%rdi,%rax), %xmm14
movdqa %xmm14, 272(%rdi,%rax)
pxor 288(%rdi), %xmm14
movdqa %xmm14, 288(%rdi)
pxor %xmm14, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_18
._step_18_end:

##### step 19 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 288(%rdi,%rax), %xmm14
pxor 304(%rdi), %xmm14
movdqa %xmm14, 304(%rdi)
pxor %xmm14, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_19
._step_19_end:

##### step 20 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 224(%rdi,%r10), %xmm14
pxor 304(%rdi,%rax), %xmm14
movdqa %xmm14, 304(%rdi,%rax)
pxor 320(%rdi), %xmm14
movdqa %xmm14, 320(%rdi)
pxor %xmm14, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_20
._step_20_end:

##### step 21 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 320(%rdi,%rax), %xmm14
pxor 336(%rdi), %xmm14
movdqa %xmm14, 336(%rdi)
pxor %xmm14, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_21
._step_21_end:

##### step 22 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 336(%rdi,%rax), %xmm14
pxor 352(%rdi), %xmm14
movdqa %xmm14, 352(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_22
._step_22_end:

##### step 23 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 368(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_23
._step_23_end:

##### step 24 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 240(%rdi,%r10), %xmm14
pxor 352(%rdi,%rax), %xmm14
movdqa %xmm14, 352(%rdi,%rax)
pxor 384(%rdi), %xmm14
movdqa %xmm14, 384(%rdi)
pxor %xmm14, %xmm8
pxor %xmm8, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_24
._step_24_end:

##### step 25 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 368(%rdi,%rax), %xmm14
pxor 400(%rdi), %xmm14
movdqa %xmm14, 400(%rdi)
pxor %xmm14, %xmm9
pxor %xmm9, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_25
._step_25_end:

##### step 26 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 384(%rdi,%rax), %xmm14
pxor 416(%rdi), %xmm14
movdqa %xmm14, 416(%rdi)
pxor %xmm14, %xmm10
pxor %xmm10, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_26
._step_26_end:

##### step 27 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 432(%rdi), %xmm11
pxor %xmm11, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_27
._step_27_end:

##### step 28 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 400(%rdi,%rax), %xmm14
pxor 448(%rdi), %xmm14
movdqa %xmm14, 448(%rdi)
pxor %xmm14, %xmm12
pxor %xmm12, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_28
._step_28_end:

##### step 29 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 464(%rdi), %xmm13
pxor %xmm13, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_29
._step_29_end:

##### step 30 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 480(%rdi), %xmm14
pxor 224(%rdi), %xmm14
movdqa %xmm14, 224(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_30
._step_30_end:

##### step 31 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 240(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_31
._step_31_end:

##### step 32 [hw=1]
##### F[ 1 ] ^= ( F[ 1 ] ^= ( F[ alpha + 1 ] ^= ( F[ beta + 1 ] ^= ( F[ gamma + 1 ])))))

movdqa 96(%rdi,%r11), %xmm14
pxor 256(%rdi,%r10), %xmm14
movdqa %xmm14, 256(%rdi,%r10)
pxor 416(%rdi,%rax), %xmm14
movdqa %xmm14, 416(%rdi,%rax)
pxor 496(%rdi), %xmm14
movdqa %xmm14, 496(%rdi)
pxor %xmm14, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_32
._step_32_end:

##### step 33 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 272(%rdi,%r10), %xmm14
pxor 432(%rdi,%rax), %xmm14
movdqa %xmm14, 432(%rdi,%rax)
pxor 512(%rdi), %xmm14
movdqa %xmm14, 512(%rdi)
pxor %xmm14, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_33
._step_33_end:

##### step 34 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 288(%rdi,%r10), %xmm14
pxor 448(%rdi,%rax), %xmm14
movdqa %xmm14, 448(%rdi,%rax)
pxor 528(%rdi), %xmm14
movdqa %xmm14, 528(%rdi)
pxor %xmm14, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_34
._step_34_end:

##### step 35 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 464(%rdi,%rax), %xmm14
pxor 544(%rdi), %xmm14
movdqa %xmm14, 544(%rdi)
pxor %xmm14, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_35
._step_35_end:

##### step 36 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 304(%rdi,%r10), %xmm14
pxor 480(%rdi,%rax), %xmm14
movdqa %xmm14, 480(%rdi,%rax)
pxor 560(%rdi), %xmm14
movdqa %xmm14, 560(%rdi)
pxor %xmm14, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_36
._step_36_end:

##### step 37 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 496(%rdi,%rax), %xmm14
pxor 576(%rdi), %xmm14
movdqa %xmm14, 576(%rdi)
pxor %xmm14, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_37
._step_37_end:

##### step 38 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 512(%rdi,%rax), %xmm14
pxor 592(%rdi), %xmm14
movdqa %xmm14, 592(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_38
._step_38_end:

##### step 39 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 608(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_39
._step_39_end:

##### step 40 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 320(%rdi,%r10), %xmm14
pxor 528(%rdi,%rax), %xmm14
movdqa %xmm14, 528(%rdi,%rax)
pxor 624(%rdi), %xmm14
movdqa %xmm14, 624(%rdi)
pxor %xmm14, %xmm8
pxor %xmm8, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_40
._step_40_end:

##### step 41 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 544(%rdi,%rax), %xmm14
pxor 640(%rdi), %xmm14
movdqa %xmm14, 640(%rdi)
pxor %xmm14, %xmm9
pxor %xmm9, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_41
._step_41_end:

##### step 42 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 560(%rdi,%rax), %xmm14
pxor 656(%rdi), %xmm14
movdqa %xmm14, 656(%rdi)
pxor %xmm14, %xmm10
pxor %xmm10, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_42
._step_42_end:

##### step 43 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 672(%rdi), %xmm11
pxor %xmm11, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_43
._step_43_end:

##### step 44 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 576(%rdi,%rax), %xmm14
pxor 688(%rdi), %xmm14
movdqa %xmm14, 688(%rdi)
pxor %xmm14, %xmm12
pxor %xmm12, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_44
._step_44_end:

##### step 45 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 704(%rdi), %xmm13
pxor %xmm13, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_45
._step_45_end:

##### step 46 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 720(%rdi), %xmm14
pxor 224(%rdi), %xmm14
movdqa %xmm14, 224(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_46
._step_46_end:

##### step 47 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 240(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_47
._step_47_end:

##### step 48 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 336(%rdi,%r10), %xmm14
pxor 592(%rdi,%rax), %xmm14
movdqa %xmm14, 592(%rdi,%rax)
pxor 736(%rdi), %xmm14
movdqa %xmm14, 736(%rdi)
pxor 256(%rdi), %xmm14
movdqa %xmm14, 256(%rdi)
pxor %xmm14, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_48
._step_48_end:

##### step 49 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 608(%rdi,%rax), %xmm14
pxor 752(%rdi), %xmm14
movdqa %xmm14, 752(%rdi)
pxor 272(%rdi), %xmm14
movdqa %xmm14, 272(%rdi)
pxor %xmm14, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_49
._step_49_end:

##### step 50 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 624(%rdi,%rax), %xmm14
pxor 768(%rdi), %xmm14
movdqa %xmm14, 768(%rdi)
pxor 288(%rdi), %xmm14
movdqa %xmm14, 288(%rdi)
pxor %xmm14, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_50
._step_50_end:

##### step 51 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 784(%rdi), %xmm14
pxor 304(%rdi), %xmm14
movdqa %xmm14, 304(%rdi)
pxor %xmm14, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_51
._step_51_end:

##### step 52 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 640(%rdi,%rax), %xmm14
pxor 800(%rdi), %xmm14
movdqa %xmm14, 800(%rdi)
pxor 320(%rdi), %xmm14
movdqa %xmm14, 320(%rdi)
pxor %xmm14, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_52
._step_52_end:

##### step 53 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 816(%rdi), %xmm14
pxor 336(%rdi), %xmm14
movdqa %xmm14, 336(%rdi)
pxor %xmm14, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_53
._step_53_end:

##### step 54 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 832(%rdi), %xmm14
pxor 352(%rdi), %xmm14
movdqa %xmm14, 352(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_54
._step_54_end:

##### step 55 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 368(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_55
._step_55_end:

##### step 56 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 656(%rdi,%rax), %xmm14
pxor 848(%rdi), %xmm14
movdqa %xmm14, 848(%rdi)
pxor 384(%rdi), %xmm14
movdqa %xmm14, 384(%rdi)
pxor %xmm14, %xmm8
pxor %xmm8, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_56
._step_56_end:

##### step 57 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 864(%rdi), %xmm14
pxor 400(%rdi), %xmm14
movdqa %xmm14, 400(%rdi)
pxor %xmm14, %xmm9
pxor %xmm9, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_57
._step_57_end:

##### step 58 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 880(%rdi), %xmm14
pxor 416(%rdi), %xmm14
movdqa %xmm14, 416(%rdi)
pxor %xmm14, %xmm10
pxor %xmm10, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_58
._step_58_end:

##### step 59 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 432(%rdi), %xmm11
pxor %xmm11, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_59
._step_59_end:

##### step 60 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 896(%rdi), %xmm14
pxor 448(%rdi), %xmm14
movdqa %xmm14, 448(%rdi)
pxor %xmm14, %xmm12
pxor %xmm12, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_60
._step_60_end:

##### step 61 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 464(%rdi), %xmm13
pxor %xmm13, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_61
._step_61_end:

##### step 62 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 480(%rdi), %xmm14
pxor 224(%rdi), %xmm14
movdqa %xmm14, 224(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_62
._step_62_end:

##### step 63 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 240(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_63
._step_63_end:

##### step 64 [hw=1]
##### F[ 1 ] ^= ( F[ 1 ] ^= ( F[ alpha + 1 ] ^= ( F[ beta + 1 ] ^= ( F[ gamma + 1 ])))))

movdqa 112(%rdi,%r11), %xmm14
pxor 352(%rdi,%r10), %xmm14
movdqa %xmm14, 352(%rdi,%r10)
pxor 672(%rdi,%rax), %xmm14
movdqa %xmm14, 672(%rdi,%rax)
pxor 912(%rdi), %xmm14
movdqa %xmm14, 912(%rdi)
pxor %xmm14, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_64
._step_64_end:

##### step 65 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 368(%rdi,%r10), %xmm14
pxor 688(%rdi,%rax), %xmm14
movdqa %xmm14, 688(%rdi,%rax)
pxor 928(%rdi), %xmm14
movdqa %xmm14, 928(%rdi)
pxor %xmm14, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_65
._step_65_end:

##### step 66 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 384(%rdi,%r10), %xmm14
pxor 704(%rdi,%rax), %xmm14
movdqa %xmm14, 704(%rdi,%rax)
pxor 944(%rdi), %xmm14
movdqa %xmm14, 944(%rdi)
pxor %xmm14, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_66
._step_66_end:

##### step 67 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 720(%rdi,%rax), %xmm14
pxor 960(%rdi), %xmm14
movdqa %xmm14, 960(%rdi)
pxor %xmm14, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_67
._step_67_end:

##### step 68 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 400(%rdi,%r10), %xmm14
pxor 736(%rdi,%rax), %xmm14
movdqa %xmm14, 736(%rdi,%rax)
pxor 976(%rdi), %xmm14
movdqa %xmm14, 976(%rdi)
pxor %xmm14, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_68
._step_68_end:

##### step 69 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 752(%rdi,%rax), %xmm14
pxor 992(%rdi), %xmm14
movdqa %xmm14, 992(%rdi)
pxor %xmm14, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_69
._step_69_end:

##### step 70 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 768(%rdi,%rax), %xmm14
pxor 1008(%rdi), %xmm14
movdqa %xmm14, 1008(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_70
._step_70_end:

##### step 71 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 1024(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_71
._step_71_end:

##### step 72 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 416(%rdi,%r10), %xmm14
pxor 784(%rdi,%rax), %xmm14
movdqa %xmm14, 784(%rdi,%rax)
pxor 1040(%rdi), %xmm14
movdqa %xmm14, 1040(%rdi)
pxor %xmm14, %xmm8
pxor %xmm8, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_72
._step_72_end:

##### step 73 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 800(%rdi,%rax), %xmm14
pxor 1056(%rdi), %xmm14
movdqa %xmm14, 1056(%rdi)
pxor %xmm14, %xmm9
pxor %xmm9, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_73
._step_73_end:

##### step 74 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 816(%rdi,%rax), %xmm14
pxor 1072(%rdi), %xmm14
movdqa %xmm14, 1072(%rdi)
pxor %xmm14, %xmm10
pxor %xmm10, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_74
._step_74_end:

##### step 75 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 1088(%rdi), %xmm11
pxor %xmm11, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_75
._step_75_end:

##### step 76 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 832(%rdi,%rax), %xmm14
pxor 1104(%rdi), %xmm14
movdqa %xmm14, 1104(%rdi)
pxor %xmm14, %xmm12
pxor %xmm12, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_76
._step_76_end:

##### step 77 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 1120(%rdi), %xmm13
pxor %xmm13, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_77
._step_77_end:

##### step 78 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1136(%rdi), %xmm14
pxor 224(%rdi), %xmm14
movdqa %xmm14, 224(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_78
._step_78_end:

##### step 79 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 240(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_79
._step_79_end:

##### step 80 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 432(%rdi,%r10), %xmm14
pxor 848(%rdi,%rax), %xmm14
movdqa %xmm14, 848(%rdi,%rax)
pxor 1152(%rdi), %xmm14
movdqa %xmm14, 1152(%rdi)
pxor 256(%rdi), %xmm14
movdqa %xmm14, 256(%rdi)
pxor %xmm14, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_80
._step_80_end:

##### step 81 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 864(%rdi,%rax), %xmm14
pxor 1168(%rdi), %xmm14
movdqa %xmm14, 1168(%rdi)
pxor 272(%rdi), %xmm14
movdqa %xmm14, 272(%rdi)
pxor %xmm14, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_81
._step_81_end:

##### step 82 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 880(%rdi,%rax), %xmm14
pxor 1184(%rdi), %xmm14
movdqa %xmm14, 1184(%rdi)
pxor 288(%rdi), %xmm14
movdqa %xmm14, 288(%rdi)
pxor %xmm14, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_82
._step_82_end:

##### step 83 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1200(%rdi), %xmm14
pxor 304(%rdi), %xmm14
movdqa %xmm14, 304(%rdi)
pxor %xmm14, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_83
._step_83_end:

##### step 84 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 896(%rdi,%rax), %xmm14
pxor 1216(%rdi), %xmm14
movdqa %xmm14, 1216(%rdi)
pxor 320(%rdi), %xmm14
movdqa %xmm14, 320(%rdi)
pxor %xmm14, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_84
._step_84_end:

##### step 85 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1232(%rdi), %xmm14
pxor 336(%rdi), %xmm14
movdqa %xmm14, 336(%rdi)
pxor %xmm14, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_85
._step_85_end:

##### step 86 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1248(%rdi), %xmm14
pxor 352(%rdi), %xmm14
movdqa %xmm14, 352(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_86
._step_86_end:

##### step 87 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 368(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_87
._step_87_end:

##### step 88 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 912(%rdi,%rax), %xmm14
pxor 1264(%rdi), %xmm14
movdqa %xmm14, 1264(%rdi)
pxor 384(%rdi), %xmm14
movdqa %xmm14, 384(%rdi)
pxor %xmm14, %xmm8
pxor %xmm8, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_88
._step_88_end:

##### step 89 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1280(%rdi), %xmm14
pxor 400(%rdi), %xmm14
movdqa %xmm14, 400(%rdi)
pxor %xmm14, %xmm9
pxor %xmm9, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_89
._step_89_end:

##### step 90 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1296(%rdi), %xmm14
pxor 416(%rdi), %xmm14
movdqa %xmm14, 416(%rdi)
pxor %xmm14, %xmm10
pxor %xmm10, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_90
._step_90_end:

##### step 91 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 432(%rdi), %xmm11
pxor %xmm11, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_91
._step_91_end:

##### step 92 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1312(%rdi), %xmm14
pxor 448(%rdi), %xmm14
movdqa %xmm14, 448(%rdi)
pxor %xmm14, %xmm12
pxor %xmm12, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_92
._step_92_end:

##### step 93 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 464(%rdi), %xmm13
pxor %xmm13, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_93
._step_93_end:

##### step 94 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 480(%rdi), %xmm14
pxor 224(%rdi), %xmm14
movdqa %xmm14, 224(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_94
._step_94_end:

##### step 95 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 240(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_95
._step_95_end:

##### step 96 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 448(%rdi,%r10), %xmm14
pxor 928(%rdi,%rax), %xmm14
movdqa %xmm14, 928(%rdi,%rax)
pxor 1328(%rdi), %xmm14
movdqa %xmm14, 1328(%rdi)
pxor 496(%rdi), %xmm14
movdqa %xmm14, 496(%rdi)
pxor %xmm14, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_96
._step_96_end:

##### step 97 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 944(%rdi,%rax), %xmm14
pxor 1344(%rdi), %xmm14
movdqa %xmm14, 1344(%rdi)
pxor 512(%rdi), %xmm14
movdqa %xmm14, 512(%rdi)
pxor %xmm14, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_97
._step_97_end:

##### step 98 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 960(%rdi,%rax), %xmm14
pxor 1360(%rdi), %xmm14
movdqa %xmm14, 1360(%rdi)
pxor 528(%rdi), %xmm14
movdqa %xmm14, 528(%rdi)
pxor %xmm14, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_98
._step_98_end:

##### step 99 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1376(%rdi), %xmm14
pxor 544(%rdi), %xmm14
movdqa %xmm14, 544(%rdi)
pxor %xmm14, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_99
._step_99_end:

##### step 100 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 976(%rdi,%rax), %xmm14
pxor 1392(%rdi), %xmm14
movdqa %xmm14, 1392(%rdi)
pxor 560(%rdi), %xmm14
movdqa %xmm14, 560(%rdi)
pxor %xmm14, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_100
._step_100_end:

##### step 101 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1408(%rdi), %xmm14
pxor 576(%rdi), %xmm14
movdqa %xmm14, 576(%rdi)
pxor %xmm14, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_101
._step_101_end:

##### step 102 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1424(%rdi), %xmm14
pxor 592(%rdi), %xmm14
movdqa %xmm14, 592(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_102
._step_102_end:

##### step 103 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 608(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_103
._step_103_end:

##### step 104 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 992(%rdi,%rax), %xmm14
pxor 1440(%rdi), %xmm14
movdqa %xmm14, 1440(%rdi)
pxor 624(%rdi), %xmm14
movdqa %xmm14, 624(%rdi)
pxor %xmm14, %xmm8
pxor %xmm8, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_104
._step_104_end:

##### step 105 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1456(%rdi), %xmm14
pxor 640(%rdi), %xmm14
movdqa %xmm14, 640(%rdi)
pxor %xmm14, %xmm9
pxor %xmm9, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_105
._step_105_end:

##### step 106 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1472(%rdi), %xmm14
pxor 656(%rdi), %xmm14
movdqa %xmm14, 656(%rdi)
pxor %xmm14, %xmm10
pxor %xmm10, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_106
._step_106_end:

##### step 107 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 672(%rdi), %xmm11
pxor %xmm11, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_107
._step_107_end:

##### step 108 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1488(%rdi), %xmm14
pxor 688(%rdi), %xmm14
movdqa %xmm14, 688(%rdi)
pxor %xmm14, %xmm12
pxor %xmm12, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_108
._step_108_end:

##### step 109 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 704(%rdi), %xmm13
pxor %xmm13, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_109
._step_109_end:

##### step 110 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 720(%rdi), %xmm14
pxor 224(%rdi), %xmm14
movdqa %xmm14, 224(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_110
._step_110_end:

##### step 111 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 240(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_111
._step_111_end:

##### step 112 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 1008(%rdi,%rax), %xmm14
pxor 1504(%rdi), %xmm14
movdqa %xmm14, 1504(%rdi)
pxor 736(%rdi), %xmm14
movdqa %xmm14, 736(%rdi)
pxor 256(%rdi), %xmm14
movdqa %xmm14, 256(%rdi)
pxor %xmm14, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_112
._step_112_end:

##### step 113 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1520(%rdi), %xmm14
pxor 752(%rdi), %xmm14
movdqa %xmm14, 752(%rdi)
pxor 272(%rdi), %xmm14
movdqa %xmm14, 272(%rdi)
pxor %xmm14, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_113
._step_113_end:

##### step 114 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1536(%rdi), %xmm14
pxor 768(%rdi), %xmm14
movdqa %xmm14, 768(%rdi)
pxor 288(%rdi), %xmm14
movdqa %xmm14, 288(%rdi)
pxor %xmm14, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_114
._step_114_end:

##### step 115 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 784(%rdi), %xmm14
pxor 304(%rdi), %xmm14
movdqa %xmm14, 304(%rdi)
pxor %xmm14, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_115
._step_115_end:

##### step 116 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1552(%rdi), %xmm14
pxor 800(%rdi), %xmm14
movdqa %xmm14, 800(%rdi)
pxor 320(%rdi), %xmm14
movdqa %xmm14, 320(%rdi)
pxor %xmm14, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_116
._step_116_end:

##### step 117 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 816(%rdi), %xmm14
pxor 336(%rdi), %xmm14
movdqa %xmm14, 336(%rdi)
pxor %xmm14, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_117
._step_117_end:

##### step 118 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 832(%rdi), %xmm14
pxor 352(%rdi), %xmm14
movdqa %xmm14, 352(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_118
._step_118_end:

##### step 119 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 368(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_119
._step_119_end:

##### step 120 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1568(%rdi), %xmm14
pxor 848(%rdi), %xmm14
movdqa %xmm14, 848(%rdi)
pxor 384(%rdi), %xmm14
movdqa %xmm14, 384(%rdi)
pxor %xmm14, %xmm8
pxor %xmm8, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_120
._step_120_end:

##### step 121 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 864(%rdi), %xmm14
pxor 400(%rdi), %xmm14
movdqa %xmm14, 400(%rdi)
pxor %xmm14, %xmm9
pxor %xmm9, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_121
._step_121_end:

##### step 122 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 880(%rdi), %xmm14
pxor 416(%rdi), %xmm14
movdqa %xmm14, 416(%rdi)
pxor %xmm14, %xmm10
pxor %xmm10, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_122
._step_122_end:

##### step 123 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 432(%rdi), %xmm11
pxor %xmm11, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_123
._step_123_end:

##### step 124 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 896(%rdi), %xmm14
pxor 448(%rdi), %xmm14
movdqa %xmm14, 448(%rdi)
pxor %xmm14, %xmm12
pxor %xmm12, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_124
._step_124_end:

##### step 125 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 464(%rdi), %xmm13
pxor %xmm13, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_125
._step_125_end:

##### step 126 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 480(%rdi), %xmm14
pxor 224(%rdi), %xmm14
movdqa %xmm14, 224(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_126
._step_126_end:

##### step 127 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 240(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_127
._step_127_end:

##### step 128 [hw=1]
##### F[ 1 ] ^= ( F[ 1 ] ^= ( F[ alpha + 1 ] ^= ( F[ beta + 1 ] ^= ( F[ gamma + 1 ])))))

movdqa 128(%rdi,%r11), %xmm14
pxor 464(%rdi,%r10), %xmm14
movdqa %xmm14, 464(%rdi,%r10)
pxor 1024(%rdi,%rax), %xmm14
movdqa %xmm14, 1024(%rdi,%rax)
pxor 1584(%rdi), %xmm14
movdqa %xmm14, 1584(%rdi)
pxor %xmm14, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_128
._step_128_end:

##### step 129 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 480(%rdi,%r10), %xmm14
pxor 1040(%rdi,%rax), %xmm14
movdqa %xmm14, 1040(%rdi,%rax)
pxor 1600(%rdi), %xmm14
movdqa %xmm14, 1600(%rdi)
pxor %xmm14, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_129
._step_129_end:

##### step 130 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 496(%rdi,%r10), %xmm14
pxor 1056(%rdi,%rax), %xmm14
movdqa %xmm14, 1056(%rdi,%rax)
pxor 1616(%rdi), %xmm14
movdqa %xmm14, 1616(%rdi)
pxor %xmm14, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_130
._step_130_end:

##### step 131 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 1072(%rdi,%rax), %xmm14
pxor 1632(%rdi), %xmm14
movdqa %xmm14, 1632(%rdi)
pxor %xmm14, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_131
._step_131_end:

##### step 132 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 512(%rdi,%r10), %xmm14
pxor 1088(%rdi,%rax), %xmm14
movdqa %xmm14, 1088(%rdi,%rax)
pxor 1648(%rdi), %xmm14
movdqa %xmm14, 1648(%rdi)
pxor %xmm14, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_132
._step_132_end:

##### step 133 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 1104(%rdi,%rax), %xmm14
pxor 1664(%rdi), %xmm14
movdqa %xmm14, 1664(%rdi)
pxor %xmm14, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_133
._step_133_end:

##### step 134 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 1120(%rdi,%rax), %xmm14
pxor 1680(%rdi), %xmm14
movdqa %xmm14, 1680(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_134
._step_134_end:

##### step 135 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 1696(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_135
._step_135_end:

##### step 136 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 528(%rdi,%r10), %xmm14
pxor 1136(%rdi,%rax), %xmm14
movdqa %xmm14, 1136(%rdi,%rax)
pxor 1712(%rdi), %xmm14
movdqa %xmm14, 1712(%rdi)
pxor %xmm14, %xmm8
pxor %xmm8, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_136
._step_136_end:

##### step 137 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 1152(%rdi,%rax), %xmm14
pxor 1728(%rdi), %xmm14
movdqa %xmm14, 1728(%rdi)
pxor %xmm14, %xmm9
pxor %xmm9, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_137
._step_137_end:

##### step 138 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 1168(%rdi,%rax), %xmm14
pxor 1744(%rdi), %xmm14
movdqa %xmm14, 1744(%rdi)
pxor %xmm14, %xmm10
pxor %xmm10, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_138
._step_138_end:

##### step 139 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 1760(%rdi), %xmm11
pxor %xmm11, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_139
._step_139_end:

##### step 140 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 1184(%rdi,%rax), %xmm14
pxor 1776(%rdi), %xmm14
movdqa %xmm14, 1776(%rdi)
pxor %xmm14, %xmm12
pxor %xmm12, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_140
._step_140_end:

##### step 141 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 1792(%rdi), %xmm13
pxor %xmm13, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_141
._step_141_end:

##### step 142 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1808(%rdi), %xmm14
pxor 224(%rdi), %xmm14
movdqa %xmm14, 224(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_142
._step_142_end:

##### step 143 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 240(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_143
._step_143_end:

##### step 144 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 544(%rdi,%r10), %xmm14
pxor 1200(%rdi,%rax), %xmm14
movdqa %xmm14, 1200(%rdi,%rax)
pxor 1824(%rdi), %xmm14
movdqa %xmm14, 1824(%rdi)
pxor 256(%rdi), %xmm14
movdqa %xmm14, 256(%rdi)
pxor %xmm14, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_144
._step_144_end:

##### step 145 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 1216(%rdi,%rax), %xmm14
pxor 1840(%rdi), %xmm14
movdqa %xmm14, 1840(%rdi)
pxor 272(%rdi), %xmm14
movdqa %xmm14, 272(%rdi)
pxor %xmm14, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_145
._step_145_end:

##### step 146 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 1232(%rdi,%rax), %xmm14
pxor 1856(%rdi), %xmm14
movdqa %xmm14, 1856(%rdi)
pxor 288(%rdi), %xmm14
movdqa %xmm14, 288(%rdi)
pxor %xmm14, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_146
._step_146_end:

##### step 147 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1872(%rdi), %xmm14
pxor 304(%rdi), %xmm14
movdqa %xmm14, 304(%rdi)
pxor %xmm14, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_147
._step_147_end:

##### step 148 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 1248(%rdi,%rax), %xmm14
pxor 1888(%rdi), %xmm14
movdqa %xmm14, 1888(%rdi)
pxor 320(%rdi), %xmm14
movdqa %xmm14, 320(%rdi)
pxor %xmm14, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_148
._step_148_end:

##### step 149 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1904(%rdi), %xmm14
pxor 336(%rdi), %xmm14
movdqa %xmm14, 336(%rdi)
pxor %xmm14, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_149
._step_149_end:

##### step 150 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1920(%rdi), %xmm14
pxor 352(%rdi), %xmm14
movdqa %xmm14, 352(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_150
._step_150_end:

##### step 151 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 368(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_151
._step_151_end:

##### step 152 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 1264(%rdi,%rax), %xmm14
pxor 1936(%rdi), %xmm14
movdqa %xmm14, 1936(%rdi)
pxor 384(%rdi), %xmm14
movdqa %xmm14, 384(%rdi)
pxor %xmm14, %xmm8
pxor %xmm8, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_152
._step_152_end:

##### step 153 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1952(%rdi), %xmm14
pxor 400(%rdi), %xmm14
movdqa %xmm14, 400(%rdi)
pxor %xmm14, %xmm9
pxor %xmm9, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_153
._step_153_end:

##### step 154 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1968(%rdi), %xmm14
pxor 416(%rdi), %xmm14
movdqa %xmm14, 416(%rdi)
pxor %xmm14, %xmm10
pxor %xmm10, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_154
._step_154_end:

##### step 155 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 432(%rdi), %xmm11
pxor %xmm11, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_155
._step_155_end:

##### step 156 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1984(%rdi), %xmm14
pxor 448(%rdi), %xmm14
movdqa %xmm14, 448(%rdi)
pxor %xmm14, %xmm12
pxor %xmm12, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_156
._step_156_end:

##### step 157 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 464(%rdi), %xmm13
pxor %xmm13, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_157
._step_157_end:

##### step 158 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 480(%rdi), %xmm14
pxor 224(%rdi), %xmm14
movdqa %xmm14, 224(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_158
._step_158_end:

##### step 159 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 240(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_159
._step_159_end:

##### step 160 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 560(%rdi,%r10), %xmm14
pxor 1280(%rdi,%rax), %xmm14
movdqa %xmm14, 1280(%rdi,%rax)
pxor 2000(%rdi), %xmm14
movdqa %xmm14, 2000(%rdi)
pxor 496(%rdi), %xmm14
movdqa %xmm14, 496(%rdi)
pxor %xmm14, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_160
._step_160_end:

##### step 161 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 1296(%rdi,%rax), %xmm14
pxor 2016(%rdi), %xmm14
movdqa %xmm14, 2016(%rdi)
pxor 512(%rdi), %xmm14
movdqa %xmm14, 512(%rdi)
pxor %xmm14, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_161
._step_161_end:

##### step 162 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 1312(%rdi,%rax), %xmm14
pxor 2032(%rdi), %xmm14
movdqa %xmm14, 2032(%rdi)
pxor 528(%rdi), %xmm14
movdqa %xmm14, 528(%rdi)
pxor %xmm14, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_162
._step_162_end:

##### step 163 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2048(%rdi), %xmm14
pxor 544(%rdi), %xmm14
movdqa %xmm14, 544(%rdi)
pxor %xmm14, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_163
._step_163_end:

##### step 164 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 1328(%rdi,%rax), %xmm14
pxor 2064(%rdi), %xmm14
movdqa %xmm14, 2064(%rdi)
pxor 560(%rdi), %xmm14
movdqa %xmm14, 560(%rdi)
pxor %xmm14, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_164
._step_164_end:

##### step 165 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2080(%rdi), %xmm14
pxor 576(%rdi), %xmm14
movdqa %xmm14, 576(%rdi)
pxor %xmm14, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_165
._step_165_end:

##### step 166 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2096(%rdi), %xmm14
pxor 592(%rdi), %xmm14
movdqa %xmm14, 592(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_166
._step_166_end:

##### step 167 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 608(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_167
._step_167_end:

##### step 168 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 1344(%rdi,%rax), %xmm14
pxor 2112(%rdi), %xmm14
movdqa %xmm14, 2112(%rdi)
pxor 624(%rdi), %xmm14
movdqa %xmm14, 624(%rdi)
pxor %xmm14, %xmm8
pxor %xmm8, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_168
._step_168_end:

##### step 169 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2128(%rdi), %xmm14
pxor 640(%rdi), %xmm14
movdqa %xmm14, 640(%rdi)
pxor %xmm14, %xmm9
pxor %xmm9, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_169
._step_169_end:

##### step 170 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2144(%rdi), %xmm14
pxor 656(%rdi), %xmm14
movdqa %xmm14, 656(%rdi)
pxor %xmm14, %xmm10
pxor %xmm10, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_170
._step_170_end:

##### step 171 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 672(%rdi), %xmm11
pxor %xmm11, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_171
._step_171_end:

##### step 172 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2160(%rdi), %xmm14
pxor 688(%rdi), %xmm14
movdqa %xmm14, 688(%rdi)
pxor %xmm14, %xmm12
pxor %xmm12, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_172
._step_172_end:

##### step 173 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 704(%rdi), %xmm13
pxor %xmm13, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_173
._step_173_end:

##### step 174 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 720(%rdi), %xmm14
pxor 224(%rdi), %xmm14
movdqa %xmm14, 224(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_174
._step_174_end:

##### step 175 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 240(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_175
._step_175_end:

##### step 176 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 1360(%rdi,%rax), %xmm14
pxor 2176(%rdi), %xmm14
movdqa %xmm14, 2176(%rdi)
pxor 736(%rdi), %xmm14
movdqa %xmm14, 736(%rdi)
pxor 256(%rdi), %xmm14
movdqa %xmm14, 256(%rdi)
pxor %xmm14, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_176
._step_176_end:

##### step 177 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2192(%rdi), %xmm14
pxor 752(%rdi), %xmm14
movdqa %xmm14, 752(%rdi)
pxor 272(%rdi), %xmm14
movdqa %xmm14, 272(%rdi)
pxor %xmm14, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_177
._step_177_end:

##### step 178 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2208(%rdi), %xmm14
pxor 768(%rdi), %xmm14
movdqa %xmm14, 768(%rdi)
pxor 288(%rdi), %xmm14
movdqa %xmm14, 288(%rdi)
pxor %xmm14, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_178
._step_178_end:

##### step 179 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 784(%rdi), %xmm14
pxor 304(%rdi), %xmm14
movdqa %xmm14, 304(%rdi)
pxor %xmm14, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_179
._step_179_end:

##### step 180 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2224(%rdi), %xmm14
pxor 800(%rdi), %xmm14
movdqa %xmm14, 800(%rdi)
pxor 320(%rdi), %xmm14
movdqa %xmm14, 320(%rdi)
pxor %xmm14, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_180
._step_180_end:

##### step 181 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 816(%rdi), %xmm14
pxor 336(%rdi), %xmm14
movdqa %xmm14, 336(%rdi)
pxor %xmm14, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_181
._step_181_end:

##### step 182 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 832(%rdi), %xmm14
pxor 352(%rdi), %xmm14
movdqa %xmm14, 352(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_182
._step_182_end:

##### step 183 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 368(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_183
._step_183_end:

##### step 184 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2240(%rdi), %xmm14
pxor 848(%rdi), %xmm14
movdqa %xmm14, 848(%rdi)
pxor 384(%rdi), %xmm14
movdqa %xmm14, 384(%rdi)
pxor %xmm14, %xmm8
pxor %xmm8, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_184
._step_184_end:

##### step 185 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 864(%rdi), %xmm14
pxor 400(%rdi), %xmm14
movdqa %xmm14, 400(%rdi)
pxor %xmm14, %xmm9
pxor %xmm9, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_185
._step_185_end:

##### step 186 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 880(%rdi), %xmm14
pxor 416(%rdi), %xmm14
movdqa %xmm14, 416(%rdi)
pxor %xmm14, %xmm10
pxor %xmm10, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_186
._step_186_end:

##### step 187 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 432(%rdi), %xmm11
pxor %xmm11, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_187
._step_187_end:

##### step 188 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 896(%rdi), %xmm14
pxor 448(%rdi), %xmm14
movdqa %xmm14, 448(%rdi)
pxor %xmm14, %xmm12
pxor %xmm12, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_188
._step_188_end:

##### step 189 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 464(%rdi), %xmm13
pxor %xmm13, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_189
._step_189_end:

##### step 190 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 480(%rdi), %xmm14
pxor 224(%rdi), %xmm14
movdqa %xmm14, 224(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_190
._step_190_end:

##### step 191 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 240(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_191
._step_191_end:

##### step 192 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ])))))

movdqa 576(%rdi,%r10), %xmm14
pxor 1376(%rdi,%rax), %xmm14
movdqa %xmm14, 1376(%rdi,%rax)
pxor 2256(%rdi), %xmm14
movdqa %xmm14, 2256(%rdi)
pxor 912(%rdi), %xmm14
movdqa %xmm14, 912(%rdi)
pxor %xmm14, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_192
._step_192_end:

##### step 193 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 1392(%rdi,%rax), %xmm14
pxor 2272(%rdi), %xmm14
movdqa %xmm14, 2272(%rdi)
pxor 928(%rdi), %xmm14
movdqa %xmm14, 928(%rdi)
pxor %xmm14, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_193
._step_193_end:

##### step 194 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 1408(%rdi,%rax), %xmm14
pxor 2288(%rdi), %xmm14
movdqa %xmm14, 2288(%rdi)
pxor 944(%rdi), %xmm14
movdqa %xmm14, 944(%rdi)
pxor %xmm14, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_194
._step_194_end:

##### step 195 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2304(%rdi), %xmm14
pxor 960(%rdi), %xmm14
movdqa %xmm14, 960(%rdi)
pxor %xmm14, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_195
._step_195_end:

##### step 196 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 1424(%rdi,%rax), %xmm14
pxor 2320(%rdi), %xmm14
movdqa %xmm14, 2320(%rdi)
pxor 976(%rdi), %xmm14
movdqa %xmm14, 976(%rdi)
pxor %xmm14, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_196
._step_196_end:

##### step 197 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2336(%rdi), %xmm14
pxor 992(%rdi), %xmm14
movdqa %xmm14, 992(%rdi)
pxor %xmm14, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_197
._step_197_end:

##### step 198 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2352(%rdi), %xmm14
pxor 1008(%rdi), %xmm14
movdqa %xmm14, 1008(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_198
._step_198_end:

##### step 199 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 1024(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_199
._step_199_end:

##### step 200 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 1440(%rdi,%rax), %xmm14
pxor 2368(%rdi), %xmm14
movdqa %xmm14, 2368(%rdi)
pxor 1040(%rdi), %xmm14
movdqa %xmm14, 1040(%rdi)
pxor %xmm14, %xmm8
pxor %xmm8, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_200
._step_200_end:

##### step 201 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2384(%rdi), %xmm14
pxor 1056(%rdi), %xmm14
movdqa %xmm14, 1056(%rdi)
pxor %xmm14, %xmm9
pxor %xmm9, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_201
._step_201_end:

##### step 202 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2400(%rdi), %xmm14
pxor 1072(%rdi), %xmm14
movdqa %xmm14, 1072(%rdi)
pxor %xmm14, %xmm10
pxor %xmm10, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_202
._step_202_end:

##### step 203 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 1088(%rdi), %xmm11
pxor %xmm11, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_203
._step_203_end:

##### step 204 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2416(%rdi), %xmm14
pxor 1104(%rdi), %xmm14
movdqa %xmm14, 1104(%rdi)
pxor %xmm14, %xmm12
pxor %xmm12, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_204
._step_204_end:

##### step 205 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 1120(%rdi), %xmm13
pxor %xmm13, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_205
._step_205_end:

##### step 206 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1136(%rdi), %xmm14
pxor 224(%rdi), %xmm14
movdqa %xmm14, 224(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_206
._step_206_end:

##### step 207 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 240(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_207
._step_207_end:

##### step 208 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 1456(%rdi,%rax), %xmm14
pxor 2432(%rdi), %xmm14
movdqa %xmm14, 2432(%rdi)
pxor 1152(%rdi), %xmm14
movdqa %xmm14, 1152(%rdi)
pxor 256(%rdi), %xmm14
movdqa %xmm14, 256(%rdi)
pxor %xmm14, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_208
._step_208_end:

##### step 209 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2448(%rdi), %xmm14
pxor 1168(%rdi), %xmm14
movdqa %xmm14, 1168(%rdi)
pxor 272(%rdi), %xmm14
movdqa %xmm14, 272(%rdi)
pxor %xmm14, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_209
._step_209_end:

##### step 210 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2464(%rdi), %xmm14
pxor 1184(%rdi), %xmm14
movdqa %xmm14, 1184(%rdi)
pxor 288(%rdi), %xmm14
movdqa %xmm14, 288(%rdi)
pxor %xmm14, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_210
._step_210_end:

##### step 211 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1200(%rdi), %xmm14
pxor 304(%rdi), %xmm14
movdqa %xmm14, 304(%rdi)
pxor %xmm14, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_211
._step_211_end:

##### step 212 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2480(%rdi), %xmm14
pxor 1216(%rdi), %xmm14
movdqa %xmm14, 1216(%rdi)
pxor 320(%rdi), %xmm14
movdqa %xmm14, 320(%rdi)
pxor %xmm14, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_212
._step_212_end:

##### step 213 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1232(%rdi), %xmm14
pxor 336(%rdi), %xmm14
movdqa %xmm14, 336(%rdi)
pxor %xmm14, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_213
._step_213_end:

##### step 214 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1248(%rdi), %xmm14
pxor 352(%rdi), %xmm14
movdqa %xmm14, 352(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_214
._step_214_end:

##### step 215 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 368(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_215
._step_215_end:

##### step 216 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2496(%rdi), %xmm14
pxor 1264(%rdi), %xmm14
movdqa %xmm14, 1264(%rdi)
pxor 384(%rdi), %xmm14
movdqa %xmm14, 384(%rdi)
pxor %xmm14, %xmm8
pxor %xmm8, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_216
._step_216_end:

##### step 217 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1280(%rdi), %xmm14
pxor 400(%rdi), %xmm14
movdqa %xmm14, 400(%rdi)
pxor %xmm14, %xmm9
pxor %xmm9, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_217
._step_217_end:

##### step 218 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1296(%rdi), %xmm14
pxor 416(%rdi), %xmm14
movdqa %xmm14, 416(%rdi)
pxor %xmm14, %xmm10
pxor %xmm10, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_218
._step_218_end:

##### step 219 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 432(%rdi), %xmm11
pxor %xmm11, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_219
._step_219_end:

##### step 220 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1312(%rdi), %xmm14
pxor 448(%rdi), %xmm14
movdqa %xmm14, 448(%rdi)
pxor %xmm14, %xmm12
pxor %xmm12, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_220
._step_220_end:

##### step 221 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 464(%rdi), %xmm13
pxor %xmm13, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_221
._step_221_end:

##### step 222 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 480(%rdi), %xmm14
pxor 224(%rdi), %xmm14
movdqa %xmm14, 224(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_222
._step_222_end:

##### step 223 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 240(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_223
._step_223_end:

##### step 224 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ])))))

movdqa 1472(%rdi,%rax), %xmm14
pxor 2512(%rdi), %xmm14
movdqa %xmm14, 2512(%rdi)
pxor 1328(%rdi), %xmm14
movdqa %xmm14, 1328(%rdi)
pxor 496(%rdi), %xmm14
movdqa %xmm14, 496(%rdi)
pxor %xmm14, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_224
._step_224_end:

##### step 225 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2528(%rdi), %xmm14
pxor 1344(%rdi), %xmm14
movdqa %xmm14, 1344(%rdi)
pxor 512(%rdi), %xmm14
movdqa %xmm14, 512(%rdi)
pxor %xmm14, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_225
._step_225_end:

##### step 226 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2544(%rdi), %xmm14
pxor 1360(%rdi), %xmm14
movdqa %xmm14, 1360(%rdi)
pxor 528(%rdi), %xmm14
movdqa %xmm14, 528(%rdi)
pxor %xmm14, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_226
._step_226_end:

##### step 227 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1376(%rdi), %xmm14
pxor 544(%rdi), %xmm14
movdqa %xmm14, 544(%rdi)
pxor %xmm14, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_227
._step_227_end:

##### step 228 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2560(%rdi), %xmm14
pxor 1392(%rdi), %xmm14
movdqa %xmm14, 1392(%rdi)
pxor 560(%rdi), %xmm14
movdqa %xmm14, 560(%rdi)
pxor %xmm14, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_228
._step_228_end:

##### step 229 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1408(%rdi), %xmm14
pxor 576(%rdi), %xmm14
movdqa %xmm14, 576(%rdi)
pxor %xmm14, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_229
._step_229_end:

##### step 230 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1424(%rdi), %xmm14
pxor 592(%rdi), %xmm14
movdqa %xmm14, 592(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_230
._step_230_end:

##### step 231 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 608(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_231
._step_231_end:

##### step 232 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2576(%rdi), %xmm14
pxor 1440(%rdi), %xmm14
movdqa %xmm14, 1440(%rdi)
pxor 624(%rdi), %xmm14
movdqa %xmm14, 624(%rdi)
pxor %xmm14, %xmm8
pxor %xmm8, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_232
._step_232_end:

##### step 233 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1456(%rdi), %xmm14
pxor 640(%rdi), %xmm14
movdqa %xmm14, 640(%rdi)
pxor %xmm14, %xmm9
pxor %xmm9, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_233
._step_233_end:

##### step 234 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1472(%rdi), %xmm14
pxor 656(%rdi), %xmm14
movdqa %xmm14, 656(%rdi)
pxor %xmm14, %xmm10
pxor %xmm10, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_234
._step_234_end:

##### step 235 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 672(%rdi), %xmm11
pxor %xmm11, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_235
._step_235_end:

##### step 236 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1488(%rdi), %xmm14
pxor 688(%rdi), %xmm14
movdqa %xmm14, 688(%rdi)
pxor %xmm14, %xmm12
pxor %xmm12, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_236
._step_236_end:

##### step 237 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 704(%rdi), %xmm13
pxor %xmm13, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_237
._step_237_end:

##### step 238 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 720(%rdi), %xmm14
pxor 224(%rdi), %xmm14
movdqa %xmm14, 224(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_238
._step_238_end:

##### step 239 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 240(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_239
._step_239_end:

##### step 240 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 2592(%rdi), %xmm14
pxor 1504(%rdi), %xmm14
movdqa %xmm14, 1504(%rdi)
pxor 736(%rdi), %xmm14
movdqa %xmm14, 736(%rdi)
pxor 256(%rdi), %xmm14
movdqa %xmm14, 256(%rdi)
pxor %xmm14, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_240
._step_240_end:

##### step 241 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1520(%rdi), %xmm14
pxor 752(%rdi), %xmm14
movdqa %xmm14, 752(%rdi)
pxor 272(%rdi), %xmm14
movdqa %xmm14, 272(%rdi)
pxor %xmm14, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_241
._step_241_end:

##### step 242 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1536(%rdi), %xmm14
pxor 768(%rdi), %xmm14
movdqa %xmm14, 768(%rdi)
pxor 288(%rdi), %xmm14
movdqa %xmm14, 288(%rdi)
pxor %xmm14, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_242
._step_242_end:

##### step 243 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 784(%rdi), %xmm14
pxor 304(%rdi), %xmm14
movdqa %xmm14, 304(%rdi)
pxor %xmm14, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_243
._step_243_end:

##### step 244 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1552(%rdi), %xmm14
pxor 800(%rdi), %xmm14
movdqa %xmm14, 800(%rdi)
pxor 320(%rdi), %xmm14
movdqa %xmm14, 320(%rdi)
pxor %xmm14, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_244
._step_244_end:

##### step 245 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 816(%rdi), %xmm14
pxor 336(%rdi), %xmm14
movdqa %xmm14, 336(%rdi)
pxor %xmm14, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_245
._step_245_end:

##### step 246 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 832(%rdi), %xmm14
pxor 352(%rdi), %xmm14
movdqa %xmm14, 352(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_246
._step_246_end:

##### step 247 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 368(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_247
._step_247_end:

##### step 248 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 1568(%rdi), %xmm14
pxor 848(%rdi), %xmm14
movdqa %xmm14, 848(%rdi)
pxor 384(%rdi), %xmm14
movdqa %xmm14, 384(%rdi)
pxor %xmm14, %xmm8
pxor %xmm8, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_248
._step_248_end:

##### step 249 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 864(%rdi), %xmm14
pxor 400(%rdi), %xmm14
movdqa %xmm14, 400(%rdi)
pxor %xmm14, %xmm9
pxor %xmm9, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_249
._step_249_end:

##### step 250 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 880(%rdi), %xmm14
pxor 416(%rdi), %xmm14
movdqa %xmm14, 416(%rdi)
pxor %xmm14, %xmm10
pxor %xmm10, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_250
._step_250_end:

##### step 251 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 432(%rdi), %xmm11
pxor %xmm11, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_251
._step_251_end:

##### step 252 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 896(%rdi), %xmm14
pxor 448(%rdi), %xmm14
movdqa %xmm14, 448(%rdi)
pxor %xmm14, %xmm12
pxor %xmm12, %xmm4
pxor %xmm4, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_252
._step_252_end:

##### step 253 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 464(%rdi), %xmm13
pxor %xmm13, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_253
._step_253_end:

##### step 254 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

movdqa 480(%rdi), %xmm14
pxor 224(%rdi), %xmm14
movdqa %xmm14, 224(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_254
._step_254_end:

##### step 255 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ])))))

pxor 240(%rdi), %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqd %xmm0, %xmm15
pmovmskb %xmm15, %r12d
test %r12d, %r12d
jne ._report_solution_255
._step_255_end:
#############################
# end of the unrolled chunk #
#############################
jmp ._ending
########### now the code that reports solutions
# here, it has been found that GrayCode(idx+1) is a solution
._report_solution_1:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $1, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_1_end

# here, it has been found that GrayCode(idx+2) is a solution
._report_solution_2:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $2, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_2_end

# here, it has been found that GrayCode(idx+3) is a solution
._report_solution_3:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $3, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_3_end

# here, it has been found that GrayCode(idx+4) is a solution
._report_solution_4:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $4, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_4_end

# here, it has been found that GrayCode(idx+5) is a solution
._report_solution_5:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $5, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_5_end

# here, it has been found that GrayCode(idx+6) is a solution
._report_solution_6:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $6, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_6_end

# here, it has been found that GrayCode(idx+7) is a solution
._report_solution_7:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $7, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_7_end

# here, it has been found that GrayCode(idx+8) is a solution
._report_solution_8:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $8, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_8_end

# here, it has been found that GrayCode(idx+9) is a solution
._report_solution_9:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $9, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_9_end

# here, it has been found that GrayCode(idx+10) is a solution
._report_solution_10:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $10, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_10_end

# here, it has been found that GrayCode(idx+11) is a solution
._report_solution_11:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $11, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_11_end

# here, it has been found that GrayCode(idx+12) is a solution
._report_solution_12:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $12, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_12_end

# here, it has been found that GrayCode(idx+13) is a solution
._report_solution_13:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $13, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_13_end

# here, it has been found that GrayCode(idx+14) is a solution
._report_solution_14:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $14, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_14_end

# here, it has been found that GrayCode(idx+15) is a solution
._report_solution_15:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $15, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_15_end

# here, it has been found that GrayCode(idx+16) is a solution
._report_solution_16:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $16, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_16_end

# here, it has been found that GrayCode(idx+17) is a solution
._report_solution_17:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $17, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_17_end

# here, it has been found that GrayCode(idx+18) is a solution
._report_solution_18:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $18, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_18_end

# here, it has been found that GrayCode(idx+19) is a solution
._report_solution_19:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $19, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_19_end

# here, it has been found that GrayCode(idx+20) is a solution
._report_solution_20:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $20, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_20_end

# here, it has been found that GrayCode(idx+21) is a solution
._report_solution_21:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $21, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_21_end

# here, it has been found that GrayCode(idx+22) is a solution
._report_solution_22:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $22, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_22_end

# here, it has been found that GrayCode(idx+23) is a solution
._report_solution_23:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $23, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_23_end

# here, it has been found that GrayCode(idx+24) is a solution
._report_solution_24:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $24, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_24_end

# here, it has been found that GrayCode(idx+25) is a solution
._report_solution_25:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $25, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_25_end

# here, it has been found that GrayCode(idx+26) is a solution
._report_solution_26:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $26, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_26_end

# here, it has been found that GrayCode(idx+27) is a solution
._report_solution_27:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $27, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_27_end

# here, it has been found that GrayCode(idx+28) is a solution
._report_solution_28:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $28, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_28_end

# here, it has been found that GrayCode(idx+29) is a solution
._report_solution_29:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $29, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_29_end

# here, it has been found that GrayCode(idx+30) is a solution
._report_solution_30:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $30, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_30_end

# here, it has been found that GrayCode(idx+31) is a solution
._report_solution_31:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $31, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_31_end

# here, it has been found that GrayCode(idx+32) is a solution
._report_solution_32:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $32, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_32_end

# here, it has been found that GrayCode(idx+33) is a solution
._report_solution_33:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $33, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_33_end

# here, it has been found that GrayCode(idx+34) is a solution
._report_solution_34:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $34, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_34_end

# here, it has been found that GrayCode(idx+35) is a solution
._report_solution_35:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $35, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_35_end

# here, it has been found that GrayCode(idx+36) is a solution
._report_solution_36:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $36, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_36_end

# here, it has been found that GrayCode(idx+37) is a solution
._report_solution_37:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $37, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_37_end

# here, it has been found that GrayCode(idx+38) is a solution
._report_solution_38:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $38, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_38_end

# here, it has been found that GrayCode(idx+39) is a solution
._report_solution_39:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $39, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_39_end

# here, it has been found that GrayCode(idx+40) is a solution
._report_solution_40:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $40, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_40_end

# here, it has been found that GrayCode(idx+41) is a solution
._report_solution_41:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $41, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_41_end

# here, it has been found that GrayCode(idx+42) is a solution
._report_solution_42:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $42, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_42_end

# here, it has been found that GrayCode(idx+43) is a solution
._report_solution_43:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $43, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_43_end

# here, it has been found that GrayCode(idx+44) is a solution
._report_solution_44:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $44, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_44_end

# here, it has been found that GrayCode(idx+45) is a solution
._report_solution_45:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $45, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_45_end

# here, it has been found that GrayCode(idx+46) is a solution
._report_solution_46:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $46, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_46_end

# here, it has been found that GrayCode(idx+47) is a solution
._report_solution_47:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $47, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_47_end

# here, it has been found that GrayCode(idx+48) is a solution
._report_solution_48:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $48, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_48_end

# here, it has been found that GrayCode(idx+49) is a solution
._report_solution_49:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $49, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_49_end

# here, it has been found that GrayCode(idx+50) is a solution
._report_solution_50:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $50, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_50_end

# here, it has been found that GrayCode(idx+51) is a solution
._report_solution_51:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $51, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_51_end

# here, it has been found that GrayCode(idx+52) is a solution
._report_solution_52:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $52, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_52_end

# here, it has been found that GrayCode(idx+53) is a solution
._report_solution_53:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $53, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_53_end

# here, it has been found that GrayCode(idx+54) is a solution
._report_solution_54:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $54, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_54_end

# here, it has been found that GrayCode(idx+55) is a solution
._report_solution_55:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $55, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_55_end

# here, it has been found that GrayCode(idx+56) is a solution
._report_solution_56:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $56, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_56_end

# here, it has been found that GrayCode(idx+57) is a solution
._report_solution_57:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $57, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_57_end

# here, it has been found that GrayCode(idx+58) is a solution
._report_solution_58:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $58, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_58_end

# here, it has been found that GrayCode(idx+59) is a solution
._report_solution_59:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $59, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_59_end

# here, it has been found that GrayCode(idx+60) is a solution
._report_solution_60:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $60, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_60_end

# here, it has been found that GrayCode(idx+61) is a solution
._report_solution_61:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $61, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_61_end

# here, it has been found that GrayCode(idx+62) is a solution
._report_solution_62:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $62, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_62_end

# here, it has been found that GrayCode(idx+63) is a solution
._report_solution_63:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $63, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_63_end

# here, it has been found that GrayCode(idx+64) is a solution
._report_solution_64:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $64, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_64_end

# here, it has been found that GrayCode(idx+65) is a solution
._report_solution_65:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $65, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_65_end

# here, it has been found that GrayCode(idx+66) is a solution
._report_solution_66:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $66, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_66_end

# here, it has been found that GrayCode(idx+67) is a solution
._report_solution_67:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $67, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_67_end

# here, it has been found that GrayCode(idx+68) is a solution
._report_solution_68:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $68, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_68_end

# here, it has been found that GrayCode(idx+69) is a solution
._report_solution_69:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $69, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_69_end

# here, it has been found that GrayCode(idx+70) is a solution
._report_solution_70:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $70, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_70_end

# here, it has been found that GrayCode(idx+71) is a solution
._report_solution_71:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $71, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_71_end

# here, it has been found that GrayCode(idx+72) is a solution
._report_solution_72:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $72, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_72_end

# here, it has been found that GrayCode(idx+73) is a solution
._report_solution_73:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $73, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_73_end

# here, it has been found that GrayCode(idx+74) is a solution
._report_solution_74:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $74, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_74_end

# here, it has been found that GrayCode(idx+75) is a solution
._report_solution_75:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $75, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_75_end

# here, it has been found that GrayCode(idx+76) is a solution
._report_solution_76:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $76, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_76_end

# here, it has been found that GrayCode(idx+77) is a solution
._report_solution_77:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $77, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_77_end

# here, it has been found that GrayCode(idx+78) is a solution
._report_solution_78:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $78, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_78_end

# here, it has been found that GrayCode(idx+79) is a solution
._report_solution_79:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $79, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_79_end

# here, it has been found that GrayCode(idx+80) is a solution
._report_solution_80:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $80, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_80_end

# here, it has been found that GrayCode(idx+81) is a solution
._report_solution_81:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $81, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_81_end

# here, it has been found that GrayCode(idx+82) is a solution
._report_solution_82:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $82, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_82_end

# here, it has been found that GrayCode(idx+83) is a solution
._report_solution_83:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $83, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_83_end

# here, it has been found that GrayCode(idx+84) is a solution
._report_solution_84:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $84, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_84_end

# here, it has been found that GrayCode(idx+85) is a solution
._report_solution_85:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $85, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_85_end

# here, it has been found that GrayCode(idx+86) is a solution
._report_solution_86:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $86, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_86_end

# here, it has been found that GrayCode(idx+87) is a solution
._report_solution_87:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $87, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_87_end

# here, it has been found that GrayCode(idx+88) is a solution
._report_solution_88:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $88, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_88_end

# here, it has been found that GrayCode(idx+89) is a solution
._report_solution_89:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $89, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_89_end

# here, it has been found that GrayCode(idx+90) is a solution
._report_solution_90:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $90, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_90_end

# here, it has been found that GrayCode(idx+91) is a solution
._report_solution_91:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $91, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_91_end

# here, it has been found that GrayCode(idx+92) is a solution
._report_solution_92:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $92, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_92_end

# here, it has been found that GrayCode(idx+93) is a solution
._report_solution_93:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $93, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_93_end

# here, it has been found that GrayCode(idx+94) is a solution
._report_solution_94:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $94, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_94_end

# here, it has been found that GrayCode(idx+95) is a solution
._report_solution_95:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $95, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_95_end

# here, it has been found that GrayCode(idx+96) is a solution
._report_solution_96:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $96, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_96_end

# here, it has been found that GrayCode(idx+97) is a solution
._report_solution_97:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $97, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_97_end

# here, it has been found that GrayCode(idx+98) is a solution
._report_solution_98:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $98, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_98_end

# here, it has been found that GrayCode(idx+99) is a solution
._report_solution_99:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $99, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_99_end

# here, it has been found that GrayCode(idx+100) is a solution
._report_solution_100:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $100, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_100_end

# here, it has been found that GrayCode(idx+101) is a solution
._report_solution_101:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $101, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_101_end

# here, it has been found that GrayCode(idx+102) is a solution
._report_solution_102:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $102, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_102_end

# here, it has been found that GrayCode(idx+103) is a solution
._report_solution_103:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $103, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_103_end

# here, it has been found that GrayCode(idx+104) is a solution
._report_solution_104:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $104, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_104_end

# here, it has been found that GrayCode(idx+105) is a solution
._report_solution_105:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $105, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_105_end

# here, it has been found that GrayCode(idx+106) is a solution
._report_solution_106:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $106, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_106_end

# here, it has been found that GrayCode(idx+107) is a solution
._report_solution_107:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $107, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_107_end

# here, it has been found that GrayCode(idx+108) is a solution
._report_solution_108:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $108, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_108_end

# here, it has been found that GrayCode(idx+109) is a solution
._report_solution_109:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $109, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_109_end

# here, it has been found that GrayCode(idx+110) is a solution
._report_solution_110:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $110, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_110_end

# here, it has been found that GrayCode(idx+111) is a solution
._report_solution_111:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $111, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_111_end

# here, it has been found that GrayCode(idx+112) is a solution
._report_solution_112:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $112, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_112_end

# here, it has been found that GrayCode(idx+113) is a solution
._report_solution_113:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $113, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_113_end

# here, it has been found that GrayCode(idx+114) is a solution
._report_solution_114:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $114, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_114_end

# here, it has been found that GrayCode(idx+115) is a solution
._report_solution_115:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $115, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_115_end

# here, it has been found that GrayCode(idx+116) is a solution
._report_solution_116:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $116, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_116_end

# here, it has been found that GrayCode(idx+117) is a solution
._report_solution_117:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $117, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_117_end

# here, it has been found that GrayCode(idx+118) is a solution
._report_solution_118:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $118, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_118_end

# here, it has been found that GrayCode(idx+119) is a solution
._report_solution_119:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $119, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_119_end

# here, it has been found that GrayCode(idx+120) is a solution
._report_solution_120:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $120, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_120_end

# here, it has been found that GrayCode(idx+121) is a solution
._report_solution_121:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $121, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_121_end

# here, it has been found that GrayCode(idx+122) is a solution
._report_solution_122:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $122, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_122_end

# here, it has been found that GrayCode(idx+123) is a solution
._report_solution_123:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $123, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_123_end

# here, it has been found that GrayCode(idx+124) is a solution
._report_solution_124:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $124, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_124_end

# here, it has been found that GrayCode(idx+125) is a solution
._report_solution_125:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $125, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_125_end

# here, it has been found that GrayCode(idx+126) is a solution
._report_solution_126:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $126, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_126_end

# here, it has been found that GrayCode(idx+127) is a solution
._report_solution_127:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $127, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_127_end

# here, it has been found that GrayCode(idx+128) is a solution
._report_solution_128:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $128, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_128_end

# here, it has been found that GrayCode(idx+129) is a solution
._report_solution_129:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $129, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_129_end

# here, it has been found that GrayCode(idx+130) is a solution
._report_solution_130:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $130, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_130_end

# here, it has been found that GrayCode(idx+131) is a solution
._report_solution_131:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $131, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_131_end

# here, it has been found that GrayCode(idx+132) is a solution
._report_solution_132:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $132, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_132_end

# here, it has been found that GrayCode(idx+133) is a solution
._report_solution_133:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $133, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_133_end

# here, it has been found that GrayCode(idx+134) is a solution
._report_solution_134:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $134, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_134_end

# here, it has been found that GrayCode(idx+135) is a solution
._report_solution_135:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $135, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_135_end

# here, it has been found that GrayCode(idx+136) is a solution
._report_solution_136:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $136, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_136_end

# here, it has been found that GrayCode(idx+137) is a solution
._report_solution_137:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $137, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_137_end

# here, it has been found that GrayCode(idx+138) is a solution
._report_solution_138:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $138, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_138_end

# here, it has been found that GrayCode(idx+139) is a solution
._report_solution_139:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $139, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_139_end

# here, it has been found that GrayCode(idx+140) is a solution
._report_solution_140:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $140, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_140_end

# here, it has been found that GrayCode(idx+141) is a solution
._report_solution_141:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $141, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_141_end

# here, it has been found that GrayCode(idx+142) is a solution
._report_solution_142:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $142, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_142_end

# here, it has been found that GrayCode(idx+143) is a solution
._report_solution_143:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $143, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_143_end

# here, it has been found that GrayCode(idx+144) is a solution
._report_solution_144:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $144, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_144_end

# here, it has been found that GrayCode(idx+145) is a solution
._report_solution_145:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $145, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_145_end

# here, it has been found that GrayCode(idx+146) is a solution
._report_solution_146:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $146, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_146_end

# here, it has been found that GrayCode(idx+147) is a solution
._report_solution_147:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $147, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_147_end

# here, it has been found that GrayCode(idx+148) is a solution
._report_solution_148:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $148, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_148_end

# here, it has been found that GrayCode(idx+149) is a solution
._report_solution_149:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $149, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_149_end

# here, it has been found that GrayCode(idx+150) is a solution
._report_solution_150:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $150, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_150_end

# here, it has been found that GrayCode(idx+151) is a solution
._report_solution_151:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $151, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_151_end

# here, it has been found that GrayCode(idx+152) is a solution
._report_solution_152:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $152, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_152_end

# here, it has been found that GrayCode(idx+153) is a solution
._report_solution_153:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $153, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_153_end

# here, it has been found that GrayCode(idx+154) is a solution
._report_solution_154:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $154, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_154_end

# here, it has been found that GrayCode(idx+155) is a solution
._report_solution_155:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $155, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_155_end

# here, it has been found that GrayCode(idx+156) is a solution
._report_solution_156:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $156, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_156_end

# here, it has been found that GrayCode(idx+157) is a solution
._report_solution_157:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $157, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_157_end

# here, it has been found that GrayCode(idx+158) is a solution
._report_solution_158:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $158, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_158_end

# here, it has been found that GrayCode(idx+159) is a solution
._report_solution_159:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $159, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_159_end

# here, it has been found that GrayCode(idx+160) is a solution
._report_solution_160:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $160, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_160_end

# here, it has been found that GrayCode(idx+161) is a solution
._report_solution_161:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $161, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_161_end

# here, it has been found that GrayCode(idx+162) is a solution
._report_solution_162:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $162, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_162_end

# here, it has been found that GrayCode(idx+163) is a solution
._report_solution_163:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $163, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_163_end

# here, it has been found that GrayCode(idx+164) is a solution
._report_solution_164:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $164, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_164_end

# here, it has been found that GrayCode(idx+165) is a solution
._report_solution_165:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $165, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_165_end

# here, it has been found that GrayCode(idx+166) is a solution
._report_solution_166:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $166, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_166_end

# here, it has been found that GrayCode(idx+167) is a solution
._report_solution_167:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $167, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_167_end

# here, it has been found that GrayCode(idx+168) is a solution
._report_solution_168:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $168, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_168_end

# here, it has been found that GrayCode(idx+169) is a solution
._report_solution_169:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $169, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_169_end

# here, it has been found that GrayCode(idx+170) is a solution
._report_solution_170:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $170, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_170_end

# here, it has been found that GrayCode(idx+171) is a solution
._report_solution_171:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $171, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_171_end

# here, it has been found that GrayCode(idx+172) is a solution
._report_solution_172:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $172, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_172_end

# here, it has been found that GrayCode(idx+173) is a solution
._report_solution_173:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $173, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_173_end

# here, it has been found that GrayCode(idx+174) is a solution
._report_solution_174:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $174, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_174_end

# here, it has been found that GrayCode(idx+175) is a solution
._report_solution_175:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $175, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_175_end

# here, it has been found that GrayCode(idx+176) is a solution
._report_solution_176:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $176, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_176_end

# here, it has been found that GrayCode(idx+177) is a solution
._report_solution_177:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $177, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_177_end

# here, it has been found that GrayCode(idx+178) is a solution
._report_solution_178:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $178, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_178_end

# here, it has been found that GrayCode(idx+179) is a solution
._report_solution_179:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $179, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_179_end

# here, it has been found that GrayCode(idx+180) is a solution
._report_solution_180:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $180, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_180_end

# here, it has been found that GrayCode(idx+181) is a solution
._report_solution_181:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $181, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_181_end

# here, it has been found that GrayCode(idx+182) is a solution
._report_solution_182:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $182, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_182_end

# here, it has been found that GrayCode(idx+183) is a solution
._report_solution_183:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $183, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_183_end

# here, it has been found that GrayCode(idx+184) is a solution
._report_solution_184:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $184, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_184_end

# here, it has been found that GrayCode(idx+185) is a solution
._report_solution_185:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $185, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_185_end

# here, it has been found that GrayCode(idx+186) is a solution
._report_solution_186:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $186, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_186_end

# here, it has been found that GrayCode(idx+187) is a solution
._report_solution_187:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $187, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_187_end

# here, it has been found that GrayCode(idx+188) is a solution
._report_solution_188:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $188, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_188_end

# here, it has been found that GrayCode(idx+189) is a solution
._report_solution_189:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $189, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_189_end

# here, it has been found that GrayCode(idx+190) is a solution
._report_solution_190:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $190, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_190_end

# here, it has been found that GrayCode(idx+191) is a solution
._report_solution_191:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $191, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_191_end

# here, it has been found that GrayCode(idx+192) is a solution
._report_solution_192:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $192, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_192_end

# here, it has been found that GrayCode(idx+193) is a solution
._report_solution_193:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $193, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_193_end

# here, it has been found that GrayCode(idx+194) is a solution
._report_solution_194:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $194, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_194_end

# here, it has been found that GrayCode(idx+195) is a solution
._report_solution_195:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $195, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_195_end

# here, it has been found that GrayCode(idx+196) is a solution
._report_solution_196:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $196, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_196_end

# here, it has been found that GrayCode(idx+197) is a solution
._report_solution_197:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $197, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_197_end

# here, it has been found that GrayCode(idx+198) is a solution
._report_solution_198:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $198, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_198_end

# here, it has been found that GrayCode(idx+199) is a solution
._report_solution_199:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $199, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_199_end

# here, it has been found that GrayCode(idx+200) is a solution
._report_solution_200:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $200, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_200_end

# here, it has been found that GrayCode(idx+201) is a solution
._report_solution_201:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $201, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_201_end

# here, it has been found that GrayCode(idx+202) is a solution
._report_solution_202:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $202, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_202_end

# here, it has been found that GrayCode(idx+203) is a solution
._report_solution_203:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $203, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_203_end

# here, it has been found that GrayCode(idx+204) is a solution
._report_solution_204:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $204, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_204_end

# here, it has been found that GrayCode(idx+205) is a solution
._report_solution_205:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $205, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_205_end

# here, it has been found that GrayCode(idx+206) is a solution
._report_solution_206:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $206, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_206_end

# here, it has been found that GrayCode(idx+207) is a solution
._report_solution_207:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $207, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_207_end

# here, it has been found that GrayCode(idx+208) is a solution
._report_solution_208:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $208, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_208_end

# here, it has been found that GrayCode(idx+209) is a solution
._report_solution_209:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $209, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_209_end

# here, it has been found that GrayCode(idx+210) is a solution
._report_solution_210:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $210, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_210_end

# here, it has been found that GrayCode(idx+211) is a solution
._report_solution_211:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $211, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_211_end

# here, it has been found that GrayCode(idx+212) is a solution
._report_solution_212:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $212, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_212_end

# here, it has been found that GrayCode(idx+213) is a solution
._report_solution_213:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $213, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_213_end

# here, it has been found that GrayCode(idx+214) is a solution
._report_solution_214:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $214, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_214_end

# here, it has been found that GrayCode(idx+215) is a solution
._report_solution_215:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $215, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_215_end

# here, it has been found that GrayCode(idx+216) is a solution
._report_solution_216:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $216, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_216_end

# here, it has been found that GrayCode(idx+217) is a solution
._report_solution_217:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $217, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_217_end

# here, it has been found that GrayCode(idx+218) is a solution
._report_solution_218:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $218, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_218_end

# here, it has been found that GrayCode(idx+219) is a solution
._report_solution_219:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $219, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_219_end

# here, it has been found that GrayCode(idx+220) is a solution
._report_solution_220:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $220, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_220_end

# here, it has been found that GrayCode(idx+221) is a solution
._report_solution_221:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $221, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_221_end

# here, it has been found that GrayCode(idx+222) is a solution
._report_solution_222:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $222, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_222_end

# here, it has been found that GrayCode(idx+223) is a solution
._report_solution_223:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $223, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_223_end

# here, it has been found that GrayCode(idx+224) is a solution
._report_solution_224:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $224, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_224_end

# here, it has been found that GrayCode(idx+225) is a solution
._report_solution_225:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $225, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_225_end

# here, it has been found that GrayCode(idx+226) is a solution
._report_solution_226:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $226, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_226_end

# here, it has been found that GrayCode(idx+227) is a solution
._report_solution_227:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $227, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_227_end

# here, it has been found that GrayCode(idx+228) is a solution
._report_solution_228:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $228, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_228_end

# here, it has been found that GrayCode(idx+229) is a solution
._report_solution_229:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $229, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_229_end

# here, it has been found that GrayCode(idx+230) is a solution
._report_solution_230:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $230, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_230_end

# here, it has been found that GrayCode(idx+231) is a solution
._report_solution_231:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $231, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_231_end

# here, it has been found that GrayCode(idx+232) is a solution
._report_solution_232:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $232, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_232_end

# here, it has been found that GrayCode(idx+233) is a solution
._report_solution_233:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $233, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_233_end

# here, it has been found that GrayCode(idx+234) is a solution
._report_solution_234:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $234, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_234_end

# here, it has been found that GrayCode(idx+235) is a solution
._report_solution_235:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $235, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_235_end

# here, it has been found that GrayCode(idx+236) is a solution
._report_solution_236:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $236, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_236_end

# here, it has been found that GrayCode(idx+237) is a solution
._report_solution_237:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $237, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_237_end

# here, it has been found that GrayCode(idx+238) is a solution
._report_solution_238:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $238, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_238_end

# here, it has been found that GrayCode(idx+239) is a solution
._report_solution_239:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $239, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_239_end

# here, it has been found that GrayCode(idx+240) is a solution
._report_solution_240:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $240, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_240_end

# here, it has been found that GrayCode(idx+241) is a solution
._report_solution_241:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $241, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_241_end

# here, it has been found that GrayCode(idx+242) is a solution
._report_solution_242:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $242, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_242_end

# here, it has been found that GrayCode(idx+243) is a solution
._report_solution_243:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $243, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_243_end

# here, it has been found that GrayCode(idx+244) is a solution
._report_solution_244:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $244, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_244_end

# here, it has been found that GrayCode(idx+245) is a solution
._report_solution_245:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $245, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_245_end

# here, it has been found that GrayCode(idx+246) is a solution
._report_solution_246:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $246, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_246_end

# here, it has been found that GrayCode(idx+247) is a solution
._report_solution_247:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $247, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_247_end

# here, it has been found that GrayCode(idx+248) is a solution
._report_solution_248:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $248, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_248_end

# here, it has been found that GrayCode(idx+249) is a solution
._report_solution_249:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $249, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_249_end

# here, it has been found that GrayCode(idx+250) is a solution
._report_solution_250:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $250, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_250_end

# here, it has been found that GrayCode(idx+251) is a solution
._report_solution_251:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $251, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_251_end

# here, it has been found that GrayCode(idx+252) is a solution
._report_solution_252:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $252, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_252_end

# here, it has been found that GrayCode(idx+253) is a solution
._report_solution_253:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $253, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_253_end

# here, it has been found that GrayCode(idx+254) is a solution
._report_solution_254:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $254, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_254_end

# here, it has been found that GrayCode(idx+255) is a solution
._report_solution_255:
pxor %xmm15, %xmm15
shl $4, %r9
mov %r8, %rsi
add $255, %rsi
movq %rsi, (%rdx, %r9)
add $8, %r9
movl %r12d, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_255_end

._ending:


# copy back to memory the (most-frequently used) derivatives that were held in registers
movdqa %xmm0, 0(%rdi)
movdqa %xmm1, 16(%rdi)
movdqa %xmm2, 32(%rdi)
movdqa %xmm3, 48(%rdi)
movdqa %xmm4, 64(%rdi)
movdqa %xmm5, 80(%rdi)
movdqa %xmm6, 96(%rdi)
movdqa %xmm7, 112(%rdi)
movdqa %xmm8, 128(%rdi)
movdqa %xmm9, 144(%rdi)
movdqa %xmm10, 160(%rdi)
movdqa %xmm11, 176(%rdi)
movdqa %xmm12, 192(%rdi)
movdqa %xmm13, 208(%rdi)

# store the number of solutions found in this chunk
movq %r9, 0(%rcx)

# restore the callee-save registers
movq 0(%rsp),%r11
movq 8(%rsp),%r12
movq 16(%rsp),%r13
movq 24(%rsp),%r14
movq 32(%rsp),%r15
movq 40(%rsp),%rbx
movq 48(%rsp),%rbp

# restore the stack frame
add %r11,%rsp

# prepare the return value (?!?)
mov %rdi,%rax
mov %rsi,%rdx
ret
