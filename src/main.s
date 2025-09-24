	.file	"main.c"
	.text
	.section .rdata,"dr"
	.align 8
.LC0:
	.ascii "Fatal error: no arguments provided.\12\0"
.LC1:
	.ascii "--docs\0"
.LC2:
	.ascii "--num-of-regs\0"
	.align 8
.LC3:
	.ascii "Error: --num-of-regs expects a number\12\0"
.LC4:
	.ascii "Unknown argument: %s\12\0"
	.align 8
.LC5:
	.ascii "Fatal error: missing one required argument 'file_name' (required when not using '--docs').\0"
.LC6:
	.ascii "r\0"
.LC7:
	.ascii "Error: File '%s' not found.\12\0"
	.section	.text.startup,"x"
	.p2align 4
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
	pushq	%r15
	.seh_pushreg	%r15
	pushq	%r14
	.seh_pushreg	%r14
	pushq	%r13
	.seh_pushreg	%r13
	pushq	%r12
	.seh_pushreg	%r12
	pushq	%rbp
	.seh_pushreg	%rbp
	pushq	%rdi
	.seh_pushreg	%rdi
	pushq	%rsi
	.seh_pushreg	%rsi
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$104, %rsp
	.seh_stackalloc	104
	.seh_endprologue
	movl	%ecx, %ebp
	movq	%rdx, %r12
	call	__main
	cmpl	$1, %ebp
	jle	.L37
	movl	$1, %ebx
	xorl	%r14d, %r14d
	movl	$100, %r13d
	xorl	%r15d, %r15d
	jmp	.L2
	.p2align 4,,10
	.p2align 3
.L39:
	leaq	.LC2(%rip), %rdx
	movq	%rsi, %rcx
	call	strcmp
	testl	%eax, %eax
	jne	.L5
	addl	$1, %ebx
	cmpl	%ebp, %ebx
	jge	.L6
	movq	8(%r12,%rdi,8), %rcx
	call	atoi
	movl	%eax, %r13d
.L4:
	addl	$1, %ebx
	cmpl	%ebx, %ebp
	jle	.L38
.L2:
	movslq	%ebx, %rdi
	leaq	.LC1(%rip), %rdx
	movq	(%r12,%rdi,8), %rsi
	movq	%rsi, %rcx
	call	strcmp
	testl	%eax, %eax
	jne	.L39
	addl	$1, %ebx
	movl	$1, %r14d
	cmpl	%ebx, %ebp
	jg	.L2
.L38:
	testb	%r14b, %r14b
	jne	.L40
	testq	%r15, %r15
	je	.L41
	leaq	.LC6(%rip), %rdx
	movq	%r15, %rcx
	call	fopen
	movq	%rax, %rbx
	testq	%rax, %rax
	je	.L42
	leaq	64(%rsp), %rsi
	movl	$50, %edx
	movq	%rsi, %rcx
	call	make_vector_char
.L12:
	movq	%rbx, %rcx
	call	fgetc
	cmpl	$62, %eax
	jg	.L17
	cmpl	$32, %eax
	jg	.L18
	addl	$1, %eax
	jne	.L12
	movq	%rbx, %rcx
	call	fclose
	movq	64(%rsp), %rdx
	movq	72(%rsp), %rax
	cmpb	$59, -1(%rdx,%rax)
	jne	.L43
.L23:
	movdqu	64(%rsp), %xmm0
	movq	80(%rsp), %rax
	leaq	32(%rsp), %rcx
	movl	%r13d, %edx
	movq	%rax, 48(%rsp)
	movups	%xmm0, 32(%rsp)
	call	interprete
	movq	64(%rsp), %rcx
	call	free
.L9:
	xorl	%eax, %eax
.L1:
	addq	$104, %rsp
	popq	%rbx
	popq	%rsi
	popq	%rdi
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
	.p2align 4,,10
	.p2align 3
.L5:
	cmpb	$45, (%rsi)
	je	.L44
	movq	%rsi, %r15
	jmp	.L4
.L18:
	leal	-33(%rax), %edx
	movl	$704689157, %ecx
	btq	%rdx, %rcx
	jc	.L15
	cmpl	$62, %eax
	je	.L12
	cmpl	$39, %eax
	jle	.L12
	leal	-40(%rax), %edx
	movl	$2883075, %ecx
	btq	%rdx, %rcx
	jnc	.L12
.L15:
	movl	%eax, %edx
	movq	%rsi, %rcx
	call	vector_char_push
	jmp	.L12
.L43:
	movl	$59, %edx
	movq	%rsi, %rcx
	call	vector_char_push
	jmp	.L23
.L40:
	call	print_docs
	jmp	.L9
.L17:
	leal	-91(%rax), %edx
	cmpl	$35, %edx
	ja	.L20
	movabsq	$34359738373, %rcx
	btq	%rdx, %rcx
	jc	.L15
.L20:
	leal	-64(%rax), %edx
	cmpl	$61, %edx
	ja	.L12
	movabsq	$4109536930956312577, %rcx
	btq	%rdx, %rcx
	jc	.L15
	jmp	.L12
.L37:
	movl	$2, %ecx
	call	*__imp___acrt_iob_func(%rip)
	movl	$36, %r8d
	movl	$1, %edx
	leaq	.LC0(%rip), %rcx
	movq	%rax, %r9
	call	fwrite
.L3:
	movl	$1, %eax
	jmp	.L1
.L44:
	movl	$2, %ecx
	call	*__imp___acrt_iob_func(%rip)
	movq	%rsi, %r8
	leaq	.LC4(%rip), %rdx
	movq	%rax, %rcx
	call	__mingw_fprintf
	jmp	.L3
.L6:
	movl	$2, %ecx
	call	*__imp___acrt_iob_func(%rip)
	movl	$38, %r8d
	movl	$1, %edx
	leaq	.LC3(%rip), %rcx
	movq	%rax, %r9
	call	fwrite
	jmp	.L3
.L41:
	movl	$2, %ecx
	call	*__imp___acrt_iob_func(%rip)
	movl	$90, %r8d
	movl	$1, %edx
	leaq	.LC5(%rip), %rcx
	movq	%rax, %r9
	call	fwrite
	jmp	.L3
.L42:
	movq	%r15, %rdx
	leaq	.LC7(%rip), %rcx
	call	__mingw_printf
	jmp	.L3
	.seh_endproc
	.def	__main;	.scl	2;	.type	32;	.endef
	.ident	"GCC: (Rev8, Built by MSYS2 project) 15.2.0"
	.def	strcmp;	.scl	2;	.type	32;	.endef
	.def	atoi;	.scl	2;	.type	32;	.endef
	.def	fopen;	.scl	2;	.type	32;	.endef
	.def	make_vector_char;	.scl	2;	.type	32;	.endef
	.def	fgetc;	.scl	2;	.type	32;	.endef
	.def	fclose;	.scl	2;	.type	32;	.endef
	.def	interprete;	.scl	2;	.type	32;	.endef
	.def	free;	.scl	2;	.type	32;	.endef
	.def	vector_char_push;	.scl	2;	.type	32;	.endef
	.def	print_docs;	.scl	2;	.type	32;	.endef
	.def	fwrite;	.scl	2;	.type	32;	.endef
