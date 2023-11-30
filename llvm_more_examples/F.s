	.text
	.file	"F.ll"
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3                               # -- Begin function F
.LCPI0_0:
	.quad	0x3ff0000000000000              # double 1
	.text
	.globl	F
	.p2align	4, 0x90
	.type	F,@function
F:                                      # @F
	.cfi_startproc
# %bb.0:                                # %entry
	movsd	%xmm0, -8(%rsp)
	addsd	.LCPI0_0(%rip), %xmm0
	retq
.Lfunc_end0:
	.size	F, .Lfunc_end0-F
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
