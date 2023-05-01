%include "load.inc"
org PHYSICAL_LOADER_ADDR
jmp _start


;===================================================================================================================================
; 16 位实模式代码
[SECTION .code16]
[BITS 16]
_start:
    ; [step 1] 显示开始 load 日志
    mov si,StartLoaderMessage			
    mov dh,1				;设置显示行
	mov dl,0				;设置显示列
    call DispStr16		


    ; [step 2] 显示开始 load 日志
    mov si,LoadKernelToMemMessage			
    mov dh,2				;设置显示行
	mov dl,0				;设置显示列
    call DispStr16		


    ; [step 3] 开启 A20,进入保护模式
    ; 关中断
	cli

    ; 加载 GDTR
	lgdt	[GdtPtr]

    ; 开启 A20
    in al, 92h
    or al, 00000010b
    out 92h, al

    ; [step 4] 读取 kernel 程序
    mov eax, 20 	                ; 第20个逻辑扇区开始
    mov ebx, PHYSICAL_KERNEL_ADDR	; 内存地址
    mov ecx, 10   	                ; 读取100个扇区
    call ReadLBA16

    ; [step 5] 把 cr0 的最低位置为 1，开启保护模式
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
      
    ; 进入保护模式
    jmp SelectorCode32:_start32
 

; ------------------------------------------------------------------------
; 显示字符串函数:DispStr16
; 参数:
; si = 字符串开始地址,
; dh = 第N行，0开始
; dl = 第N列，0开始
; ------------------------------------------------------------------------
DispStr16:
            mov cx,0			;BIOS中断参数：显示字符串长度
            mov bx,si
    .len:;获取字符串长度
            mov al,[bx]			;读取1个字节到al
            inc bx				;读取下个字节
            cmp al,0			;是否以0结束
            je .sDispStr
            inc	cx				;计数器
            jmp .len
    .sDispStr:;显示字符串
            mov bx,si
            mov bp,bx
            mov bx,ds
            mov es,bx			;BIOS中断参数：计算[ES:BP]为显示字符串开始地址

            mov ah,0x13			;BIOS中断参数：中断模式
            mov al,0x01			;BIOS中断参数：输出方式
            
            mov bh,0x0			;BIOS中断参数：指定分页为0
            mov bl,0x0f			;BIOS中断参数：显示属性，指定白色文字			
            int 0x10			;调用BIOS中断操作显卡。输出字符串
            ret

; ------------------------------------------------------------------------
; 读取磁盘:ReadLBA16
; 参数:
; eax=LBA扇区号
; ebx=将数据写入的内存地址
; ecx=读入的扇区数
; ------------------------------------------------------------------------	
ReadLBA16:
      mov esi,eax	  ;备份eax
      mov di,cx		  ;备份cx
;读写硬盘:
;第1步：设置要读取的扇区数
      mov dx,0x1f2
      mov al,cl
      out dx,al            ; 读取的扇区数

      mov eax,esi	       ; 恢复ax

;第2步：将LBA地址存入0x1f3 ~ 0x1f6

      ;LBA地址7~0位写入端口0x1f3
      mov dx,0x1f3                       
      out dx,al                          

      ;LBA地址15~8位写入端口0x1f4
      mov cl,8
      shr eax,cl
      mov dx,0x1f4
      out dx,al

      ;LBA地址23~16位写入端口0x1f5
      shr eax,cl
      mov dx,0x1f5
      out dx,al

      shr eax,cl
      and al,0x0f	   ;lba第24~27位
      or al,0xe0	   ; 设置7～4位为1110,表示lba模式
      mov dx,0x1f6
      out dx,al

;第3步：向0x1f7端口写入读命令，0x20 
      mov dx,0x1f7
      mov al,0x20                        
      out dx,al

;第4步：检测硬盘状态
  .not_ready:
      ;同一端口，写时表示写入命令字，读时表示读入硬盘状态
      nop
      in al,dx
      and al,0x88	        ;第4位为1表示硬盘控制器已准备好数据传输，第7位为1表示硬盘忙
      cmp al,0x08
      jnz .not_ready	    ;若未准备好，继续等。

