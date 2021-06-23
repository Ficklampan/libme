	.text
	.file	"format_test.cpp"
	.globl	_Z11test_printfPKcz             # -- Begin function _Z11test_printfPKcz
	.p2align	4, 0x90
	.type	_Z11test_printfPKcz,@function
_Z11test_printfPKcz:                    # @_Z11test_printfPKcz
	.cfi_startproc
# %bb.0:
	subq	$1240, %rsp                     # imm = 0x4D8
	.cfi_def_cfa_offset 1248
	movq	%rdi, %r10
	testb	%al, %al
	je	.LBB0_2
# %bb.1:
	movaps	%xmm0, 48(%rsp)
	movaps	%xmm1, 64(%rsp)
	movaps	%xmm2, 80(%rsp)
	movaps	%xmm3, 96(%rsp)
	movaps	%xmm4, 112(%rsp)
	movaps	%xmm5, 128(%rsp)
	movaps	%xmm6, 144(%rsp)
	movaps	%xmm7, 160(%rsp)
.LBB0_2:
	movq	%rsi, 8(%rsp)
	movq	%rdx, 16(%rsp)
	movq	%rcx, 24(%rsp)
	movq	%r8, 32(%rsp)
	movq	%r9, 40(%rsp)
	movq	%fs:40, %rax
	movq	%rax, 1232(%rsp)
	movq	%rsp, %rax
	movq	%rax, 1216(%rsp)
	leaq	1248(%rsp), %rax
	movq	%rax, 1208(%rsp)
	movabsq	$206158430216, %rax             # imm = 0x3000000008
	movq	%rax, 1200(%rsp)
	leaq	176(%rsp), %rdi
	leaq	1200(%rsp), %rdx
	movq	%r10, %rsi
	callq	vsprintf@PLT
	movq	%fs:40, %rax
	cmpq	1232(%rsp), %rax
	jne	.LBB0_4
# %bb.3:
	xorl	%eax, %eax
	addq	$1240, %rsp                     # imm = 0x4D8
	.cfi_def_cfa_offset 8
	retq
.LBB0_4:
	.cfi_def_cfa_offset 1248
	callq	__stack_chk_fail@PLT
.Lfunc_end0:
	.size	_Z11test_printfPKcz, .Lfunc_end0-_Z11test_printfPKcz
	.cfi_endproc
                                        # -- End function
	.globl	_Z8get_timev                    # -- Begin function _Z8get_timev
	.p2align	4, 0x90
	.type	_Z8get_timev,@function
_Z8get_timev:                           # @_Z8get_timev
	.cfi_startproc
# %bb.0:
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movq	%fs:40, %rax
	movq	%rax, 16(%rsp)
	movq	%rsp, %rdi
	xorl	%esi, %esi
	callq	gettimeofday@PLT
	movq	8(%rsp), %rax
	movq	%fs:40, %rcx
	cmpq	16(%rsp), %rcx
	jne	.LBB1_2
# %bb.1:
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.LBB1_2:
	.cfi_def_cfa_offset 32
	callq	__stack_chk_fail@PLT
.Lfunc_end1:
	.size	_Z8get_timev, .Lfunc_end1-_Z8get_timev
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	subq	$1032, %rsp                     # imm = 0x408
	.cfi_def_cfa_offset 1040
	movq	%fs:40, %rax
	movq	%rax, 1024(%rsp)
	movq	%rsp, %rax
	leaq	.L.str(%rip), %rcx
	.p2align	4, 0x90
.LBB2_1:                                # =>This Loop Header: Depth=1
                                        #     Child Loop BB2_5 Depth 2
	movb	(%rcx), %dl
	cmpb	$123, %dl
	je	.LBB2_4
# %bb.2:                                #   in Loop: Header=BB2_1 Depth=1
	testb	%dl, %dl
	je	.LBB2_7
# %bb.3:                                #   in Loop: Header=BB2_1 Depth=1
	addq	$1, %rcx
	movb	%dl, (%rax)
	addq	$1, %rax
	jmp	.LBB2_1
	.p2align	4, 0x90
.LBB2_4:                                #   in Loop: Header=BB2_1 Depth=1
	addq	$1, %rcx
	.p2align	4, 0x90
.LBB2_5:                                #   Parent Loop BB2_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	cmpb	$125, (%rcx)
	leaq	1(%rcx), %rcx
	jne	.LBB2_5
# %bb.6:                                #   in Loop: Header=BB2_1 Depth=1
	movl	$1769366597, (%rax)             # imm = 0x69766445
	movb	$110, 4(%rax)
	addq	$5, %rax
	jmp	.LBB2_1
.LBB2_7:
	movb	$0, (%rax)
	leaq	.L.str.3(%rip), %rdi
	movq	%rsp, %rsi
	xorl	%eax, %eax
	callq	printf@PLT
	movq	%fs:40, %rax
	cmpq	1024(%rsp), %rax
	jne	.LBB2_9
# %bb.8:
	xorl	%eax, %eax
	addq	$1032, %rsp                     # imm = 0x408
	.cfi_def_cfa_offset 8
	retq
.LBB2_9:
	.cfi_def_cfa_offset 1040
	callq	__stack_chk_fail@PLT
.Lfunc_end2:
	.size	main, .Lfunc_end2-main
	.cfi_endproc
                                        # -- End function
	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"Hello {} and hello {}! :D"
	.size	.L.str, 26

	.type	.L.str.3,@object                # @.str.3
.L.str.3:
	.asciz	"string: [%s]\n"
	.size	.L.str.3, 14

	.section	".linker-options","e",@llvm_linker_options
	.ident	"clang version 12.0.0"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym __stack_chk_fail
