	.file	"3.31.c"
	.text
	.p2align 4,,15
.globl decode2
	.type	decode2,@function
decode2:
	pushl	%ebp
	movl	%esp, %ebp
	movl	16(%ebp), %eax
	movl	12(%ebp), %edx
	subl	%eax, %edx
	movl	8(%ebp), %eax
	popl	%ebp
	imull	%edx, %eax
	sall	$31, %edx
	sarl	$31, %edx
	xorl	%edx, %eax
	ret
.Lfe1:
	.size	decode2,.Lfe1-decode2
	.ident	"GCC: (GNU) 3.2.3 20030422 (Gentoo Linux 1.4 3.2.3-r1, propolice)"
