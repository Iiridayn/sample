	.file	"3.33.c"
	.text
.globl switch3
	.type	switch3,@function
switch3:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$4, %esp
	movl	$0, -4(%ebp)
	cmpl	$4, 16(%ebp)
	ja	.L8
	movl	16(%ebp), %eax
	sall	$2, %eax
	movl	.L9(%eax), %eax
	jmp	*%eax
	.section	.rodata
	.align 4
	.align 4
.L9:
	.long	.L3
	.long	.L4
	.long	.L5
	.long	.L6
	.long	.L7
	.text
.L3:
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -4(%ebp)
	movl	12(%ebp), %edx
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, (%edx)
	jmp	.L2
.L4:
	movl	8(%ebp), %ecx
	movl	8(%ebp), %edx
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	addl	(%edx), %eax
	movl	%eax, (%ecx)
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -4(%ebp)
	jmp	.L2
.L5:
	movl	8(%ebp), %eax
	movl	$15, (%eax)
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -4(%ebp)
	jmp	.L2
.L6:
	movl	8(%ebp), %eax
	movl	12(%ebp), %edx
	movl	(%edx), %edx
	movl	%edx, (%eax)
.L7:
	movl	$17, -4(%ebp)
	jmp	.L2
.L8:
	movl	$-1, -4(%ebp)
.L2:
	movl	-4(%ebp), %eax
	leave
	ret
.Lfe1:
	.size	switch3,.Lfe1-switch3
	.ident	"GCC: (GNU) 3.2.3 20030422 (Gentoo Linux 1.4 3.2.3-r1, propolice)"
