# Programs
ASM			:=	nasm
CC 			:= gcc

# Flags
ASMKFLAGS	:= -I ../include/  -f elf64
CC_FLAGS 	:=  -I ../include/ -mcmodel=large  -fno-stack-protector -fno-builtin -m64 -c

# var
TARGET 	:=    memory.o 

# rules

# Default starting position
all:  $(TARGET) 

memory.o: memory.c
	$(CC) $(CC_FLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -rf $(TARGET)
