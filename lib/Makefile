# Programs
ASM			:=	nasm
CC 			:= gcc

# Flags
ASM_FLAGS	:= -I ../include/  -f elf64
CC_FLAGS 	:= -mcmodel=large  -fno-stack-protector -fno-builtin -m64 -c

# var
TARGET 	:= klib.o screen.o 

# rules

# Default starting position
all:  $(TARGET)

klib.o: klib.c
	$(CC) $(CC_FLAGS) -o $@ $<

screen.o: screen.asm	
	$(ASM) $(ASM_FLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -rf $(TARGET)
