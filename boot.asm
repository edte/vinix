[bits 16]
org     0x7c00 				;指明程序的偏移的基地址
global _start

%include "load.inc"

_start:
    ; [step 1] 初始化寄存器
    mov ax,cs				
    mov ss,ax
    mov es,ax
    mov ss,ax
    mov fs,ax
	mov	sp,BASE_OF_STACK
    
    ; [step 2] clear screen
	mov	ax,	0600h
	mov	bx,	0700h
	mov	cx,	0
	mov	dx,	0184fh
	int	10h

    ; [step 3]	set focus
	mov	ax,	0200h
	mov	bx,	0000h
	mov	dx,	0000h
	int	10h

    ; [step 4] 显示 boot
	mov	ax,	1301h
	mov	bx,	000fh
	mov	dx,	0000h
	mov	cx,	10
	mov	bp,	StartBootMessage
	int	10h

    ; [step 5] 读取 loader 程序
    mov eax, 0x1 	                ; 第1个逻辑扇区开始
    mov ebx, PHYSICAL_LOADER_ADDR   ; 内存地址
    mov ecx, 10   	                ; 读取10个扇区
    call ReadLBA16

    ; [step 6] 跳转到 loader 程序
    jmp BaseOfLoader:OffsetOfLoader

; ------------------------------------------------------------------------
; 读取磁盘:ReadLBA16
; 参数:
; eax=LBA扇区号(8b)
; ebx=将数据写入的内存地址
; ecx=读入的扇区数
; ------------------------------------------------------------------------	
ReadLBA16:
      mov esi,eax	  ;备份eax
      mov di,cx		  ;备份cx

; [step 1] 设置要读取的扇区数
      mov dx,0x1f2
      mov al,cl
      out dx,al            ; 读取的扇区数

      mov eax,esi	       ; 恢复ax

; [step 2] 将LBA地址存入0x1f3 ~ 0x1f6
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
      and al,0x0f	   ; lba第24~27位
      or al,0xe0	   ; 设置7～4位为1110,表示lba模式
      mov dx,0x1f6
      out dx,al

; [step 3] 向0x1f7端口写入读命令，0x20 
      mov dx,0x1f7
      mov al,0x20                        
      out dx,al

; [step 4] 检测硬盘状态
  .not_ready:
      ;同一端口，写时表示写入命令字，读时表示读入硬盘状态
      nop
      in al,dx
      and al,0x88	        ;第4位为1表示硬盘控制器已准备好数据传输，第7位为1表示硬盘忙
      cmp al,0x08
      jnz .not_ready	    ;若未准备好，继续等。

; [step 5] 从0x1f0端口读数据
      mov ax, di
      mov dx, 256
      mul dx
      mov cx, ax	    ; di为要读取的扇区数，一个扇区有512字节，每次读入一个字,共需 di*512/2 次，所以 di*256
      mov dx, 0x1f0

  .go_on_read:
      in ax,dx
      mov [bx],ax
      add bx,2		  
      loop .go_on_read
      ret


; ------------------------------------------------------------------------
;准备显示字符串
StartBootMessage: db "start boot ..",0

fill_sector:
    resb    510-($-$$)       	;处理当前行$至结束(1FE)的填充
    db      0x55, 0xaa
