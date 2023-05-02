# Programs
ASM			:=	nasm
LD		:= ld
DASM		:= objdump

# Flags
ASM_FLAGS	:= 	-I include/
ASMKFLAGS	= -I include/ -I include/sys/ -f elf64

LDFLAGS		= -Ttext $(PHYSICAL_KERNEL_ADDR)  -b elf64-x86-64 -z muldefs 
DASMFLAGS	:= -D

# objects
TARGET 		:= kernel.o


DASMOUTPUT	:= kernel.bin.asm

# DISK := /dev/sdc
DISK := os.img

PHYSICAL_KERNEL_ADDR = 0x10000


# rules

# Default starting position
all:  kernel.bin  flash 


kernel.bin: kernel.asm	
	$(ASM) $(ASM_FLAGS) -o $@ $<


# kernel.bin: kernel.elf
# 	objcopy -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary kernel.elf kernel.bin

# kernel.elf: $(TARGET) 
# 	$(LD) $(LDFLAGS) -o $@ $^

# kernel.o: kernel.asm
# 	$(ASM) $(ASMKFLAGS) -o $@ $<


flash:
	sudo dd if=boot/boot.bin   of=$(DISK) bs=512 conv=notrunc count=1 
	sudo dd if=boot/loader.bin of=$(DISK) bs=512 conv=notrunc seek=1
	sudo dd if=kernel.bin of=$(DISK) bs=512 conv=notrunc seek=20


.PHONY: clean

clean:
	rm -rf $(TARGET)
	rm -rf *.elf
	rm -rf bx_enh_dbg.ini
	rm -rf *.bin
	rm -rf *.o
