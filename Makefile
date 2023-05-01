# Programs
ASM			:=	nasm

# Flags
ASM_FLAGS	:= 	-I include/

# objects
TARGET 		:= boot.bin loader.bin kernel.bin


DASMFLAGS	= -D

DASMOUTPUT	= kernel.bin.asm

DASM		= objdump



# DISK := /dev/sdc
DISK := os.img

# Default starting position
all: $(TARGET)  flash clean

boot.bin: boot.asm  
	$(ASM) $(ASM_FLAGS) -o $@ $<

loader.bin: loader.asm 
	$(ASM) $(ASM_FLAGS) -o $@ $<

kernel.bin: kernel.asm 
	$(ASM) $(ASM_FLAGS) -o $@ $<

flash:
	sudo dd if=boot.bin   of=$(DISK) bs=512 conv=notrunc count=1 
	sudo dd if=loader.bin of=$(DISK) bs=512 conv=notrunc seek=1
	sudo dd if=kernel.bin of=$(DISK) bs=512 conv=notrunc seek=20



disasm :
	$(DASM) $(DASMFLAGS) boot.bin  > $(DASMOUTPUT)

.PHONY: clean

clean:
	rm -rf $(TARGET)
