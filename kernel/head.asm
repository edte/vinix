global _start	
extern	kernel_main

[section .text]	

ALIGN	64
[bits 64]
_start:	
    mov ebx, EnterKernelMessage
    mov eax,8
    mov ecx,0
    call DispStr

    ; 设置新的栈
    mov rsp,  _stack_start 

    jmp kernel_main


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


[section .bss]
_stack_start: resq 32768
