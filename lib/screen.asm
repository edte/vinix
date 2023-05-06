global disp_str_raw 
global clear        ; 将 clear_screen 设置为全局可见符号


; TODO: 这里连续调用会出错，需要解决

;************************************************************8
; 函数： 在长模式下打印字符串 
; 参数: 
;       rdi: 行数
;       rsi: 列数
;       rdx, 字符串首地址
;************************************************************8
disp_str_raw:
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


;************************************************************8
; 函数： 清屏
; 参数: 
;************************************************************8


; 清屏函数定义
clear:
    xor esi, esi            ; ES = 0, 所有指针都将被转换为线性地址
    mov edi, 0xb8000        ; 显存基地址
    mov ah, 0x00            ; 属性为黑底白字，默认 ROSA

    ; 使用 REP STOSW 在显存中填充空格
    mov cx, 80*25/2         ; 总共字符数，除以二减少循环次数
    rep stosw

    ret                     ; 返回


