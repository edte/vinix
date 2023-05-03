global print


;************************************************************8
; 函数： 在长模式下打印字符串 
; 参数: 
;       rdi: 行数
;       rsi: 列数
;       rdx, 字符串首地址
;************************************************************8
print:
    mov rax,rdi
    mov rcx,rsi
    mov rbx,rdx
 
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


; ========================================================================
;                  void out_byte(u16 port, u8 value);
; ========================================================================
out_byte:
	mov	edx, [esp + 4]		; port
	mov	al, [esp + 4 + 4]	; value
	out	dx, al
	nop	; 一点延迟
	nop
	ret

; ========================================================================
;                  u8 in_byte(u16 port);
; ========================================================================
in_byte:
	mov	edx, [esp + 4]		; port
	xor	eax, eax
	in	al, dx
	nop	; 一点延迟
	nop
	ret
