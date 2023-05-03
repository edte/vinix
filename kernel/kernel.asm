; org 0x100000

global _start	
global GDT_POINTER 
global GDT_Table;
global IDT_POINTER
global TSS_POINTER
global out_byte
global in_byte


global	divide_error
global	single_step_exception
global	nmi
global	breakpoint_exception
global	overflow
global	bounds_check
global	inval_opcode
global	copr_not_available
global	double_fault
global	copr_seg_overrun
global	inval_tss
global	segment_not_present
global	stack_exception
global	general_protection
global	page_fault
global	copr_error

extern	kernel_main
; extern exception_handler

[section .text]	

ALIGN	64
[bits 64]
_start:	
    mov ebx, EnterKernelMessage
    mov eax,8
    mov ecx,0
    call DispStr

    ; 加载新的 GDT
    lgdt [rel GDT_POINTER]

    ; 加载新的 IDT
    lidt [rel IDT_POINTER]

    ; 设置新的栈
    mov rsp,  _stack_start 
 
    ; 初始化 TSS
 ;    mov ax, SelectorTSS
	; ltr	ax


    ; jmp _pause

    ; TODO: 继续实现 level-4 页表，现在还是暂时用到 loader 中设置的页表
    ; 加载页目录基地址
    ; mov rax, 0x101000    ; 设置 cr3 为顶级页目录地址 
    ; mov cr3, rax
    ; 


    ; mov rax,0
    ; mov rax, __PML4E

    ; mov rbx,0
    ; mov rbx,__PDPTE

    ; mov rcx,0
    ; mov rcx,__PDE

    ; mov rdx,0
    ; mov rdx,0xfffff


    ; mov rax,0
    ; mov eax,[GDT_POINTER]
    ; jmp $
    
    ; 初始化内核代码段(cs),只能通过 jmp 来设置 
    jmp _entry

_entry:
    ; 初始化内核数据段
    mov ax, SelectorKernelData64 
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov ebx, aa 
    mov eax,10
    mov ecx,0
    call DispStr

    call init_8259A

    ; call setup_IDT
    ; call setup_TSS

    ; mov rax,0
    ; mov rax, [GDT_BASE]
    ; jmp _pause


    jmp kernel_main

    ; jmp _pause

_pause:
    hlt
    jmp _pause


;************************************************************8
; 函数： 初始化 i8259a 
; 参数: 
;************************************************************8
%define INT_M_CTL     0x20 ; I/O port for interrupt controller       <Master>
%define INT_M_CTLMASK 0x21 ; setting bits in this port disables ints <Master>
%define INT_S_CTL     0xA0 ; I/O port for second interrupt controller<Slave>
%define INT_S_CTLMASK 0xA1 ; setting bits in this port disables ints <Slave>

%define	INT_VECTOR_IRQ0			0x20
%define	INT_VECTOR_IRQ8			0x28

init_8259A:
    ; Master 8259, ICW1.
    mov al, 0x11
    out INT_M_CTL, al
    
    ; Slave 8259, ICW1.
    mov al, 0x11
    out INT_S_CTL, al
    
    ; Master 8259, ICW2. Set the interrupt entry address of the 'Master' to 0x20.
    mov al, byte INT_VECTOR_IRQ0
    out INT_M_CTLMASK, al
    
    ; Slave 8259, ICW2. Set the interrupt entry address of the 'Slave' to 0x28.
    mov al, byte INT_VECTOR_IRQ8
    out INT_S_CTLMASK, al
    
    ; Master 8259, ICW3. IR2 corresponds to 'Slave'.
    mov al, 0x4
    out INT_M_CTLMASK, al
    
    ; Slave 8259, ICW3. Corresponds to IR2 of 'Master'.
    mov al, 0x2
    out INT_S_CTLMASK, al
    
    ; Master 8259, ICW4.
    mov al, 0x1
    out INT_M_CTLMASK, al
    
    ; Slave 8259, ICW4.
    mov al, 0x1
    out INT_S_CTLMASK, al
    
    ; Master 8259, OCW1.
    mov al, 0xFF
    out INT_M_CTLMASK, al
    
    ; Slave 8259, OCW1.
    mov al, 0xFF
    out INT_S_CTLMASK, al
    
    ret



;************************************************************8
; 函数： 在长模式下打印字符串 
; 参数: 
;       rbx: 字符串首地址
;       rax: 行数
;       rcx, 列数
;************************************************************8
DispStr:
    xor rcx, rcx            ; 清空 ecx
    
    mov rdx,80
    mul rdx
    add rax, rcx
    mov rdx,2
    mul rdx
    add rax,0xb8000
    mov rdx, rax            ; 显存的地址

    mov ah, 0x0f            ; ah 为打印的颜色属性，0x0f为白字黑底
