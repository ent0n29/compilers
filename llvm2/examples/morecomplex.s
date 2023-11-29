	.text
	.file	"morecomplex.ll"
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function f
.LCPI0_0:
	.quad	0x4008000000000000              # double 3
	.text
	.globl	f
	.p2align	4, 0x90
	.type	f,@function
f:                                      # @f
	.cfi_startproc
# %bb.0:                                # %entry
	movsd	%xmm0, -16(%rsp)
	movsd	%xmm1, -8(%rsp)
	movapd	%xmm0, %xmm2
	mulsd	%xmm0, %xmm2
	addsd	%xmm0, %xmm0
	mulsd	%xmm1, %xmm0
	subsd	%xmm0, %xmm2
	movsd	.LCPI0_0(%rip), %xmm0           # xmm0 = mem[0],zero
	mulsd	%xmm1, %xmm0
	mulsd	%xmm1, %xmm0
	addsd	%xmm2, %xmm0
	retq
.Lfunc_end0:
	.size	f, .Lfunc_end0-f
	.cfi_endproc
                                        # -- End function
	.globl	helper                          # -- Begin function helper
	.p2align	4, 0x90
	.type	helper,@function
helper:                                 # @helper
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	callq	x@PLT
	movsd	%xmm0, (%rsp)                   # 8-byte Spill
	callq	y@PLT
	movaps	%xmm0, %xmm1
	movsd	(%rsp), %xmm0                   # 8-byte Reload
                                        # xmm0 = mem[0],zero
	callq	f@PLT
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	helper, .Lfunc_end1-helper
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	callq	helper@PLT
	callq	printval@PLT
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end2:
	.size	main, .Lfunc_end2-main
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
