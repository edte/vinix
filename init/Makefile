# Programs
ASM			:=	nasm
CC 			:= gcc

# Flags
ASM_FLAGS	:= -I ../include/  -f elf64
CC_FLAGS 	:= -mcmodel=large  -fno-stack-protector -fno-builtin -m64 -c

# var
TARGET 	:= head.o init.o 

# rules

# Default starting position
all:  $(TARGET)

head.o: head.asm
	$(ASM) $(ASM_FLAGS) -o $@ $<

init.o:	init.c
	$(CC) $(CC_FLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -rf $(TARGET)