;第5步：从0x1f0端口读数据
      mov ax, di
      mov dx, 256
      mul dx
      mov cx, ax	    ; di为要读取的扇区数，一个扇区有512字节，每次读入一个字，
			            ; 共需di*512/2次，所以di*256
      mov dx, 0x1f0
  .go_on_read:
      in ax,dx
      mov [bx],ax
      add bx,2		  
      loop .go_on_read
      ret


[SECTION .data16]
StartLoaderMessage:	            db	"Start Loader",0
LoadKernelToMemMessage:	        db	"load kernel to memery succ",0


;===================================================================================================================================
; 32 位 GDT
[SECTION .gdt32]
gdt_start:
; 第一个描述符必须是空描述符
gdt_null:
    dq 0
; 代码段描述符
gdt_code:
    dw 0xffff    ; Limit (bits 0-15)
    dw 0x0       ; Base (bits 0-15)
    db 0x0       ; Base (bits 16-23)
    db 10011010b ; Access Byte
    db 11001111b ; Flags , Limit (bits 16-19)
    db 0x0       ; Base (bits 24-31)
; 数据段描述符
gdt_data:
    dw 0xffff    ; Limit (bits 0-15)          
    dw 0x0       ; Base (bits 0-15)
    db 0x0       ; Base (bits 16-23)
    db 10010010b ; Access Byte
    db 11001111b ; Flags , Limit (bits 16-19)
    db 0x0       ; Base (bits 24-31)
; 栈段描述符
gdt_stack:
    dw 0x7c00    ; Limit (bits 0-15)           
    dw 0x0       ; Base (bits 0-15)
    db 0x0       ; Base (bits 16-23)
    db 10010010b ; Access Byte
    db 01000000b ; Flags , Limit (bits 16-19)
    db 0x0       ; Base (bits 24-31)
gdt_end:
 
GdtLen	equ	$ - gdt_start
GdtPtr	dw	GdtLen - 1
	    dd	gdt_start	

SelectorCode32	equ	gdt_code - gdt_start
SelectorData32	equ	gdt_data - gdt_start
SelectorStack32	equ	gdt_stack - gdt_start


[SECTION .code32]
ALIGN	32
[bits 32]
_start32:
    ; [step 1] 初始化寄存器
    mov ax, SelectorData32        ; 将数据段寄存器ds和附加段寄存器es置为0x10
    mov ds, ax         
    mov es, ax
    mov fs, ax          ; fs和gs寄存器由操作系统使用，这里统一设成0x10
    mov gs, ax
    mov ax, SelectorData32        ; 将栈段寄存器ss置为0x18
    mov ss, ax
    mov ebp, BASE_OF_STACK     ; 现在栈顶指向 0x7c00
    mov esp, ebp

    ; [step 2] 打印进入保护模式日志
    mov ebx, EnterProtectMessage             ; 被打印字符的地址
    mov ax, 3
    mov cx,0
    call DispStr32
 
    ; [step 3] 测试是否支持长模式 
	call	if_enter_long_mode
	test	eax,	eax
	jz	Pause

    ; [step 4] 初始化临时页表
	mov	dword	[0x90000],	0x91007
	mov	dword	[0x90800],	0x91007		
	mov	dword	[0x91000],	0x92007
	mov	dword	[0x92000],	0x000083
	mov	dword	[0x92008],	0x200083
	mov	dword	[0x92010],	0x400083
	mov	dword	[0x92018],	0x600083
	mov	dword	[0x92020],	0x800083
	mov	dword	[0x92028],	0xa00083


    ; [step 5] 加载 GDT
	db	66h
	lgdt	[Gdt64Ptr]


    ; [step 6] 开启 PAE 功能	
	mov	eax,	cr4
	bts	eax,	5
	mov	cr4,	eax

	
    ; [step 7] 加载页目录基地址
	mov	eax,	0x90000
	mov	cr3,	eax
	

    ; [step 8] 开启长模式/ IA-32e
	mov	ecx,	0C0000080h		;IA32_EFER
	rdmsr
	bts	eax,	8
	wrmsr
	

    ; [step 9] 开启分页机制
	mov	eax,	cr0
	bts	eax,	0
	bts	eax,	31
	mov	cr0,	eax

    mov ebx, PrepareEnterLongModeMessage            ; 被打印字符的地址
    mov ax, 4
    mov cx,0
    call DispStr32

    ; [step 10]  跳转进入临时长模式
	jmp	SelectorCode64:_start64

