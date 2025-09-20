	.file	"main.c"
	.text
	.section .rdata,"dr"
.LC0:
	.ascii "r\0"
.LC1:
	.ascii "test.txt\0"
.LC2:
	.ascii "Error: File not found.\12\0"
	.section	.text.startup,"x"
	.p2align 4
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
.LFB21:
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	call	__main
	leaq	.LC0(%rip), %rdx
	leaq	.LC1(%rip), %rcx
	call	fopen
	movq	%rax, %rbx
	testq	%rax, %rax
	jne	.L2
	jmp	.L7
	.p2align 4,,10
	.p2align 3
.L4:
	movl	%eax, %ecx
	call	putchar
.L2:
	movq	%rbx, %rcx
	call	fgetc
	cmpl	$-1, %eax
	jne	.L4
	movq	%rbx, %rcx
	call	fclose
	xorl	%eax, %eax
.L1:
	addq	$32, %rsp
	popq	%rbx
	ret
.L7:
	leaq	.LC2(%rip), %rcx
	call	__mingw_printf
	movl	$1, %eax
	jmp	.L1
	.seh_endproc
	.def	__main;	.scl	2;	.type	32;	.endef
	.ident	"GCC: (Rev8, Built by MSYS2 project) 15.2.0"
	.def	fopen;	.scl	2;	.type	32;	.endef
	.def	putchar;	.scl	2;	.type	32;	.endef
	.def	fgetc;	.scl	2;	.type	32;	.endef
	.def	fclose;	.scl	2;	.type	32;	.endef
