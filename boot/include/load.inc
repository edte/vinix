; 最终物理内存分布：

;|-----------------|
;|	100000 ~ END    |
;|	   KERNEL	   |
;|-----------------|
;|  10000~100000   |
;|   一些临时数据  |
;|-----------------|
;|	a000 ~ 10000   |
;|	  LOADER	   |
;|-----------------|
;|	8000 ~ a000    |
;|	  VBE info     |
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

; MBR 起始地址
PHYSICAL_MBR_ADDR equ 0x7c00

; 内存物理地址存放基址
PHYSICAL_MEMORY_STRUCT_BUFFER_ADDR  equ 0x7e00

; svga 物理信息基础地址
PHYSICAL_SVGA_ADDR equ 0x8000

; Loader 程序的物理地址
PHYSICAL_LOADER_ADDR equ 0xa000

; Loader 程序内存的段地址和偏移地址
BaseOfLoader	equ	0xa00
OffsetOfLoader	equ	0x00

; kernel 程序的物理地址
PHYSICAL_KERNEL_ADDR equ 0x100000
