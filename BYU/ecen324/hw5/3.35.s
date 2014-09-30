	.file	"3.35.c"
	.text
	.p2align 4,,15
.globl var_prod_ele_ged
	.type	var_prod_ele_ged,@function
var_prod_ele_ged:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	movl	24(%ebp), %eax
	pushl	%esi
	movl	16(%ebp), %edx
	pushl	%ebx
	movl	12(%ebp), %ebx
	movl	%eax, %ecx
	imull	%eax, %edx
	xorl	%edi, %edi
	leal	0(,%edx,4), %esi
	movl	8(%ebp), %edx
	addl	%edx, %esi
	leal	0(,%eax,4), %edx
	addl	%edx, %ebx
	.p2align 4,,15
.L2:
	movl	(%ebx), %eax
	addl	%edx, %ebx
	imull	(%esi), %eax
	addl	$4, %esi
	addl	%eax, %edi
	decl	%ecx
	jne	.L2
	popl	%ebx
	movl	%edi, %eax
	popl	%esi
	popl	%edi
	popl	%ebp
	ret
.Lfe1:
	.size	var_prod_ele_ged,.Lfe1-var_prod_ele_ged
	.ident	"GCC: (GNU) 3.2.3 20030422 (Gentoo Linux 1.4 3.2.3-r1, propolice)"
