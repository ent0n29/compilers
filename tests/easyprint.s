	.text
	.file	"easyprint.ll"
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %init
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -16
	movl	$0, 12(%rsp)
	movq	fmt@GOTPCREL(%rip), %rbx
	.p2align	4, 0x90
.LBB0_1:                                # %loop
                                        # =>This Inner Loop Header: Depth=1
	movl	12(%rsp), %esi
	incl	%esi
	cmpl	$11, %esi
	je	.LBB0_3
# %bb.2:                                # %cont
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	%esi, 12(%rsp)
	movq	%rbx, %rdi
	callq	printf@PLT
	jmp	.LBB0_1
.LBB0_3:                                # %exit
	xorl	%eax, %eax
	addq	$16, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	fmt,@object                     # @fmt
	.section	.rodata,"a",@progbits
	.globl	fmt
fmt:
	.asciz	"%d\n"
	.size	fmt, 4

	.section	".note.GNU-stack","",@progbits
