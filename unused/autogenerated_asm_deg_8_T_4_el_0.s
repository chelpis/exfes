.text
.p2align 5

.globl _func_deg_8_T_4_el_0
.globl func_deg_8_T_4_el_0
### void func_deg_8_T_4_el_0(__m128i *F, __m128i **F_sp, void *buf, uint64_t *num, uint64_t idx);

# the ABI (http://www.x86-64.org/documentation/abi.pdf) says that...
# A) we should preserve the values of %rbx, %rbp, %r12...%r15 [callee-save registers]
# B) We will receive the arguments of the function in registers :
#       the pointer to F should be in %rdi
#       the pointer to *F_sp should be in %rsi
#       the pointer to buf should be in %rdx
#       the pointer to num should be in %rcx
#       idx should be in %r8

_func_deg_8_T_4_el_0:
func_deg_8_T_4_el_0:

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
# variable delta maps to %r12
# variable epsilon maps to %r13
# variable zeta maps to %r14
# variable eta maps to %r15
# variable sum maps to %xmm14
# variable zero maps to %xmm15
# variable mask maps to %ebx

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
movq 24(%rsi), %r12   ## %r12 = delta
movq 32(%rsi), %r13   ## %r13 = epsilon
movq 40(%rsi), %r14   ## %r14 = zeta
movq 48(%rsi), %r15   ## %r15 = eta

# note that at this point, the register holding `F_sp` [%rsi] could be used for something else

# initialize the last things that remains to be intialized...
movq (%rcx), %r9  ## num = *num_ptr
pxor %xmm15, %xmm15   ## zero = 0


##### step 1 [hw=1]
##### F[ 1 ] ^= ( F[ 1 ] ^= ( F[ alpha + 1 ] ^= ( F[ beta + 1 ] ^= ( F[ gamma + 1 ] ^= ( F[ delta + 1 ] ^= ( F[ epsilon + 1 ] ^= ( F[ zeta + 1 ] ^= ( F[ eta + 1 ])))))))))

movdqa 16(%rdi,%r15), %xmm14
pxor 16(%rdi,%r14), %xmm14
movdqa %xmm14, 16(%rdi,%r14)
pxor 16(%rdi,%r13), %xmm14
movdqa %xmm14, 16(%rdi,%r13)
pxor 16(%rdi,%r12), %xmm14
movdqa %xmm14, 16(%rdi,%r12)
pxor 16(%rdi,%r11), %xmm14
movdqa %xmm14, 16(%rdi,%r11)
pxor 16(%rdi,%r10), %xmm14
movdqa %xmm14, 16(%rdi,%r10)
pxor 16(%rdi,%rax), %xmm14
movdqa %xmm14, 16(%rdi,%rax)
pxor %xmm14, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_1
._step_1_end:

##### step 2 [hw=1]
##### F[ 1 ] ^= ( F[ 1 ] ^= ( F[ alpha + 1 ] ^= ( F[ beta + 1 ] ^= ( F[ gamma + 1 ] ^= ( F[ delta + 1 ] ^= ( F[ epsilon + 1 ] ^= ( F[ zeta + 1 ] ^= ( F[ eta + 1 ])))))))))

movdqa 32(%rdi,%r15), %xmm14
pxor 32(%rdi,%r14), %xmm14
movdqa %xmm14, 32(%rdi,%r14)
pxor 32(%rdi,%r13), %xmm14
movdqa %xmm14, 32(%rdi,%r13)
pxor 32(%rdi,%r12), %xmm14
movdqa %xmm14, 32(%rdi,%r12)
pxor 32(%rdi,%r11), %xmm14
movdqa %xmm14, 32(%rdi,%r11)
pxor 32(%rdi,%r10), %xmm14
movdqa %xmm14, 32(%rdi,%r10)
pxor 32(%rdi,%rax), %xmm14
movdqa %xmm14, 32(%rdi,%rax)
pxor %xmm14, %xmm2
pxor %xmm2, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_2
._step_2_end:

##### step 3 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ] ^= ( F[ gamma + 2 ] ^= ( F[ delta + 2 ] ^= ( F[ epsilon + 2 ] ^= ( F[ zeta + 2 ])))))))))

movdqa 48(%rdi,%r14), %xmm14
pxor 48(%rdi,%r13), %xmm14
movdqa %xmm14, 48(%rdi,%r13)
pxor 48(%rdi,%r12), %xmm14
movdqa %xmm14, 48(%rdi,%r12)
pxor 48(%rdi,%r11), %xmm14
movdqa %xmm14, 48(%rdi,%r11)
pxor 48(%rdi,%r10), %xmm14
movdqa %xmm14, 48(%rdi,%r10)
pxor 48(%rdi,%rax), %xmm14
movdqa %xmm14, 48(%rdi,%rax)
pxor %xmm14, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_3
._step_3_end:

##### step 4 [hw=1]
##### F[ 1 ] ^= ( F[ 1 ] ^= ( F[ alpha + 1 ] ^= ( F[ beta + 1 ] ^= ( F[ gamma + 1 ] ^= ( F[ delta + 1 ] ^= ( F[ epsilon + 1 ] ^= ( F[ zeta + 1 ] ^= ( F[ eta + 1 ])))))))))

movdqa 48(%rdi,%r15), %xmm14
pxor 64(%rdi,%r14), %xmm14
movdqa %xmm14, 64(%rdi,%r14)
pxor 64(%rdi,%r13), %xmm14
movdqa %xmm14, 64(%rdi,%r13)
pxor 64(%rdi,%r12), %xmm14
movdqa %xmm14, 64(%rdi,%r12)
pxor 64(%rdi,%r11), %xmm14
movdqa %xmm14, 64(%rdi,%r11)
pxor 64(%rdi,%r10), %xmm14
movdqa %xmm14, 64(%rdi,%r10)
pxor 64(%rdi,%rax), %xmm14
movdqa %xmm14, 64(%rdi,%rax)
pxor %xmm14, %xmm4
pxor %xmm4, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_4
._step_4_end:

##### step 5 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ] ^= ( F[ gamma + 2 ] ^= ( F[ delta + 2 ] ^= ( F[ epsilon + 2 ] ^= ( F[ zeta + 2 ])))))))))

movdqa 80(%rdi,%r14), %xmm14
pxor 80(%rdi,%r13), %xmm14
movdqa %xmm14, 80(%rdi,%r13)
pxor 80(%rdi,%r12), %xmm14
movdqa %xmm14, 80(%rdi,%r12)
pxor 80(%rdi,%r11), %xmm14
movdqa %xmm14, 80(%rdi,%r11)
pxor 80(%rdi,%r10), %xmm14
movdqa %xmm14, 80(%rdi,%r10)
pxor 80(%rdi,%rax), %xmm14
movdqa %xmm14, 80(%rdi,%rax)
pxor %xmm14, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_5
._step_5_end:

##### step 6 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ] ^= ( F[ gamma + 2 ] ^= ( F[ delta + 2 ] ^= ( F[ epsilon + 2 ] ^= ( F[ zeta + 2 ])))))))))

