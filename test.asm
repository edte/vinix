[section .text]


global _stack_start
global GDT_Table
global IDT_Table 
global TSS64_Table

global _start
_start:

    ; Set up data segment registers
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ss, ax

    ; Set up stack pointer
    mov esp, 0x7E00

    ; Load GDT
    lgdt [GDT_POINTER]

    ; Load IDT
    lidt [IDT_POINTER]

    ; Set up additional segment registers
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Set up stack pointer
    mov rsp, [ _stack_start ]

    ; Set up page tables
    mov rax, 0x101000
    mov cr3, rax

    ; Jump to kernel
    mov rax, [ switch_seg ]
    push 0x08
    push rax
    lretq

; Define switch_seg label
switch_seg:
    dq entry64

; Define entry64 label
entry64:
    mov rax, 0x10
    mov ds, rax
    mov es, rax
    mov gs, rax
    mov ss, rax

    ; Set up stack pointer
    mov rsp, [ _stack_start ]

    ; Read MSR
    mov rax, 0x1b
    rdmsr

    ; Check if bit 8 is set
    bt rax, 8
    jnc start_smp

; Define setup_IDT label
setup_IDT:
    lea rdx, [ ignore_int ]
    mov rax, 0x08
    shl rax, 16
    mov rcx, 0x8E00
    shl rcx, 32
    add rax, rcx
    mov ecx, edx
    shr ecx, 16
    shl rcx, 48
    add rax, rcx
    shr rdx, 32
    lea rdi, [ IDT_Table ]
    mov rcx, 256
rp_sidt:
    mov [rdi], rax
    mov [rdi+8], rdx
    add rdi, 0x10
    dec rcx
    jne rp_sidt

; Define setup_TSS64 label
setup_TSS64:
    lea rdx, [ init_tss ]
    xor rax, rax
    xor rcx, rcx
    mov rax, 0x89
    shl rax, 40
    mov ecx, edx
    shr ecx, 24
    shl rcx, 56
    add rax, rcx
    xor rcx, rcx
    mov ecx, edx
    and ecx, 0xffffff
    shl rcx, 16
    add rax, rcx
    add rax, 103
    lea rdi, [ GDT_Table ]
    mov [rdi+80], rax
    shr rdx, 32
    mov [rdi+88], rdx

    ; Jump to kernel
    mov rax, [ go_to_kernel ]
    push 0x08
    push rax
    lretq

; Define go_to_kernel label
go_to_kernel:
    dq Start_Kernel

; Define start_smp label
start_smp:
    ; Jump to SMP kernel
    mov rax, [ go_to_smp_kernel ]
    push 0x08
    push rax
    lretq

; Define go_to_smp_kernel label
go_to_smp_kernel:
    dq Start_SMP

; Define ignore_int label
ignore_int:
    cld
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rdi
    push rsi

    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov rax, es
    push rax
    mov rax, ds
    push rax

    mov rax, 0x10
    mov ds, rax
    mov es, rax

    lea rax, [ int_msg ]
    push rax
    mov rdx, rax
    mov rsi, 0x00000000
    mov rdi, 0x00ff0000
    mov rax, 0
    call color_printk
    add rsp, 0x8

Loop:
    jmp Loop

    pop rax
    mov ds, rax
    pop rax
    mov es, rax

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8

    pop rsi
    pop rdi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax
    iretq

; Define int_msg label
int_msg:
    db "Unknown interrupt or fault at RIP", 0

[section .data]


;*********************************************************************************************************
; GDT 定义
GDT_Table:
GDT_NULL:
	dq	0x0000000000000000			/*0	NULL descriptor		       	00*/
GDT_KERNEL_CODE64:
	dq	0x0020980000000000			/*1	KERNEL	Code	64-bit	Segment	08*/
GDT_KERNEL_DATA64:
	dq	0x0000920000000000			/*2	KERNEL	Data	64-bit	Segment	10*/
GDT_USER_CODE64:
	dq	0x0020f80000000000			/*3	USER	Code	64-bit	Segment	18*/
GDT_USER_DATA64:
	dq	0x0000f20000000000			/*4	USER	Data	64-bit	Segment	20*/
TSS:
	times 10 dq 0					/*8 ~ 9	TSS (jmp one segment <7>) in long-mode 128-bit 40*/
GDT_END:

GDT_POINTER:
GDT_LIMIT:   dw  GDT_END - GDT_Table - 1
GDT_BASE:    dq GDT_Table

SelectorKernelCode64	equ	GDT_KERNEL_CODE64 - GDT_Table 
SelectorKernelData64	equ	GDT_KERNEL_DATA64- GDT_Table 
SelectorUserCode64	equ	GDT_USER_CODE64 - GDT_Table 
SelectorUserData64	equ	GDT_USER_DATA64- GDT_Table 


;*********************************************************************************************************


; IDT 定义
IDT_Table:
    times 512 dq 0
IDT_END:

IDT_POINTER:
IDU_LIMIT:    dw IDT_END - IDT_Table - 1
IDT_BASE:     dq IDT_Table







;*******************************************************************************************************8
; TSS 表定义

TSS64_Table:
	times  13 dq 0
TSS64_END:

TSS64_POINTER:
TSS64_LIMIT:	dw	TSS64_END - TSS64_Table - 1
TSS64_BASE:	    dq	TSS64_Table


;**********************************************************************************************************
; 页表定义

; 参考 https://blog.csdn.net/huang987246510/article/details/104616857

; 启动 4-level 分页模式

; 缩写缩名
; PGD: 一级页表 (Page Global Directory)
; PUD: 二级页表 (Page Upper Directory)
; PMD: 三级页表 (Page Middle Directory)
; PT:  四级页表 (Page Table)


; PDE: 页目录项 （Page Directory entry)
; PTE: 页表项  （page table entry）


; PAE: 物理地址扩展（Physical Adress Extend）
; PDPTE（page-directory-pointer table entry）寄存器


align 8

__PML4E:
    dq 0x102003
    times 255 dq 0
    dq 0x102003
    times 255 dq 0

align 8 

__PDPTE:
    dq 0x103003
    times 511 dq 0

align 8 

__PDE:
    dq 0x000083
    dq 0x200083
    dq 0x400083
    dq 0x600083
    dq 0x800083
    dq 0xa00083
    dq 0xc00083
    dq 0xe00083
    dq 0x1000083
    dq 0x1200083
    dq 0x1400083
    dq 0x1600083
    dq 0x1800083
    dq 0x1a00083
    dq 0x1c00083
    dq 0x1e00083
    dq 0x2000083
    dq 0x2200083
    dq 0x2400083
    dq 0x2600083
    dq 0x2800083



;*******************************************************************************************************8
; 栈空间
section .bss

_stack_start: resq 32768
