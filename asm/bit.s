	.file	"bit.c"
	.text
	.p2align 4
	.globl	bit_clz
	.type	bit_clz, @function
bit_clz:
.LFB0:
	.cfi_startproc
	endbr64
	bsrl	%edi, %eax
	xorl	$31, %eax
	ret
	.cfi_endproc
.LFE0:
	.size	bit_clz, .-bit_clz
	.p2align 4
	.globl	bit_ceil
	.type	bit_ceil, @function
bit_ceil:
.LFB1:
	.cfi_startproc
	endbr64
	movl	$1, %eax
	cmpl	$1, %edi
	jle	.L3
	subl	$1, %edi
	movl	$32, %ecx
	bsrl	%edi, %edi
	xorl	$31, %edi
	subl	%edi, %ecx
	sall	%cl, %eax
.L3:
	ret
	.cfi_endproc
.LFE1:
	.size	bit_ceil, .-bit_ceil
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