movdqa 96(%rdi,%r14), %xmm14
pxor 96(%rdi,%r13), %xmm14
movdqa %xmm14, 96(%rdi,%r13)
pxor 96(%rdi,%r12), %xmm14
movdqa %xmm14, 96(%rdi,%r12)
pxor 96(%rdi,%r11), %xmm14
movdqa %xmm14, 96(%rdi,%r11)
pxor 96(%rdi,%r10), %xmm14
movdqa %xmm14, 96(%rdi,%r10)
pxor 96(%rdi,%rax), %xmm14
movdqa %xmm14, 96(%rdi,%rax)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_6
._step_6_end:

##### step 7 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ] ^= ( F[ beta + 3 ] ^= ( F[ gamma + 3 ] ^= ( F[ delta + 3 ] ^= ( F[ epsilon + 3 ])))))))))

movdqa 112(%rdi,%r13), %xmm14
pxor 112(%rdi,%r12), %xmm14
movdqa %xmm14, 112(%rdi,%r12)
pxor 112(%rdi,%r11), %xmm14
movdqa %xmm14, 112(%rdi,%r11)
pxor 112(%rdi,%r10), %xmm14
movdqa %xmm14, 112(%rdi,%r10)
pxor 112(%rdi,%rax), %xmm14
movdqa %xmm14, 112(%rdi,%rax)
pxor %xmm14, %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_7
._step_7_end:

##### step 8 [hw=1]
##### F[ 1 ] ^= ( F[ 1 ] ^= ( F[ alpha + 1 ] ^= ( F[ beta + 1 ] ^= ( F[ gamma + 1 ] ^= ( F[ delta + 1 ] ^= ( F[ epsilon + 1 ] ^= ( F[ zeta + 1 ] ^= ( F[ eta + 1 ])))))))))

movdqa 64(%rdi,%r15), %xmm14
pxor 112(%rdi,%r14), %xmm14
movdqa %xmm14, 112(%rdi,%r14)
pxor 128(%rdi,%r13), %xmm14
movdqa %xmm14, 128(%rdi,%r13)
pxor 128(%rdi,%r12), %xmm14
movdqa %xmm14, 128(%rdi,%r12)
pxor 128(%rdi,%r11), %xmm14
movdqa %xmm14, 128(%rdi,%r11)
pxor 128(%rdi,%r10), %xmm14
movdqa %xmm14, 128(%rdi,%r10)
pxor 128(%rdi,%rax), %xmm14
movdqa %xmm14, 128(%rdi,%rax)
pxor %xmm14, %xmm8
pxor %xmm8, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_8
._step_8_end:

##### step 9 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ] ^= ( F[ gamma + 2 ] ^= ( F[ delta + 2 ] ^= ( F[ epsilon + 2 ] ^= ( F[ zeta + 2 ])))))))))

movdqa 128(%rdi,%r14), %xmm14
pxor 144(%rdi,%r13), %xmm14
movdqa %xmm14, 144(%rdi,%r13)
pxor 144(%rdi,%r12), %xmm14
movdqa %xmm14, 144(%rdi,%r12)
pxor 144(%rdi,%r11), %xmm14
movdqa %xmm14, 144(%rdi,%r11)
pxor 144(%rdi,%r10), %xmm14
movdqa %xmm14, 144(%rdi,%r10)
pxor 144(%rdi,%rax), %xmm14
movdqa %xmm14, 144(%rdi,%rax)
pxor %xmm14, %xmm9
pxor %xmm9, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_9
._step_9_end:

##### step 10 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ] ^= ( F[ gamma + 2 ] ^= ( F[ delta + 2 ] ^= ( F[ epsilon + 2 ] ^= ( F[ zeta + 2 ])))))))))

movdqa 144(%rdi,%r14), %xmm14
pxor 160(%rdi,%r13), %xmm14
movdqa %xmm14, 160(%rdi,%r13)
pxor 160(%rdi,%r12), %xmm14
movdqa %xmm14, 160(%rdi,%r12)
pxor 160(%rdi,%r11), %xmm14
movdqa %xmm14, 160(%rdi,%r11)
pxor 160(%rdi,%r10), %xmm14
movdqa %xmm14, 160(%rdi,%r10)
pxor 160(%rdi,%rax), %xmm14
movdqa %xmm14, 160(%rdi,%rax)
pxor %xmm14, %xmm10
pxor %xmm10, %xmm2
pxor %xmm2, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_10
._step_10_end:

##### step 11 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ] ^= ( F[ beta + 3 ] ^= ( F[ gamma + 3 ] ^= ( F[ delta + 3 ] ^= ( F[ epsilon + 3 ])))))))))

movdqa 176(%rdi,%r13), %xmm14
pxor 176(%rdi,%r12), %xmm14
movdqa %xmm14, 176(%rdi,%r12)
pxor 176(%rdi,%r11), %xmm14
movdqa %xmm14, 176(%rdi,%r11)
pxor 176(%rdi,%r10), %xmm14
movdqa %xmm14, 176(%rdi,%r10)
pxor 176(%rdi,%rax), %xmm14
movdqa %xmm14, 176(%rdi,%rax)
pxor %xmm14, %xmm11
pxor %xmm11, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_11
._step_11_end:

##### step 12 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ] ^= ( F[ gamma + 2 ] ^= ( F[ delta + 2 ] ^= ( F[ epsilon + 2 ] ^= ( F[ zeta + 2 ])))))))))

movdqa 160(%rdi,%r14), %xmm14
pxor 192(%rdi,%r13), %xmm14
movdqa %xmm14, 192(%rdi,%r13)
pxor 192(%rdi,%r12), %xmm14
movdqa %xmm14, 192(%rdi,%r12)
pxor 192(%rdi,%r11), %xmm14
movdqa %xmm14, 192(%rdi,%r11)
pxor 192(%rdi,%r10), %xmm14
movdqa %xmm14, 192(%rdi,%r10)
pxor 192(%rdi,%rax), %xmm14
movdqa %xmm14, 192(%rdi,%rax)
pxor %xmm14, %xmm12
pxor %xmm12, %xmm4
pxor %xmm4, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_12
._step_12_end:

##### step 13 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ] ^= ( F[ beta + 3 ] ^= ( F[ gamma + 3 ] ^= ( F[ delta + 3 ] ^= ( F[ epsilon + 3 ])))))))))

movdqa 208(%rdi,%r13), %xmm14
pxor 208(%rdi,%r12), %xmm14
movdqa %xmm14, 208(%rdi,%r12)
pxor 208(%rdi,%r11), %xmm14
movdqa %xmm14, 208(%rdi,%r11)
pxor 208(%rdi,%r10), %xmm14
movdqa %xmm14, 208(%rdi,%r10)
pxor 208(%rdi,%rax), %xmm14
movdqa %xmm14, 208(%rdi,%rax)
pxor %xmm14, %xmm13
pxor %xmm13, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_13
._step_13_end:

##### step 14 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ] ^= ( F[ beta + 3 ] ^= ( F[ gamma + 3 ] ^= ( F[ delta + 3 ] ^= ( F[ epsilon + 3 ])))))))))

