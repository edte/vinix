org 0x10000

; global _start	
; global GDT_Table


[section .text]	
ALIGN	64
[bits 64]
_start:	
    mov ebx, EnterKernelMessage
    mov eax,10
    mov ecx,0
    call DispStr64

Pause:
    hlt
    jmp Pause


;************************************************************8
; 函数： 在长模式下打印字符串 
; 参数: 
;       rbx: 字符串首地址
;       rax: 行数
;       rcx, 列数
;************************************************************8
DispStr64:
    xor rcx, rcx            ; 清空 ecx
    
    mov rdx,80
    mul rdx
    add rax, rcx
    mov rdx,2
    mul rdx
    add rax,0xb8000
    mov rdx, rax            ; 显存的地址

    mov ah, 0x0f            ; ah 为打印的颜色属性，0x0f为白字黑底
loop1_begin64:
    mov al, [ebx]           ; al为被打印的字符
    cmp al, 0               ; 若al为0，结束打印
    je loop1_end64
    mov [edx], ax           ; 向显存中写入字符及其颜色属性（2字节）
    inc ebx
    add edx, 2
    jmp loop1_begin64
loop1_end64:
    ret


[SECTION .data]
EnterKernelMessage db      "enter kernel", 0 


; ;=================================================================
; ; ======= init page

; align 8
; org 0x1000

; __PML4E:
; dq 0x102007
; times 255 dq 0
; dq 0x102007
; times 255 dq 0

; org 0x2000

; __PDPTE:
; dq 0x103003
; times 511 dq 0

; org 0x3000

; __PDE:
; dq 0x000083

; dq 0x200083
; dq 0x400083
; dq 0x600083
; dq 0x800083
; dq 0xe0000083    ; 0xa00000
; dq 0xe0200083
; dq 0xe0400083
; dq 0xe0600083
; dq 0xe0800083    ; 0x1000000
; dq 0xe0a00083
; dq 0xe0c00083
; dq 0xe0e00083
; times 499 dq 0

; ; ======= GDT_Table

; [section .data]


; GDT_Table:
;     dq 0x0000000000000000            ; 0 NULL descriptor              00
;     dq 0x0020980000000000            ; 1 KERNEL Code 64-bit Segment   08
;     dq 0x0000920000000000            ; 2 KERNEL Data 64-bit Segment   10
;     dq 0x0020f80000000000            ; 3 USER Code 64-bit Segment     18
;     dq 0x0000f20000000000            ; 4 USER Data 64-bit Segment     20
;     dq 0x00cf9a000000ffff            ; 5 KERNEL Code 32-bit Segment   28
;     dq 0x00cf92000000ffff            ; 6 KERNEL Data 32-bit Segment   30
;     times 10 dq 0                    ; 8 ~ 9 TSS (jmp one segment <7>) in long-mode 128-bit 40
; GDT_END:

; GDT_POINTER: dw GDT_END - GDT_Table - 1
; dq GDT_Table
; GDT_LIMIT equ GDT_END - GDT_Table - 1
; GDT_BASE  equ GDT_Table

; ; ======= IDT_Table

; global IDT_Table

; IDT_Table:
; times 512 dq 0
; IDT_END:

; IDT_POINTER: dw IDT_END - IDT_Table -1
; dq IDT_Table
; IDT_LIMIT equ IDT_END - IDT_Table - 1
; IDT_BASE  equ IDT_Table

; ; ======= TSS64_Table

; global TSS64_Table

; TSS64_Table:
; times 13 dq 0
; TSS64_END:

; TSS64_POINTER: dw TSS64_END - TSS64_Table -1
; dq TSS64_Table
; TSS64_LIMIT equ TSS64_END - TSS64_Table - 1
; TSS64_BASE  equ TSS64_Table









; gdt64_start:
; ; 第一个描述符必须是空描述符
; gdt64_null:
;     dq	0x0000000000000000
; ; 代码段描述符
; gdt64_code:
;     dq	0x0020980000000000
; ; 数据段描述符
; gdt64_data:
;     dq	0x0000920000000000
; gdt64_end:

; Gdt64Len	equ	$ - gdt64_start
; Gdt64Ptr	dw	Gdt64Len - 1
; 	        dd	gdt64_start	

; SelectorCode64	equ	gdt64_code - gdt64_start
; SelectorData64	equ	gdt64_data - gdt64_start





; [SECTION .bss]
; StackSpace		resb	2 * 1024
; StackTop:		; 栈顶

