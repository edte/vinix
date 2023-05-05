global  _start	
extern	kernel_init
extern	kernel_main

; 选择子
%define SELECTOR_KERNEL_CODE (1 * 8) 
%define SELECTOR_KERNEL_DATA (2 * 8) 
%define SELECTOR_USER_CODE (3 * 8)
%define SELECTOR_USER_DATA (4 * 8)
%define SELECTOR_TSS (5 * 8)
%define SELECTOR_VIDEO (6 * 8)
%define SELECTOR_KERNEL_STACK (7 * 8)

; 栈大小
%define KERNEL_STACK_SIZE 1<<15 

[section .text]	

ALIGN	64
[bits 64]
_start:
    mov ebx, EnterKernelMessage
    mov eax,8
    mov ecx,0
    call DispStr

    ; 初始化内核
    call kernel_init 
    
    ; 设置新的 gdt 寄存器
    mov ax, SELECTOR_KERNEL_DATA 
    mov ds, ax
    mov es, ax
    mov fs, ax

    mov ax,SELECTOR_VIDEO
    mov gs, ax

    mov ax,SELECTOR_KERNEL_STACK
    mov ss,ax

    ; 设置新的栈
    mov rsp,  _stack_start 

    ; 开始内核
    jmp  _entry

_entry:
    jmp kernel_main

_pause:
    hlt
    jmp _pause


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





[SECTION .data]
EnterKernelMessage db      "enter kernel head", 0 
tt db "testtesttest",0


[section .bss]
_stack_start: resq KERNEL_STACK_SIZE
