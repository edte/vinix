; 最终物理内存分布：

;|-----------------|
;|	f000 ~ END     |
;|	   KERNEL	   |
;|-----------------|
;| ?               |
;|-----------------|
;|	9000 ~ e000    |
;|	 SVAG info     |
;|-----------------|
;|	8000 ~ 9000	   |
;|	  LOADER	   |
;|-----------------|
;|	7e00 ~ 8000	   |
;|	  mem info	   |
;|-----------------|
;|	7c00 ~ 7e00	   |
;|	 MBR (BOOT)	   |
;|-----------------|
;|	0000 ~ 7c00	   |
;|	 BIOS Code	   |
;|  (loader stack) |
;|-----------------|

; 栈基址，在 boot 和 loader 中都默认设 0x7c00,注意是向低位置即 0 开始延展
BASE_OF_STACK equ 0x7c00 

; Loader 程序内存的段地址和偏移地址
BaseOfLoader	equ	0x800
OffsetOfLoader	equ	0x00

; Loader 程序的物理地址
PHYSICAL_LOADER_ADDR equ 0x8000

; kernel 程序的物理地址
PHYSICAL_KERNEL_ADDR equ 0x10000

; 物理内存信息存放基址
PHYSICAL_MEMORY_STRUCT_BUFFER_ADDR  equ 0x7e00

; svga  物理信息基础地址
PHYSICAL_SVGA_ADDR equ 0x9000
