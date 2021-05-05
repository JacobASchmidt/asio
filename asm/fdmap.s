	.file	"fdmap.c"
	.text
	.p2align 4
	.globl	fdmap_create
	.type	fdmap_create, @function
fdmap_create:
.LFB16:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	movslq	%esi, %rdi
	movq	%rdi, %rbp
	salq	$4, %rdi
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	call	malloc@PLT
	movl	%ebp, 8(%rbx)
	movq	%rax, (%rbx)
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE16:
	.size	fdmap_create, .-fdmap_create
	.p2align 4
	.globl	fdmap_add
	.type	fdmap_add, @function
fdmap_add:
.LFB17:
	.cfi_startproc
	endbr64
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	movq	%rcx, %r13
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	movq	%rdx, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	movslq	%esi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 48
	movl	8(%rdi), %edi
	cmpl	%ebx, %edi
	jle	.L5
	movq	0(%rbp), %rax
.L6:
	salq	$4, %rbx
	movq	%r12, (%rax,%rbx)
	movq	%r13, 8(%rax,%rbx)
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L5:
	.cfi_restore_state
	leal	1(%rbx), %eax
	addl	$1, %edi
	orl	%eax, %edi
	call	bit_ceil@PLT
	movq	0(%rbp), %rdi
	movl	%eax, 8(%rbp)
	movslq	%eax, %rsi
	salq	$4, %rsi
	call	realloc@PLT
	movq	%rax, 0(%rbp)
	jmp	.L6
	.cfi_endproc
.LFE17:
	.size	fdmap_add, .-fdmap_add
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"src/fdmap.c"
.LC1:
	.string	"fd < fdmap->cap"
	.text
	.p2align 4
	.globl	fdmap_get
	.type	fdmap_get, @function
fdmap_get:
.LFB18:
	.cfi_startproc
	endbr64
	cmpl	%esi, 8(%rdi)
	jle	.L13
	movslq	%esi, %rsi
	salq	$4, %rsi
	addq	(%rdi), %rsi
	movq	(%rsi), %rax
	movq	8(%rsi), %rdx
	ret
.L13:
	pushq	%rax
	.cfi_def_cfa_offset 16
	leaq	__PRETTY_FUNCTION__.2668(%rip), %rcx
	movl	$24, %edx
	leaq	.LC0(%rip), %rsi
	leaq	.LC1(%rip), %rdi
	call	__assert_fail@PLT
	.cfi_endproc
.LFE18:
	.size	fdmap_get, .-fdmap_get
	.p2align 4
	.globl	fdmap_set
	.type	fdmap_set, @function
fdmap_set:
.LFB19:
	.cfi_startproc
	endbr64
	cmpl	%esi, 8(%rdi)
	jle	.L19
	movslq	%esi, %rsi
	salq	$4, %rsi
	addq	(%rdi), %rsi
	movq	%rdx, (%rsi)
	movq	%rcx, 8(%rsi)
	ret
.L19:
	pushq	%rax
	.cfi_def_cfa_offset 16
	leaq	__PRETTY_FUNCTION__.2674(%rip), %rcx
	movl	$29, %edx
	leaq	.LC0(%rip), %rsi
	leaq	.LC1(%rip), %rdi
	call	__assert_fail@PLT
	.cfi_endproc
.LFE19:
	.size	fdmap_set, .-fdmap_set
	.p2align 4
	.globl	fdmap_destroy
	.type	fdmap_destroy, @function
fdmap_destroy:
.LFB20:
	.cfi_startproc
	endbr64
	movq	(%rdi), %rdi
	jmp	free@PLT
	.cfi_endproc
.LFE20:
	.size	fdmap_destroy, .-fdmap_destroy
	.section	.rodata
	.align 8
	.type	__PRETTY_FUNCTION__.2674, @object
	.size	__PRETTY_FUNCTION__.2674, 10
__PRETTY_FUNCTION__.2674:
	.string	"fdmap_set"
	.align 8
	.type	__PRETTY_FUNCTION__.2668, @object
	.size	__PRETTY_FUNCTION__.2668, 10
__PRETTY_FUNCTION__.2668:
	.string	"fdmap_get"
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
