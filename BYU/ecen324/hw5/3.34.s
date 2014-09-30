	.file	"3.34.c"
	.text
.globl switch_prob
	.type	switch_prob,@function
switch_prob:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	leal	-50(%eax), %edx
	cmpl	$5, %edx
	ja	.L9
	jmp	*.L10(,%edx,4)
	.section	.rodata
	.align 4
	.align 4
.L10:
	.long	.L4
	.long	.L9
	.long	.L4
	.long	.L5
	.long	.L6
	.long	.L7
	.text
.L4:
	sall	$2, %eax
	jmp	.L2
.L5:
	sarl	$2, %eax
	jmp	.L2
.L6:
	leal	(%eax,%eax,2), %eax
.L7:
	imull	%eax, %eax
.L9:
	addl	$10, %eax
.L2:
	popl	%ebp
	ret
.Lfe1:
	.size	switch_prob,.Lfe1-switch_prob
	.ident	"GCC: (GNU) 3.2.3 20030422 (Gentoo Linux 1.4 3.2.3-r1, propolice)"
