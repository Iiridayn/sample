	.file	"3.32.c"
	.text
	.p2align 4,,15
.globl absdiff2
	.type	absdiff2,@function
absdiff2:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %edx
	movl	12(%ebp), %eax
	cmpl	%eax, %edx
	jl	.L3
	subl	%edx, %eax
.L4:
	popl	%ebp
	ret
	.p2align 4,,7
.L3:
	subl	%eax, %edx
	movl	%edx, %eax
	jmp	.L4
.Lfe1:
	.size	absdiff2,.Lfe1-absdiff2
	.ident	"GCC: (GNU) 3.2.3 20030422 (Gentoo Linux 1.4 3.2.3-r1, propolice)"
