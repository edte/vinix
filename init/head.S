.section .text

.globl _start

_start:
    // 加载新的页表
    // TODO: 暂时汇编直接写，之后 c 写
	movq	$0x101000,	%rax
	movq	%rax,		%cr3

	movq	switch_seg(%rip),	%rax
	pushq	$0x08
	pushq	%rax
	lretq

switch_seg:
	.quad	entry64

entry64:
	movq	go_to_kernel(%rip),	%rax		/* movq address */
	pushq	$0x08
	pushq	%rax
	lretq

go_to_kernel:
	.quad	kernel_init

//=======	init page
.align 8

.org	0x1000

__PML4E:

	.quad	0x102007
	.fill	255,8,0
	.quad	0x102007
	.fill	255,8,0

.org	0x2000

__PDPTE:
	
	.quad	0x103003
	.fill	511,8,0

.org	0x3000

__PDE:

	.quad	0x000083	
	.quad	0x200083
	.quad	0x400083
	.quad	0x600083
	.quad	0x800083
	.quad	0xe0000083		/*0x a00000*/
	.quad	0xe0200083
	.quad	0xe0400083
	.quad	0xe0600083
	.quad	0xe0800083		/*0x1000000*/
	.quad	0xe0a00083
	.quad	0xe0c00083
	.quad	0xe0e00083
	.fill	499,8,0

