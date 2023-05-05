# Programs
ASM			:=	nasm

# Flags
ASM_FLAGS	:= 	-I include/

# objects
TARGET 		:= boot.bin loader.bin

# Default starting position
all: $(TARGET) 

boot.bin: boot.asm  
	$(ASM) $(ASM_FLAGS) -o $@ $<

loader.bin: loader.asm 
	$(ASM) $(ASM_FLAGS) -o $@ $<


.PHONY: clean

clean:
	rm -rf $(TARGET)
