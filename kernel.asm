%include "load.inc"
org PHYSICAL_KERNEL_ADDR
global _start	

[section .text]	
_start:	
    push EnterKernelMessage
    jmp $

;*************************************************************
DispStr32:
    push ebp
    mov ebp, esp
    mov ebx, [ebp+8]        ; [ebp+8]即传入的字符串参数
    xor ecx, ecx
    mov ah, 0x0f            ; ah为打印的颜色属性，0x0f为白字黑底
    mov edx, 0xb8000+160*5    ; 显存的地址
loop1_begin:
    mov al, [ebx]           ; al为被打印的字符
    cmp al, 0               ; 若al为0，结束打印
    je loop1_end
    mov [edx], ax           ; 向显存中写入字符及其颜色属性（2字节）
    inc ebx
    add edx, 2
    jmp loop1_begin
loop1_end:
    mov esp, ebp
    pop ebp
    ret

EnterKernelMessage db      "enter kernel", 0 