_loop1_begin64:
    mov al, [ebx]           ; al为被打印的字符
    cmp al, 0               ; 若al为0，结束打印
    je _loop1_end64
    mov [edx], ax           ; 向显存中写入字符及其颜色属性（2字节）
    inc ebx
    add edx, 2
    jmp _loop1_begin64
_loop1_end64:
    ret


;************************************************************8
; 函数： 在长模式下打印字符串 
; 参数: 
;       rbx: 字符串首地址
;       rax: 行数
;       rcx, 列数
;************************************************************8
; 中断和异常 -- 异常
divide_error:
	push	0xFFFFFFFF	; no err code
	push	0		; vector_no	= 0
	jmp	exception
single_step_exception:
	push	0xFFFFFFFF	; no err code
	push	1		; vector_no	= 1
	jmp	exception
nmi:
	push	0xFFFFFFFF	; no err code
	push	2		; vector_no	= 2
	jmp	exception
breakpoint_exception:
	push	0xFFFFFFFF	; no err code
	push	3		; vector_no	= 3
	jmp	exception
overflow:
	push	0xFFFFFFFF	; no err code
	push	4		; vector_no	= 4
	jmp	exception
bounds_check:
	push	0xFFFFFFFF	; no err code
	push	5		; vector_no	= 5
	jmp	exception
inval_opcode:
	push	0xFFFFFFFF	; no err code
	push	6		; vector_no	= 6
	jmp	exception
copr_not_available:
	push	0xFFFFFFFF	; no err code
	push	7		; vector_no	= 7
	jmp	exception
double_fault:
	push	8		; vector_no	= 8
	jmp	exception
copr_seg_overrun:
	push	0xFFFFFFFF	; no err code
	push	9		; vector_no	= 9
	jmp	exception
inval_tss:
	push	10		; vector_no	= A
	jmp	exception
segment_not_present:
	push	11		; vector_no	= B
	jmp	exception
stack_exception:
	push	12		; vector_no	= C
	jmp	exception
general_protection:
	push	13		; vector_no	= D
	jmp	exception
page_fault:
	push	14		; vector_no	= E
	jmp	exception
copr_error:
	push	0xFFFFFFFF	; no err code
	push	16		; vector_no	= 10h
	jmp	exception

exception: 
    mov ebx,error 
    mov eax,12
    mov ecx,0
    call DispStr


	; call	exception_handler
	; add	esp, 4*2	; 让栈顶指向 EIP，堆栈中从顶向下依次是：EIP、CS、EFLAGS
	; hlt


[SECTION .data]
EnterKernelMessage db      "enter kernel head", 0 
aa db "test",0
error    db "Unknown interrupt or fault at RIP", 0


;*********************************************************************************************************
; GDT 定义
GDT_Table:
GDT_NULL:
	dq	0x0000000000000000		;0	NULL descriptor		         	                    0x00
GDT_KERNEL_CODE64:
	dq	0x0020980000000000		;1	KERNEL	Code	64-bit	Segment	                    0x08
GDT_KERNEL_DATA64:
	dq	0x0000920000000000		;2	KERNEL	Data	64-bit	Segment	                    0x10
GDT_USER_CODE64:
	dq	0x0020f80000000000		;3	USER	Code	64-bit	Segment	                    0x18
GDT_USER_DATA64:
	dq	0x0000f20000000000		;4	USER	Data	64-bit	Segment	                    0x20
TSS:
	times 10 dq 0				;8 ~ 9	TSS (jmp one segment <7>) in long-mode 128-bit  0x40
GDT_END:

GDT_POINTER:
GDT_LIMIT:   dw  GDT_END - GDT_Table - 1
GDT_BASE:    dq GDT_Table

SelectorKernelCode64	equ	GDT_KERNEL_CODE64 - GDT_Table 
SelectorKernelData64	equ	GDT_KERNEL_DATA64- GDT_Table 
SelectorUserCode64	    equ	GDT_USER_CODE64 - GDT_Table 
SelectorUserData64	    equ	GDT_USER_DATA64- GDT_Table 
SelectorTSS             equ TSS - GDT_Table


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

TSS_Table:
	times  13 dq 0
TSS_END:

TSS_POINTER:
TSS_LIMIT:	dw	TSS_END - TSS_Table - 1
TSS_BASE:	    dq	TSS_Table


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

[section .page vstart=0]
    resb 0xE10

align 8

__PML4E:
    dq 0x102003 
    times 255 dq 0
    dq 0x102003 
    times 255 dq 0

__PDPTE:
    dq 0x103003 
    times 511 dq 0

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
[section .bss]

_stack_start: resq 32768
