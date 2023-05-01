%include "load.inc"
org PHYSICAL_KERNEL_ADDR
global _start	

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


[SECTION .data64]
EnterKernelMessage db      "enter kernel", 0 
