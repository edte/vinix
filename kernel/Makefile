# Programs
ASM			:=	nasm
LD		:= ld
DASM		:= objdump

# Flags
ASM_FLAGS	:= -I include/
ASMKFLAGS	:= -I include/ -I include/sys/ -f elf64
LDFLAGS		= -Ttext $(PHYSICAL_KERNEL_ADDR)  -b elf64-x86-64 -z muldefs -T kernel.lds -e _start 

# var
PHYSICAL_KERNEL_ADDR = 0x100000
TARGET 	:= head.o  lib.o gdt.o idt.o main.o 
# DISK := /dev/sdc
DISK := os.img

# rules

# Default starting position
all:  kernel.bin 

kernel.bin: kernel.elf
	objcopy -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary kernel.elf kernel.bin

kernel.elf: $(TARGET) 
	$(LD) $(LDFLAGS) -o $@ $^

start.o: start.S
	gcc -E start.S > start.s
	as $(ASFLAGS) -o start.o start.s

head.o: head.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

main.o:	main.c
	gcc  -mcmodel=large  -fno-stack-protector -fno-builtin -m64 -c main.c

lib.o:lib.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

gdt.o:	gdt.c
	gcc  -mcmodel=large  -fno-stack-protector -fno-builtin -m64 -c gdt.c

irq.c: irq.o	
	gcc  -mcmodel=large  -fno-stack-protector -fno-builtin -m64 -c irq.c

.PHONY: clean

clean:
	rm -rf $(TARGET)
	rm -rf *.elf
	rm -rf bx_enh_dbg.ini
	rm -rf *.bin
	rm -rf *.o
