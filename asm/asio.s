	.file	"asio.c"
	.text
	.p2align 4
	.globl	asio_create
	.type	asio_create, @function
asio_create:
.LFB16:
	.cfi_startproc
	endbr64
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	leaq	8(%rdi), %rdi
	call	fdmap_create@PLT
	xorl	%edi, %edi
	call	epoll_create1@PLT
	movl	%eax, (%rbx)
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE16:
	.size	asio_create, .-asio_create
	.p2align 4
	.globl	asio_add
	.type	asio_add, @function
asio_add:
.LFB17:
	.cfi_startproc
	endbr64
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movl	%edx, %r9d
	movq	%rcx, %rdx
	movq	%r8, %rcx
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	addq	$8, %rdi
	movl	%esi, %r12d
	subq	$40, %rsp
	.cfi_def_cfa_offset 64
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	movl	%esi, 16(%rsp)
	movl	%r9d, 12(%rsp)
	call	fdmap_add@PLT
	movl	(%rbx), %edi
	leaq	12(%rsp), %rcx
	movl	%r12d, %edx
	movl	$1, %esi
	call	epoll_ctl@PLT
	movq	24(%rsp), %rbx
	xorq	%fs:40, %rbx
	jne	.L7
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L7:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE17:
	.size	asio_add, .-asio_add
	.p2align 4
	.globl	asio_modify_events
	.type	asio_modify_events, @function
asio_modify_events:
.LFB18:
	.cfi_startproc
	endbr64
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movl	(%rdi), %edi
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	leaq	12(%rsp), %rcx
	movl	%esi, 16(%rsp)
	movl	%edx, 12(%rsp)
	movl	%esi, %edx
	movl	$3, %esi
	call	epoll_ctl@PLT
	movq	24(%rsp), %rcx
	xorq	%fs:40, %rcx
	jne	.L11
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L11:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE18:
	.size	asio_modify_events, .-asio_modify_events
	.p2align 4
	.globl	asio_modify_continuation
	.type	asio_modify_continuation, @function
asio_modify_continuation:
.LFB19:
	.cfi_startproc
	endbr64
	addq	$8, %rdi
	jmp	fdmap_set@PLT
	.cfi_endproc
.LFE19:
	.size	asio_modify_continuation, .-asio_modify_continuation
	.p2align 4
	.globl	asio_remove
	.type	asio_remove, @function
asio_remove:
.LFB20:
	.cfi_startproc
	endbr64
	movl	(%rdi), %edi
	movl	%esi, %edx
	xorl	%ecx, %ecx
	movl	$2, %esi
	jmp	epoll_ctl@PLT
	.cfi_endproc
.LFE20:
	.size	asio_remove, .-asio_remove
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"src/asio.c"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC1:
	.string	"errno == EAGAIN || errno == EINTR"
	.text
	.p2align 4
	.globl	asio_run
	.type	asio_run, @function
asio_run:
.LFB21:
	.cfi_startproc
	endbr64
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	xorl	%r14d, %r14d
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	xorl	%ebp, %ebp
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movq	%rdi, %rbx
	leaq	8(%rbx), %r12
	subq	$40, %rsp
	.cfi_def_cfa_offset 96
	movl	%esi, 20(%rsp)
	call	__errno_location@PLT
	movq	%rax, 24(%rsp)
.L15:
	movl	16(%rbx), %r13d
	cmpl	%r14d, %r13d
	jg	.L23
.L16:
	movl	20(%rsp), %ecx
	movl	(%rbx), %edi
	movl	%r14d, %edx
	movq	%rbp, %rsi
	call	epoll_wait@PLT
	testl	%eax, %eax
	jns	.L24
	movq	24(%rsp), %rax
	movl	(%rax), %eax
	cmpl	$11, %eax
	je	.L15
	cmpl	$4, %eax
	je	.L15
	leaq	__PRETTY_FUNCTION__.2785(%rip), %rcx
	movl	$56, %edx
	leaq	.LC0(%rip), %rsi
	leaq	.LC1(%rip), %rdi
	call	__assert_fail@PLT
	.p2align 4,,10
	.p2align 3
.L24:
	je	.L15
	subl	$1, %eax
	movq	%rbp, %r15
	leaq	(%rax,%rax,2), %rax
	leaq	12(%rbp,%rax,4), %rax
	movq	%rax, 8(%rsp)
	.p2align 4,,10
	.p2align 3
.L20:
	movl	4(%r15), %r13d
	movq	%r12, %rdi
	addq	$12, %r15
	movl	%r13d, %esi
	call	fdmap_get@PLT
	movl	%r13d, %edi
	movq	%rdx, %rsi
	movl	-12(%r15), %edx
	call	*%rax
	cmpq	%r15, 8(%rsp)
	jne	.L20
	movl	16(%rbx), %r13d
	cmpl	%r14d, %r13d
	jle	.L16
.L23:
	movslq	%r13d, %rax
	movq	%rbp, %rdi
	movl	%r13d, %r14d
	leaq	(%rax,%rax,2), %rsi
	salq	$2, %rsi
	call	realloc@PLT
	movq	%rax, %rbp
	jmp	.L16
	.cfi_endproc
.LFE21:
	.size	asio_run, .-asio_run
	.section	.rodata
	.align 8
	.type	__PRETTY_FUNCTION__.2785, @object
	.size	__PRETTY_FUNCTION__.2785, 9
__PRETTY_FUNCTION__.2785:
	.string	"asio_run"
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
