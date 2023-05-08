global disp_str_raw

global clear        ; 将 clear_screen 设置为全局可见符号
global disp_str
global disp_str_color

extern disp_pos

%define screen_row 25 
%define screen_col 160 
%define screen_base 0xb8000

;************************************************************8
; 函数： 清屏
; 参数: 
;************************************************************8

; 清屏函数定义
clear:
    xor rsi, rsi                ; ES = 0, 所有指针都将被转换为线性地址
    mov rdi, screen_base     ; 显存基地址
    mov ah, 0x00            ; 属性为黑底白字，默认 ROSA

    ; 使用 REP STOSW 在显存中填充空格
    mov cx, screen_row*screen_col/2         ; 总共字符数，除以二减少循环次数
    rep stosw

    ret                     ; 返回


; ========================================================================
;		   void disp_str(char * info);
; ========================================================================
; rsi：输入字符串的起始地址。
; rdi：字符要输出到屏幕的位置。
disp_str:
    ; 保存字符串基址 
    mov rsi, rdi;

    ; 计算显存基址 
    mov rax, [disp_pos]
    add rax, screen_base
    mov rdi, rax 

    ; ah 为打印的颜色属性，0x0f为白字黑底
    mov ah, 0x0f 

_disp_str_deal_char:
    ; 取一个字符
    lodsb 

    ; 判断是不是结束符 '\0'
    cmp al, 0               
    je _disp_str_end

    ; 判断回车
    cmp	al, 0Ah		          
    jnz _disp_str_print_char	

    ; 是回车的话，那么计算新的显存地址 
    push	rax               ; 把 AX 的值压入栈中

    sub rdi, screen_base

    mov	rax, rdi              ; 存储当前标准光标位置到 RAX
    mov	bl, screen_col        ; 将 BL 设定为列数
    div	bl                    ; 将 RDX:RAX 合并成一个双倍字（DDWORD），除以 BL 得到当前行数和列数
    and	eax, 0FFh             ; 取出行数
    inc	eax                   ; 行数加 1
    mov	bl, screen_col        ; 设列数
    mul	bl                    ; RAX = AX * BL，得到下一行的偏移量 （因每行占 160 个字），存储到 RDI 中
    mov	rdi, rax              ; RAX 中存在偏移量，将其复制到 RDI 中

    add rdi, screen_base

    pop	rax                   ; 把压入栈的字符回复到 AL 中
    jmp	_disp_str_deal_char   ; 处理下一个字符

_disp_str_print_char:
    ; 写一个字符入显存
    mov word [rdi], ax
    add rdi,2
    jmp _disp_str_deal_char

_disp_str_end:
    ; 设置新的光标
    sub	rdi, screen_base
    mov [disp_pos],rdi
    ret


; ========================================================================
;		   void disp_str_color(char * info, int color);
; ========================================================================
; rsi：输入字符串的起始地址。
; rdi：字符要输出到屏幕的位置。
disp_str_color:
    ; 设置颜色
    mov [tmp_color],sil

    ; 保存字符串基址 
    mov rsi, rdi;

    ; 计算显存基址 
    mov rax, [disp_pos]
    add rax, screen_base
    mov rdi, rax 

_disp_str_color_deal_char:
    ; 取一个字符
    lodsb 

    ; 判断是不是结束符 '\0'
    cmp al, 0               
    je _disp_str_color_end

    ; 判断回车
    cmp	al, 0Ah		          
    jnz _disp_str_color_print_char	

    ; 是回车的话，那么计算新的显存地址 
    push	rax               ; 把 AX 的值压入栈中

    sub rdi, screen_base

    mov	rax, rdi              ; 存储当前标准光标位置到 RAX
    mov	bl, screen_col        ; 将 BL 设定为列数
    div	bl                    ; 将 RDX:RAX 合并成一个双倍字（DDWORD），除以 BL 得到当前行数和列数
    and	eax, 0FFh             ; 取出行数
    inc	eax                   ; 行数加 1
    mov	bl, screen_col        ; 设列数
    mul	bl                    ; RAX = AX * BL，得到下一行的偏移量 （因每行占 160 个字），存储到 RDI 中
    mov	rdi, rax              ; RAX 中存在偏移量，将其复制到 RDI 中

    add rdi, screen_base

    pop	rax                   ; 把压入栈的字符回复到 AL 中
    jmp	_disp_str_color_deal_char   ; 处理下一个字符

_disp_str_color_print_char:
    ; 设置颜色
    mov ah,[tmp_color]
    ; 写一个字符入显存
    mov word [rdi], ax
    add rdi,2
    jmp _disp_str_color_deal_char

_disp_str_color_end:
    ; 设置新的光标
    sub	rdi, screen_base
    mov [disp_pos],rdi
    ret

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



[section .data]
tmp_color dd 0