Pause:
    hlt

    jmp Pause


;************************************************************8
; 函数： 测试是否支持进入长模式
; 无参数
;************************************************************8
if_enter_long_mode:
	mov	eax,	0x80000000
	cpuid
	cmp	eax,	0x80000001
	setnb	al	
	jb	if_enter_long_mode_done
	mov	eax,	0x80000001
	cpuid
	bt	edx,	29
	setc	al
if_enter_long_mode_done:
	movzx	eax,	al
	ret

;************************************************************8
; 函数： 测试 A20 地址线是否打开
; 无参数
;************************************************************8
PrintA20:
    mov edi, 0x112345
    mov esi, 0x012345
    mov [esi], esi
    mov [edi], edi
    mov eax, [esi]
    cmp eax, edi
    je A20_DISABLE

    mov ebx, aA20_ENABLE
    mov ax,0
    mov cx,0
    call DispStr32
A20_DISABLE:
    mov ebx, aA20_DISABLE
    mov ax,0
    mov cx,0
    call DispStr32

    ret
 

;************************************************************8
; 函数： 在保护模式下打印字符串 
; 参数: 
;       ebx: 字符串首地址
;       eax: 行数
;       ecx, 列数
;************************************************************8
DispStr32:
    xor ecx, ecx            ; 清空 ecx
    
    mov edx,80
    mul edx
    add eax, ecx
    mov edx,2
    mul edx
    add eax,0xb8000
    mov edx, eax            ; 显存的地址

    mov ah, 0x0f            ; ah 为打印的颜色属性，0x0f为白字黑底
loop1_begin:
    mov al, [ebx]           ; al为被打印的字符
    cmp al, 0               ; 若al为0，结束打印
    je loop1_end
    mov [edx], ax           ; 向显存中写入字符及其颜色属性（2字节）
    inc ebx
    add edx, 2
    jmp loop1_begin
loop1_end:
    ret

[SECTION .data32]
aA20_ENABLE                 db      "A20 Enable", 0
aA20_DISABLE                db      "A20 Disable", 0
EnterProtectMessage         db      "enter protect model", 0 
PrepareEnterLongModeMessage db      "Prepare enter long mode message",0 
PauseMessage                db      "pause",0


;===================================================================================================================================
; 64 位 GDT
[SECTION .gdt64]
gdt64_start:
; 第一个描述符必须是空描述符
gdt64_null:
    dq	0x0000000000000000
; 代码段描述符
gdt64_code:
    dq	0x0020980000000000
; 数据段描述符
gdt64_data:
    dq	0x0000920000000000
gdt64_end:

Gdt64Len	equ	$ - gdt64_start
Gdt64Ptr	dw	Gdt64Len - 1
	        dd	gdt64_start	

SelectorCode64	equ	gdt64_code - gdt64_start
SelectorData64	equ	gdt64_data - gdt64_start

[SECTION .code64]
ALIGN	64
[bits 64]
_start64:
    ; [step 1] 初始化寄存器
	mov	ax, SelectorData64	
	mov	ds,	ax
	mov	es,	ax
	mov	fs,	ax
	mov	ss,	ax
	mov	esp,BASE_OF_STACK

    ; [step 2] 打印日志，进入 64 位
    mov ebx, EnterLongModeMessage
    mov eax, 5
    mov ecx, 0
    call DispStr64

    jmp PHYSICAL_KERNEL_ADDR

    ; jmp Pause64

Pause64:
    hlt
    jmp Pause64

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
EnterLongModeMessage         db      "enter long model", 0 