movdqa 224(%rdi,%r13), %xmm14
pxor 224(%rdi,%r12), %xmm14
movdqa %xmm14, 224(%rdi,%r12)
pxor 224(%rdi,%r11), %xmm14
movdqa %xmm14, 224(%rdi,%r11)
pxor 224(%rdi,%r10), %xmm14
movdqa %xmm14, 224(%rdi,%r10)
pxor 224(%rdi,%rax), %xmm14
movdqa %xmm14, 224(%rdi,%rax)
pxor 224(%rdi), %xmm14
movdqa %xmm14, 224(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_14
._step_14_end:

##### step 15 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ alpha + 4 ] ^= ( F[ beta + 4 ] ^= ( F[ gamma + 4 ] ^= ( F[ delta + 4 ])))))))))

movdqa 240(%rdi,%r12), %xmm14
pxor 240(%rdi,%r11), %xmm14
movdqa %xmm14, 240(%rdi,%r11)
pxor 240(%rdi,%r10), %xmm14
movdqa %xmm14, 240(%rdi,%r10)
pxor 240(%rdi,%rax), %xmm14
movdqa %xmm14, 240(%rdi,%rax)
pxor 240(%rdi), %xmm14
movdqa %xmm14, 240(%rdi)
pxor %xmm14, %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_15
._step_15_end:

##### step 16 [hw=1]
##### F[ 1 ] ^= ( F[ 1 ] ^= ( F[ alpha + 1 ] ^= ( F[ beta + 1 ] ^= ( F[ gamma + 1 ] ^= ( F[ delta + 1 ] ^= ( F[ epsilon + 1 ] ^= ( F[ zeta + 1 ] ^= ( F[ eta + 1 ])))))))))

movdqa 80(%rdi,%r15), %xmm14
pxor 176(%rdi,%r14), %xmm14
movdqa %xmm14, 176(%rdi,%r14)
pxor 240(%rdi,%r13), %xmm14
movdqa %xmm14, 240(%rdi,%r13)
pxor 256(%rdi,%r12), %xmm14
movdqa %xmm14, 256(%rdi,%r12)
pxor 256(%rdi,%r11), %xmm14
movdqa %xmm14, 256(%rdi,%r11)
pxor 256(%rdi,%r10), %xmm14
movdqa %xmm14, 256(%rdi,%r10)
pxor 256(%rdi,%rax), %xmm14
movdqa %xmm14, 256(%rdi,%rax)
pxor 256(%rdi), %xmm14
movdqa %xmm14, 256(%rdi)
pxor %xmm14, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_16
._step_16_end:

##### step 17 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ] ^= ( F[ gamma + 2 ] ^= ( F[ delta + 2 ] ^= ( F[ epsilon + 2 ] ^= ( F[ zeta + 2 ])))))))))

movdqa 192(%rdi,%r14), %xmm14
pxor 256(%rdi,%r13), %xmm14
movdqa %xmm14, 256(%rdi,%r13)
pxor 272(%rdi,%r12), %xmm14
movdqa %xmm14, 272(%rdi,%r12)
pxor 272(%rdi,%r11), %xmm14
movdqa %xmm14, 272(%rdi,%r11)
pxor 272(%rdi,%r10), %xmm14
movdqa %xmm14, 272(%rdi,%r10)
pxor 272(%rdi,%rax), %xmm14
movdqa %xmm14, 272(%rdi,%rax)
pxor 272(%rdi), %xmm14
movdqa %xmm14, 272(%rdi)
pxor %xmm14, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_17
._step_17_end:

##### step 18 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ] ^= ( F[ gamma + 2 ] ^= ( F[ delta + 2 ] ^= ( F[ epsilon + 2 ] ^= ( F[ zeta + 2 ])))))))))

movdqa 208(%rdi,%r14), %xmm14
pxor 272(%rdi,%r13), %xmm14
movdqa %xmm14, 272(%rdi,%r13)
pxor 288(%rdi,%r12), %xmm14
movdqa %xmm14, 288(%rdi,%r12)
pxor 288(%rdi,%r11), %xmm14
movdqa %xmm14, 288(%rdi,%r11)
pxor 288(%rdi,%r10), %xmm14
movdqa %xmm14, 288(%rdi,%r10)
pxor 288(%rdi,%rax), %xmm14
movdqa %xmm14, 288(%rdi,%rax)
pxor 288(%rdi), %xmm14
movdqa %xmm14, 288(%rdi)
pxor %xmm14, %xmm2
pxor %xmm2, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_18
._step_18_end:

##### step 19 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ] ^= ( F[ beta + 3 ] ^= ( F[ gamma + 3 ] ^= ( F[ delta + 3 ] ^= ( F[ epsilon + 3 ])))))))))

movdqa 288(%rdi,%r13), %xmm14
pxor 304(%rdi,%r12), %xmm14
movdqa %xmm14, 304(%rdi,%r12)
pxor 304(%rdi,%r11), %xmm14
movdqa %xmm14, 304(%rdi,%r11)
pxor 304(%rdi,%r10), %xmm14
movdqa %xmm14, 304(%rdi,%r10)
pxor 304(%rdi,%rax), %xmm14
movdqa %xmm14, 304(%rdi,%rax)
pxor 304(%rdi), %xmm14
movdqa %xmm14, 304(%rdi)
pxor %xmm14, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_19
._step_19_end:

##### step 20 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ] ^= ( F[ gamma + 2 ] ^= ( F[ delta + 2 ] ^= ( F[ epsilon + 2 ] ^= ( F[ zeta + 2 ])))))))))

movdqa 224(%rdi,%r14), %xmm14
pxor 304(%rdi,%r13), %xmm14
movdqa %xmm14, 304(%rdi,%r13)
pxor 320(%rdi,%r12), %xmm14
movdqa %xmm14, 320(%rdi,%r12)
pxor 320(%rdi,%r11), %xmm14
movdqa %xmm14, 320(%rdi,%r11)
pxor 320(%rdi,%r10), %xmm14
movdqa %xmm14, 320(%rdi,%r10)
pxor 320(%rdi,%rax), %xmm14
movdqa %xmm14, 320(%rdi,%rax)
pxor 320(%rdi), %xmm14
movdqa %xmm14, 320(%rdi)
pxor %xmm14, %xmm4
pxor %xmm4, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_20
._step_20_end:

##### step 21 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ] ^= ( F[ beta + 3 ] ^= ( F[ gamma + 3 ] ^= ( F[ delta + 3 ] ^= ( F[ epsilon + 3 ])))))))))

movdqa 320(%rdi,%r13), %xmm14
pxor 336(%rdi,%r12), %xmm14
movdqa %xmm14, 336(%rdi,%r12)
pxor 336(%rdi,%r11), %xmm14
movdqa %xmm14, 336(%rdi,%r11)
pxor 336(%rdi,%r10), %xmm14
movdqa %xmm14, 336(%rdi,%r10)
pxor 336(%rdi,%rax), %xmm14
movdqa %xmm14, 336(%rdi,%rax)
pxor 336(%rdi), %xmm14
movdqa %xmm14, 336(%rdi)
pxor %xmm14, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_21
._step_21_end:

##### step 22 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ] ^= ( F[ beta + 3 ] ^= ( F[ gamma + 3 ] ^= ( F[ delta + 3 ] ^= ( F[ epsilon + 3 ])))))))))

movdqa 336(%rdi,%r13), %xmm14
pxor 352(%rdi,%r12), %xmm14
movdqa %xmm14, 352(%rdi,%r12)
pxor 352(%rdi,%r11), %xmm14
movdqa %xmm14, 352(%rdi,%r11)
pxor 352(%rdi,%r10), %xmm14
movdqa %xmm14, 352(%rdi,%r10)
pxor 352(%rdi,%rax), %xmm14
movdqa %xmm14, 352(%rdi,%rax)
pxor 352(%rdi), %xmm14
movdqa %xmm14, 352(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_22
._step_22_end:

##### step 23 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ alpha + 4 ] ^= ( F[ beta + 4 ] ^= ( F[ gamma + 4 ] ^= ( F[ delta + 4 ])))))))))

movdqa 368(%rdi,%r12), %xmm14
pxor 368(%rdi,%r11), %xmm14
movdqa %xmm14, 368(%rdi,%r11)
pxor 368(%rdi,%r10), %xmm14
movdqa %xmm14, 368(%rdi,%r10)
pxor 368(%rdi,%rax), %xmm14
movdqa %xmm14, 368(%rdi,%rax)
pxor 368(%rdi), %xmm14
movdqa %xmm14, 368(%rdi)
pxor %xmm14, %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_23
._step_23_end:

##### step 24 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ] ^= ( F[ gamma + 2 ] ^= ( F[ delta + 2 ] ^= ( F[ epsilon + 2 ] ^= ( F[ zeta + 2 ])))))))))

movdqa 240(%rdi,%r14), %xmm14
pxor 352(%rdi,%r13), %xmm14
movdqa %xmm14, 352(%rdi,%r13)
pxor 384(%rdi,%r12), %xmm14
movdqa %xmm14, 384(%rdi,%r12)
pxor 384(%rdi,%r11), %xmm14
movdqa %xmm14, 384(%rdi,%r11)
pxor 384(%rdi,%r10), %xmm14
movdqa %xmm14, 384(%rdi,%r10)
pxor 384(%rdi,%rax), %xmm14
movdqa %xmm14, 384(%rdi,%rax)
pxor 384(%rdi), %xmm14
movdqa %xmm14, 384(%rdi)
pxor %xmm14, %xmm8
pxor %xmm8, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_24
._step_24_end:

##### step 25 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ] ^= ( F[ beta + 3 ] ^= ( F[ gamma + 3 ] ^= ( F[ delta + 3 ] ^= ( F[ epsilon + 3 ])))))))))

movdqa 368(%rdi,%r13), %xmm14
pxor 400(%rdi,%r12), %xmm14
movdqa %xmm14, 400(%rdi,%r12)
pxor 400(%rdi,%r11), %xmm14
movdqa %xmm14, 400(%rdi,%r11)
pxor 400(%rdi,%r10), %xmm14
movdqa %xmm14, 400(%rdi,%r10)
pxor 400(%rdi,%rax), %xmm14
movdqa %xmm14, 400(%rdi,%rax)
pxor 400(%rdi), %xmm14
movdqa %xmm14, 400(%rdi)
pxor %xmm14, %xmm9
pxor %xmm9, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_25
._step_25_end:

##### step 26 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ] ^= ( F[ beta + 3 ] ^= ( F[ gamma + 3 ] ^= ( F[ delta + 3 ] ^= ( F[ epsilon + 3 ])))))))))

movdqa 384(%rdi,%r13), %xmm14
pxor 416(%rdi,%r12), %xmm14
movdqa %xmm14, 416(%rdi,%r12)
pxor 416(%rdi,%r11), %xmm14
movdqa %xmm14, 416(%rdi,%r11)
pxor 416(%rdi,%r10), %xmm14
movdqa %xmm14, 416(%rdi,%r10)
pxor 416(%rdi,%rax), %xmm14
movdqa %xmm14, 416(%rdi,%rax)
pxor 416(%rdi), %xmm14
movdqa %xmm14, 416(%rdi)
pxor %xmm14, %xmm10
pxor %xmm10, %xmm2
pxor %xmm2, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_26
._step_26_end:

##### step 27 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ alpha + 4 ] ^= ( F[ beta + 4 ] ^= ( F[ gamma + 4 ] ^= ( F[ delta + 4 ])))))))))

movdqa 432(%rdi,%r12), %xmm14
pxor 432(%rdi,%r11), %xmm14
movdqa %xmm14, 432(%rdi,%r11)
pxor 432(%rdi,%r10), %xmm14
movdqa %xmm14, 432(%rdi,%r10)
pxor 432(%rdi,%rax), %xmm14
movdqa %xmm14, 432(%rdi,%rax)
pxor 432(%rdi), %xmm14
movdqa %xmm14, 432(%rdi)
pxor %xmm14, %xmm11
pxor %xmm11, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_27
._step_27_end:

##### step 28 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ] ^= ( F[ beta + 3 ] ^= ( F[ gamma + 3 ] ^= ( F[ delta + 3 ] ^= ( F[ epsilon + 3 ])))))))))

movdqa 400(%rdi,%r13), %xmm14
pxor 448(%rdi,%r12), %xmm14
movdqa %xmm14, 448(%rdi,%r12)
pxor 448(%rdi,%r11), %xmm14
movdqa %xmm14, 448(%rdi,%r11)
pxor 448(%rdi,%r10), %xmm14
movdqa %xmm14, 448(%rdi,%r10)
pxor 448(%rdi,%rax), %xmm14
movdqa %xmm14, 448(%rdi,%rax)
pxor 448(%rdi), %xmm14
movdqa %xmm14, 448(%rdi)
pxor %xmm14, %xmm12
pxor %xmm12, %xmm4
pxor %xmm4, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_28
._step_28_end:

##### step 29 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ alpha + 4 ] ^= ( F[ beta + 4 ] ^= ( F[ gamma + 4 ] ^= ( F[ delta + 4 ])))))))))

movdqa 464(%rdi,%r12), %xmm14
pxor 464(%rdi,%r11), %xmm14
movdqa %xmm14, 464(%rdi,%r11)
pxor 464(%rdi,%r10), %xmm14
movdqa %xmm14, 464(%rdi,%r10)
pxor 464(%rdi,%rax), %xmm14
movdqa %xmm14, 464(%rdi,%rax)
pxor 464(%rdi), %xmm14
movdqa %xmm14, 464(%rdi)
pxor %xmm14, %xmm13
pxor %xmm13, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_29
._step_29_end:

##### step 30 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ alpha + 4 ] ^= ( F[ beta + 4 ] ^= ( F[ gamma + 4 ] ^= ( F[ delta + 4 ])))))))))

movdqa 480(%rdi,%r12), %xmm14
pxor 480(%rdi,%r11), %xmm14
movdqa %xmm14, 480(%rdi,%r11)
pxor 480(%rdi,%r10), %xmm14
movdqa %xmm14, 480(%rdi,%r10)
pxor 480(%rdi,%rax), %xmm14
movdqa %xmm14, 480(%rdi,%rax)
pxor 480(%rdi), %xmm14
movdqa %xmm14, 480(%rdi)
pxor 224(%rdi), %xmm14
movdqa %xmm14, 224(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_30
._step_30_end:

##### step 31 [hw=5]
##### F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ alpha + 5 ] ^= ( F[ beta + 5 ] ^= ( F[ gamma + 5 ])))))))))

movdqa 496(%rdi,%r11), %xmm14
pxor 496(%rdi,%r10), %xmm14
movdqa %xmm14, 496(%rdi,%r10)
pxor 496(%rdi,%rax), %xmm14
movdqa %xmm14, 496(%rdi,%rax)
pxor 496(%rdi), %xmm14
movdqa %xmm14, 496(%rdi)
pxor 240(%rdi), %xmm14
movdqa %xmm14, 240(%rdi)
pxor %xmm14, %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_31
._step_31_end:

##### step 32 [hw=1]
##### F[ 1 ] ^= ( F[ 1 ] ^= ( F[ alpha + 1 ] ^= ( F[ beta + 1 ] ^= ( F[ gamma + 1 ] ^= ( F[ delta + 1 ] ^= ( F[ epsilon + 1 ] ^= ( F[ zeta + 1 ] ^= ( F[ eta + 1 ])))))))))

movdqa 96(%rdi,%r15), %xmm14
pxor 256(%rdi,%r14), %xmm14
movdqa %xmm14, 256(%rdi,%r14)
pxor 416(%rdi,%r13), %xmm14
movdqa %xmm14, 416(%rdi,%r13)
pxor 496(%rdi,%r12), %xmm14
movdqa %xmm14, 496(%rdi,%r12)
pxor 512(%rdi,%r11), %xmm14
movdqa %xmm14, 512(%rdi,%r11)
pxor 512(%rdi,%r10), %xmm14
movdqa %xmm14, 512(%rdi,%r10)
pxor 512(%rdi,%rax), %xmm14
movdqa %xmm14, 512(%rdi,%rax)
pxor 512(%rdi), %xmm14
movdqa %xmm14, 512(%rdi)
pxor %xmm14, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_32
._step_32_end:

##### step 33 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ] ^= ( F[ gamma + 2 ] ^= ( F[ delta + 2 ] ^= ( F[ epsilon + 2 ] ^= ( F[ zeta + 2 ])))))))))

movdqa 272(%rdi,%r14), %xmm14
pxor 432(%rdi,%r13), %xmm14
movdqa %xmm14, 432(%rdi,%r13)
pxor 512(%rdi,%r12), %xmm14
movdqa %xmm14, 512(%rdi,%r12)
pxor 528(%rdi,%r11), %xmm14
movdqa %xmm14, 528(%rdi,%r11)
pxor 528(%rdi,%r10), %xmm14
movdqa %xmm14, 528(%rdi,%r10)
pxor 528(%rdi,%rax), %xmm14
movdqa %xmm14, 528(%rdi,%rax)
pxor 528(%rdi), %xmm14
movdqa %xmm14, 528(%rdi)
pxor %xmm14, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_33
._step_33_end:

##### step 34 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ] ^= ( F[ gamma + 2 ] ^= ( F[ delta + 2 ] ^= ( F[ epsilon + 2 ] ^= ( F[ zeta + 2 ])))))))))

movdqa 288(%rdi,%r14), %xmm14
pxor 448(%rdi,%r13), %xmm14
movdqa %xmm14, 448(%rdi,%r13)
pxor 528(%rdi,%r12), %xmm14
movdqa %xmm14, 528(%rdi,%r12)
pxor 544(%rdi,%r11), %xmm14
movdqa %xmm14, 544(%rdi,%r11)
pxor 544(%rdi,%r10), %xmm14
movdqa %xmm14, 544(%rdi,%r10)
pxor 544(%rdi,%rax), %xmm14
movdqa %xmm14, 544(%rdi,%rax)
pxor 544(%rdi), %xmm14
movdqa %xmm14, 544(%rdi)
pxor %xmm14, %xmm2
pxor %xmm2, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_34
._step_34_end:

##### step 35 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ] ^= ( F[ beta + 3 ] ^= ( F[ gamma + 3 ] ^= ( F[ delta + 3 ] ^= ( F[ epsilon + 3 ])))))))))

movdqa 464(%rdi,%r13), %xmm14
pxor 544(%rdi,%r12), %xmm14
movdqa %xmm14, 544(%rdi,%r12)
pxor 560(%rdi,%r11), %xmm14
movdqa %xmm14, 560(%rdi,%r11)
pxor 560(%rdi,%r10), %xmm14
movdqa %xmm14, 560(%rdi,%r10)
pxor 560(%rdi,%rax), %xmm14
movdqa %xmm14, 560(%rdi,%rax)
pxor 560(%rdi), %xmm14
movdqa %xmm14, 560(%rdi)
pxor %xmm14, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_35
._step_35_end:

##### step 36 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ] ^= ( F[ gamma + 2 ] ^= ( F[ delta + 2 ] ^= ( F[ epsilon + 2 ] ^= ( F[ zeta + 2 ])))))))))

movdqa 304(%rdi,%r14), %xmm14
pxor 480(%rdi,%r13), %xmm14
movdqa %xmm14, 480(%rdi,%r13)
pxor 560(%rdi,%r12), %xmm14
movdqa %xmm14, 560(%rdi,%r12)
pxor 576(%rdi,%r11), %xmm14
movdqa %xmm14, 576(%rdi,%r11)
pxor 576(%rdi,%r10), %xmm14
movdqa %xmm14, 576(%rdi,%r10)
pxor 576(%rdi,%rax), %xmm14
movdqa %xmm14, 576(%rdi,%rax)
pxor 576(%rdi), %xmm14
movdqa %xmm14, 576(%rdi)
pxor %xmm14, %xmm4
pxor %xmm4, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_36
._step_36_end:

##### step 37 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ] ^= ( F[ beta + 3 ] ^= ( F[ gamma + 3 ] ^= ( F[ delta + 3 ] ^= ( F[ epsilon + 3 ])))))))))

movdqa 496(%rdi,%r13), %xmm14
pxor 576(%rdi,%r12), %xmm14
movdqa %xmm14, 576(%rdi,%r12)
pxor 592(%rdi,%r11), %xmm14
movdqa %xmm14, 592(%rdi,%r11)
pxor 592(%rdi,%r10), %xmm14
movdqa %xmm14, 592(%rdi,%r10)
pxor 592(%rdi,%rax), %xmm14
movdqa %xmm14, 592(%rdi,%rax)
pxor 592(%rdi), %xmm14
movdqa %xmm14, 592(%rdi)
pxor %xmm14, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_37
._step_37_end:

##### step 38 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ] ^= ( F[ beta + 3 ] ^= ( F[ gamma + 3 ] ^= ( F[ delta + 3 ] ^= ( F[ epsilon + 3 ])))))))))

movdqa 512(%rdi,%r13), %xmm14
pxor 592(%rdi,%r12), %xmm14
movdqa %xmm14, 592(%rdi,%r12)
pxor 608(%rdi,%r11), %xmm14
movdqa %xmm14, 608(%rdi,%r11)
pxor 608(%rdi,%r10), %xmm14
movdqa %xmm14, 608(%rdi,%r10)
pxor 608(%rdi,%rax), %xmm14
movdqa %xmm14, 608(%rdi,%rax)
pxor 608(%rdi), %xmm14
movdqa %xmm14, 608(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_38
._step_38_end:

##### step 39 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ alpha + 4 ] ^= ( F[ beta + 4 ] ^= ( F[ gamma + 4 ] ^= ( F[ delta + 4 ])))))))))

movdqa 608(%rdi,%r12), %xmm14
pxor 624(%rdi,%r11), %xmm14
movdqa %xmm14, 624(%rdi,%r11)
pxor 624(%rdi,%r10), %xmm14
movdqa %xmm14, 624(%rdi,%r10)
pxor 624(%rdi,%rax), %xmm14
movdqa %xmm14, 624(%rdi,%rax)
pxor 624(%rdi), %xmm14
movdqa %xmm14, 624(%rdi)
pxor %xmm14, %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_39
._step_39_end:

##### step 40 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ] ^= ( F[ gamma + 2 ] ^= ( F[ delta + 2 ] ^= ( F[ epsilon + 2 ] ^= ( F[ zeta + 2 ])))))))))

movdqa 320(%rdi,%r14), %xmm14
pxor 528(%rdi,%r13), %xmm14
movdqa %xmm14, 528(%rdi,%r13)
pxor 624(%rdi,%r12), %xmm14
movdqa %xmm14, 624(%rdi,%r12)
pxor 640(%rdi,%r11), %xmm14
movdqa %xmm14, 640(%rdi,%r11)
pxor 640(%rdi,%r10), %xmm14
movdqa %xmm14, 640(%rdi,%r10)
pxor 640(%rdi,%rax), %xmm14
movdqa %xmm14, 640(%rdi,%rax)
pxor 640(%rdi), %xmm14
movdqa %xmm14, 640(%rdi)
pxor %xmm14, %xmm8
pxor %xmm8, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_40
._step_40_end:

##### step 41 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ] ^= ( F[ beta + 3 ] ^= ( F[ gamma + 3 ] ^= ( F[ delta + 3 ] ^= ( F[ epsilon + 3 ])))))))))

movdqa 544(%rdi,%r13), %xmm14
pxor 640(%rdi,%r12), %xmm14
movdqa %xmm14, 640(%rdi,%r12)
pxor 656(%rdi,%r11), %xmm14
movdqa %xmm14, 656(%rdi,%r11)
pxor 656(%rdi,%r10), %xmm14
movdqa %xmm14, 656(%rdi,%r10)
pxor 656(%rdi,%rax), %xmm14
movdqa %xmm14, 656(%rdi,%rax)
pxor 656(%rdi), %xmm14
movdqa %xmm14, 656(%rdi)
pxor %xmm14, %xmm9
pxor %xmm9, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_41
._step_41_end:

##### step 42 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ] ^= ( F[ beta + 3 ] ^= ( F[ gamma + 3 ] ^= ( F[ delta + 3 ] ^= ( F[ epsilon + 3 ])))))))))

movdqa 560(%rdi,%r13), %xmm14
pxor 656(%rdi,%r12), %xmm14
movdqa %xmm14, 656(%rdi,%r12)
pxor 672(%rdi,%r11), %xmm14
movdqa %xmm14, 672(%rdi,%r11)
pxor 672(%rdi,%r10), %xmm14
movdqa %xmm14, 672(%rdi,%r10)
pxor 672(%rdi,%rax), %xmm14
movdqa %xmm14, 672(%rdi,%rax)
pxor 672(%rdi), %xmm14
movdqa %xmm14, 672(%rdi)
pxor %xmm14, %xmm10
pxor %xmm10, %xmm2
pxor %xmm2, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_42
._step_42_end:

##### step 43 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ alpha + 4 ] ^= ( F[ beta + 4 ] ^= ( F[ gamma + 4 ] ^= ( F[ delta + 4 ])))))))))

movdqa 672(%rdi,%r12), %xmm14
pxor 688(%rdi,%r11), %xmm14
movdqa %xmm14, 688(%rdi,%r11)
pxor 688(%rdi,%r10), %xmm14
movdqa %xmm14, 688(%rdi,%r10)
pxor 688(%rdi,%rax), %xmm14
movdqa %xmm14, 688(%rdi,%rax)
pxor 688(%rdi), %xmm14
movdqa %xmm14, 688(%rdi)
pxor %xmm14, %xmm11
pxor %xmm11, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_43
._step_43_end:

##### step 44 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ] ^= ( F[ beta + 3 ] ^= ( F[ gamma + 3 ] ^= ( F[ delta + 3 ] ^= ( F[ epsilon + 3 ])))))))))

movdqa 576(%rdi,%r13), %xmm14
pxor 688(%rdi,%r12), %xmm14
movdqa %xmm14, 688(%rdi,%r12)
pxor 704(%rdi,%r11), %xmm14
movdqa %xmm14, 704(%rdi,%r11)
pxor 704(%rdi,%r10), %xmm14
movdqa %xmm14, 704(%rdi,%r10)
pxor 704(%rdi,%rax), %xmm14
movdqa %xmm14, 704(%rdi,%rax)
pxor 704(%rdi), %xmm14
movdqa %xmm14, 704(%rdi)
pxor %xmm14, %xmm12
pxor %xmm12, %xmm4
pxor %xmm4, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_44
._step_44_end:

##### step 45 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ alpha + 4 ] ^= ( F[ beta + 4 ] ^= ( F[ gamma + 4 ] ^= ( F[ delta + 4 ])))))))))

movdqa 704(%rdi,%r12), %xmm14
pxor 720(%rdi,%r11), %xmm14
movdqa %xmm14, 720(%rdi,%r11)
pxor 720(%rdi,%r10), %xmm14
movdqa %xmm14, 720(%rdi,%r10)
pxor 720(%rdi,%rax), %xmm14
movdqa %xmm14, 720(%rdi,%rax)
pxor 720(%rdi), %xmm14
movdqa %xmm14, 720(%rdi)
pxor %xmm14, %xmm13
pxor %xmm13, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_45
._step_45_end:

##### step 46 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ alpha + 4 ] ^= ( F[ beta + 4 ] ^= ( F[ gamma + 4 ] ^= ( F[ delta + 4 ])))))))))

movdqa 720(%rdi,%r12), %xmm14
pxor 736(%rdi,%r11), %xmm14
movdqa %xmm14, 736(%rdi,%r11)
pxor 736(%rdi,%r10), %xmm14
movdqa %xmm14, 736(%rdi,%r10)
pxor 736(%rdi,%rax), %xmm14
movdqa %xmm14, 736(%rdi,%rax)
pxor 736(%rdi), %xmm14
movdqa %xmm14, 736(%rdi)
pxor 224(%rdi), %xmm14
movdqa %xmm14, 224(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_46
._step_46_end:

##### step 47 [hw=5]
##### F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ alpha + 5 ] ^= ( F[ beta + 5 ] ^= ( F[ gamma + 5 ])))))))))

movdqa 752(%rdi,%r11), %xmm14
pxor 752(%rdi,%r10), %xmm14
movdqa %xmm14, 752(%rdi,%r10)
pxor 752(%rdi,%rax), %xmm14
movdqa %xmm14, 752(%rdi,%rax)
pxor 752(%rdi), %xmm14
movdqa %xmm14, 752(%rdi)
pxor 240(%rdi), %xmm14
movdqa %xmm14, 240(%rdi)
pxor %xmm14, %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_47
._step_47_end:

##### step 48 [hw=2]
##### F[ 2 ] ^= ( F[ 2 ] ^= ( F[ 2 ] ^= ( F[ alpha + 2 ] ^= ( F[ beta + 2 ] ^= ( F[ gamma + 2 ] ^= ( F[ delta + 2 ] ^= ( F[ epsilon + 2 ] ^= ( F[ zeta + 2 ])))))))))

movdqa 336(%rdi,%r14), %xmm14
pxor 592(%rdi,%r13), %xmm14
movdqa %xmm14, 592(%rdi,%r13)
pxor 736(%rdi,%r12), %xmm14
movdqa %xmm14, 736(%rdi,%r12)
pxor 768(%rdi,%r11), %xmm14
movdqa %xmm14, 768(%rdi,%r11)
pxor 768(%rdi,%r10), %xmm14
movdqa %xmm14, 768(%rdi,%r10)
pxor 768(%rdi,%rax), %xmm14
movdqa %xmm14, 768(%rdi,%rax)
pxor 768(%rdi), %xmm14
movdqa %xmm14, 768(%rdi)
pxor 256(%rdi), %xmm14
movdqa %xmm14, 256(%rdi)
pxor %xmm14, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_48
._step_48_end:

##### step 49 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ] ^= ( F[ beta + 3 ] ^= ( F[ gamma + 3 ] ^= ( F[ delta + 3 ] ^= ( F[ epsilon + 3 ])))))))))

movdqa 608(%rdi,%r13), %xmm14
pxor 752(%rdi,%r12), %xmm14
movdqa %xmm14, 752(%rdi,%r12)
pxor 784(%rdi,%r11), %xmm14
movdqa %xmm14, 784(%rdi,%r11)
pxor 784(%rdi,%r10), %xmm14
movdqa %xmm14, 784(%rdi,%r10)
pxor 784(%rdi,%rax), %xmm14
movdqa %xmm14, 784(%rdi,%rax)
pxor 784(%rdi), %xmm14
movdqa %xmm14, 784(%rdi)
pxor 272(%rdi), %xmm14
movdqa %xmm14, 272(%rdi)
pxor %xmm14, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_49
._step_49_end:

##### step 50 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ] ^= ( F[ beta + 3 ] ^= ( F[ gamma + 3 ] ^= ( F[ delta + 3 ] ^= ( F[ epsilon + 3 ])))))))))

movdqa 624(%rdi,%r13), %xmm14
pxor 768(%rdi,%r12), %xmm14
movdqa %xmm14, 768(%rdi,%r12)
pxor 800(%rdi,%r11), %xmm14
movdqa %xmm14, 800(%rdi,%r11)
pxor 800(%rdi,%r10), %xmm14
movdqa %xmm14, 800(%rdi,%r10)
pxor 800(%rdi,%rax), %xmm14
movdqa %xmm14, 800(%rdi,%rax)
pxor 800(%rdi), %xmm14
movdqa %xmm14, 800(%rdi)
pxor 288(%rdi), %xmm14
movdqa %xmm14, 288(%rdi)
pxor %xmm14, %xmm2
pxor %xmm2, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_50
._step_50_end:

##### step 51 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ alpha + 4 ] ^= ( F[ beta + 4 ] ^= ( F[ gamma + 4 ] ^= ( F[ delta + 4 ])))))))))

movdqa 784(%rdi,%r12), %xmm14
pxor 816(%rdi,%r11), %xmm14
movdqa %xmm14, 816(%rdi,%r11)
pxor 816(%rdi,%r10), %xmm14
movdqa %xmm14, 816(%rdi,%r10)
pxor 816(%rdi,%rax), %xmm14
movdqa %xmm14, 816(%rdi,%rax)
pxor 816(%rdi), %xmm14
movdqa %xmm14, 816(%rdi)
pxor 304(%rdi), %xmm14
movdqa %xmm14, 304(%rdi)
pxor %xmm14, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_51
._step_51_end:

##### step 52 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ] ^= ( F[ beta + 3 ] ^= ( F[ gamma + 3 ] ^= ( F[ delta + 3 ] ^= ( F[ epsilon + 3 ])))))))))

movdqa 640(%rdi,%r13), %xmm14
pxor 800(%rdi,%r12), %xmm14
movdqa %xmm14, 800(%rdi,%r12)
pxor 832(%rdi,%r11), %xmm14
movdqa %xmm14, 832(%rdi,%r11)
pxor 832(%rdi,%r10), %xmm14
movdqa %xmm14, 832(%rdi,%r10)
pxor 832(%rdi,%rax), %xmm14
movdqa %xmm14, 832(%rdi,%rax)
pxor 832(%rdi), %xmm14
movdqa %xmm14, 832(%rdi)
pxor 320(%rdi), %xmm14
movdqa %xmm14, 320(%rdi)
pxor %xmm14, %xmm4
pxor %xmm4, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_52
._step_52_end:

##### step 53 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ alpha + 4 ] ^= ( F[ beta + 4 ] ^= ( F[ gamma + 4 ] ^= ( F[ delta + 4 ])))))))))

movdqa 816(%rdi,%r12), %xmm14
pxor 848(%rdi,%r11), %xmm14
movdqa %xmm14, 848(%rdi,%r11)
pxor 848(%rdi,%r10), %xmm14
movdqa %xmm14, 848(%rdi,%r10)
pxor 848(%rdi,%rax), %xmm14
movdqa %xmm14, 848(%rdi,%rax)
pxor 848(%rdi), %xmm14
movdqa %xmm14, 848(%rdi)
pxor 336(%rdi), %xmm14
movdqa %xmm14, 336(%rdi)
pxor %xmm14, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_53
._step_53_end:

##### step 54 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ alpha + 4 ] ^= ( F[ beta + 4 ] ^= ( F[ gamma + 4 ] ^= ( F[ delta + 4 ])))))))))

movdqa 832(%rdi,%r12), %xmm14
pxor 864(%rdi,%r11), %xmm14
movdqa %xmm14, 864(%rdi,%r11)
pxor 864(%rdi,%r10), %xmm14
movdqa %xmm14, 864(%rdi,%r10)
pxor 864(%rdi,%rax), %xmm14
movdqa %xmm14, 864(%rdi,%rax)
pxor 864(%rdi), %xmm14
movdqa %xmm14, 864(%rdi)
pxor 352(%rdi), %xmm14
movdqa %xmm14, 352(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_54
._step_54_end:

##### step 55 [hw=5]
##### F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ alpha + 5 ] ^= ( F[ beta + 5 ] ^= ( F[ gamma + 5 ])))))))))

movdqa 880(%rdi,%r11), %xmm14
pxor 880(%rdi,%r10), %xmm14
movdqa %xmm14, 880(%rdi,%r10)
pxor 880(%rdi,%rax), %xmm14
movdqa %xmm14, 880(%rdi,%rax)
pxor 880(%rdi), %xmm14
movdqa %xmm14, 880(%rdi)
pxor 368(%rdi), %xmm14
movdqa %xmm14, 368(%rdi)
pxor %xmm14, %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_55
._step_55_end:

##### step 56 [hw=3]
##### F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ 3 ] ^= ( F[ alpha + 3 ] ^= ( F[ beta + 3 ] ^= ( F[ gamma + 3 ] ^= ( F[ delta + 3 ] ^= ( F[ epsilon + 3 ])))))))))

movdqa 656(%rdi,%r13), %xmm14
pxor 848(%rdi,%r12), %xmm14
movdqa %xmm14, 848(%rdi,%r12)
pxor 896(%rdi,%r11), %xmm14
movdqa %xmm14, 896(%rdi,%r11)
pxor 896(%rdi,%r10), %xmm14
movdqa %xmm14, 896(%rdi,%r10)
pxor 896(%rdi,%rax), %xmm14
movdqa %xmm14, 896(%rdi,%rax)
pxor 896(%rdi), %xmm14
movdqa %xmm14, 896(%rdi)
pxor 384(%rdi), %xmm14
movdqa %xmm14, 384(%rdi)
pxor %xmm14, %xmm8
pxor %xmm8, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_56
._step_56_end:

##### step 57 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ alpha + 4 ] ^= ( F[ beta + 4 ] ^= ( F[ gamma + 4 ] ^= ( F[ delta + 4 ])))))))))

movdqa 864(%rdi,%r12), %xmm14
pxor 912(%rdi,%r11), %xmm14
movdqa %xmm14, 912(%rdi,%r11)
pxor 912(%rdi,%r10), %xmm14
movdqa %xmm14, 912(%rdi,%r10)
pxor 912(%rdi,%rax), %xmm14
movdqa %xmm14, 912(%rdi,%rax)
pxor 912(%rdi), %xmm14
movdqa %xmm14, 912(%rdi)
pxor 400(%rdi), %xmm14
movdqa %xmm14, 400(%rdi)
pxor %xmm14, %xmm9
pxor %xmm9, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_57
._step_57_end:

##### step 58 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ alpha + 4 ] ^= ( F[ beta + 4 ] ^= ( F[ gamma + 4 ] ^= ( F[ delta + 4 ])))))))))

movdqa 880(%rdi,%r12), %xmm14
pxor 928(%rdi,%r11), %xmm14
movdqa %xmm14, 928(%rdi,%r11)
pxor 928(%rdi,%r10), %xmm14
movdqa %xmm14, 928(%rdi,%r10)
pxor 928(%rdi,%rax), %xmm14
movdqa %xmm14, 928(%rdi,%rax)
pxor 928(%rdi), %xmm14
movdqa %xmm14, 928(%rdi)
pxor 416(%rdi), %xmm14
movdqa %xmm14, 416(%rdi)
pxor %xmm14, %xmm10
pxor %xmm10, %xmm2
pxor %xmm2, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_58
._step_58_end:

##### step 59 [hw=5]
##### F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ alpha + 5 ] ^= ( F[ beta + 5 ] ^= ( F[ gamma + 5 ])))))))))

movdqa 944(%rdi,%r11), %xmm14
pxor 944(%rdi,%r10), %xmm14
movdqa %xmm14, 944(%rdi,%r10)
pxor 944(%rdi,%rax), %xmm14
movdqa %xmm14, 944(%rdi,%rax)
pxor 944(%rdi), %xmm14
movdqa %xmm14, 944(%rdi)
pxor 432(%rdi), %xmm14
movdqa %xmm14, 432(%rdi)
pxor %xmm14, %xmm11
pxor %xmm11, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_59
._step_59_end:

##### step 60 [hw=4]
##### F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ 4 ] ^= ( F[ alpha + 4 ] ^= ( F[ beta + 4 ] ^= ( F[ gamma + 4 ] ^= ( F[ delta + 4 ])))))))))

movdqa 896(%rdi,%r12), %xmm14
pxor 960(%rdi,%r11), %xmm14
movdqa %xmm14, 960(%rdi,%r11)
pxor 960(%rdi,%r10), %xmm14
movdqa %xmm14, 960(%rdi,%r10)
pxor 960(%rdi,%rax), %xmm14
movdqa %xmm14, 960(%rdi,%rax)
pxor 960(%rdi), %xmm14
movdqa %xmm14, 960(%rdi)
pxor 448(%rdi), %xmm14
movdqa %xmm14, 448(%rdi)
pxor %xmm14, %xmm12
pxor %xmm12, %xmm4
pxor %xmm4, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_60
._step_60_end:

##### step 61 [hw=5]
##### F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ alpha + 5 ] ^= ( F[ beta + 5 ] ^= ( F[ gamma + 5 ])))))))))

movdqa 976(%rdi,%r11), %xmm14
pxor 976(%rdi,%r10), %xmm14
movdqa %xmm14, 976(%rdi,%r10)
pxor 976(%rdi,%rax), %xmm14
movdqa %xmm14, 976(%rdi,%rax)
pxor 976(%rdi), %xmm14
movdqa %xmm14, 976(%rdi)
pxor 464(%rdi), %xmm14
movdqa %xmm14, 464(%rdi)
pxor %xmm14, %xmm13
pxor %xmm13, %xmm5
pxor %xmm5, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_61
._step_61_end:

##### step 62 [hw=5]
##### F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ 5 ] ^= ( F[ alpha + 5 ] ^= ( F[ beta + 5 ] ^= ( F[ gamma + 5 ])))))))))

movdqa 992(%rdi,%r11), %xmm14
pxor 992(%rdi,%r10), %xmm14
movdqa %xmm14, 992(%rdi,%r10)
pxor 992(%rdi,%rax), %xmm14
movdqa %xmm14, 992(%rdi,%rax)
pxor 992(%rdi), %xmm14
movdqa %xmm14, 992(%rdi)
pxor 480(%rdi), %xmm14
movdqa %xmm14, 480(%rdi)
pxor 224(%rdi), %xmm14
movdqa %xmm14, 224(%rdi)
pxor %xmm14, %xmm6
pxor %xmm6, %xmm2
pxor %xmm2, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_62
._step_62_end:

##### step 63 [hw=6]
##### F[ 6 ] ^= ( F[ 6 ] ^= ( F[ 6 ] ^= ( F[ 6 ] ^= ( F[ 6 ] ^= ( F[ 6 ] ^= ( F[ 6 ] ^= ( F[ alpha + 6 ] ^= ( F[ beta + 6 ])))))))))

movdqa 1008(%rdi,%r10), %xmm14
pxor 1008(%rdi,%rax), %xmm14
movdqa %xmm14, 1008(%rdi,%rax)
pxor 1008(%rdi), %xmm14
movdqa %xmm14, 1008(%rdi)
pxor 496(%rdi), %xmm14
movdqa %xmm14, 496(%rdi)
pxor 240(%rdi), %xmm14
movdqa %xmm14, 240(%rdi)
pxor %xmm14, %xmm7
pxor %xmm7, %xmm3
pxor %xmm3, %xmm1
pxor %xmm1, %xmm0
pcmpeqb %xmm0, %xmm15
pmovmskb %xmm15, %ebx
test %ebx, %ebx
jne ._report_solution_63
._step_63_end:
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
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
movl %ebx, (%rdx, %r9)
shr $4, %r9
add  $1, %r9
jmp ._step_63_end

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
