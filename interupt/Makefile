# Programs
ASM			:=	nasm
CC 			:= gcc

# Flags
ASMKFLAGS	:= -I ../include/  -f elf64
CC_FLAGS 	:=  -I ../include/ -mcmodel=large  -fno-stack-protector -fno-builtin -m64 -c

# var
TARGET 		:=  idt.o irq.o   irq_handler.o 

# rules

# Default starting position
all:  $(TARGET) 


idt.o: idt.c
	$(CC) $(CC_FLAGS) -o $@ $<

irq.o: irq.c	
	$(CC) $(CC_FLAGS) -o $@ $<

irq_handler.o: irq_handler.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

# irq_handler.o: irq_handler.S
# 	gcc -E irq_handler.S > irq_handler.s
# 	as --64  -o $@ $<

.PHONY: clean

clean:
	rm -rf $(TARGET)
