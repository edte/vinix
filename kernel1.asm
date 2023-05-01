%include "boot.inc"

[section .text]	; 代码在此


org KERNEL_BASE_ADDR

global _start	; 导出 _start

_start:	; 跳到这里来的时候，我们假设 gs 指向显存
    ;输出字符串
    mov si,kernel			; 将HelloMsg的地址放入 si
    mov dh,4				; 设置显示行
	mov dl,0				; 设置显示列
    call Print		        ; 调用函数

	jmp	$


; ------------------------------------------------------------------------
; 显示字符串函数:Print
; 参数:
; si = 字符串开始地址,
; dh = 第N行，0开始
; dl = 第N列，0开始
; ------------------------------------------------------------------------
Print:
    mov cx,0			;BIOS中断参数：显示字符串长度
    mov bx,si
.len:   ;获取字符串长度
    mov al,[bx]			;读取1个字节到al
    inc bx				;读取下个字节
    cmp al,0			;是否以0结束
    je .sprint
    inc	cx				;计数器
    jmp .len
.sprint:    ;显示字符串
    mov bx,si
    mov bp,bx
    mov bx,ds
    mov es,bx			;BIOS中断参数：计算[ES:BP]为显示字符串开始地址

    mov ah,0x13			;BIOS中断参数：中断模式
    mov al,0x01			;BIOS中断参数：输出方式
    mov bh,0x0			;BIOS中断参数：指定分页为0
    mov bl,0x1F			;BIOS中断参数：显示属性，指定白色文字			
    int 0x10			;调用BIOS中断操作显卡。输出字符串
    ret



kernel: db "kernel ...",0
