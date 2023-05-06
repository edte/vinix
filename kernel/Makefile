# Programs
ASM			:=	nasm
CC 			:= gcc

# Flags
ASMKFLAGS	:= -I ../include/  -f elf64
CC_FLAGS 	:= -mcmodel=large  -fno-stack-protector -fno-builtin -m64 -c

# var
TARGET 	:=   gdt.o idt.o irq.o   kprint.o 

# rules

# Default starting position
all:  $(TARGET) 

gdt.o:	gdt.c
	$(CC) $(CC_FLAGS) -o $@ $<

idt.o: idt.c
	$(CC) $(CC_FLAGS) -o $@ $<

irq.o: irq.c	
	$(CC) $(CC_FLAGS) -o $@ $<

kprint.o: kprint.c
	$(CC) $(CC_FLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -rf $(TARGET)
