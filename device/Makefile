# Programs
ASM			:=	nasm
CC 			:= gcc

# Flags
ASMKFLAGS	:= -I ../include/  -f elf64
CC_FLAGS 	:=  -I ../include/ -mcmodel=large -static -fno-stack-protector -fno-builtin -m64 -c

# var
TARGET 		:=       time.o keyboard.o printk.o 

# rules

# Default starting position
all:  $(TARGET) 

kprint.o: kprint.c
	$(CC) $(CC_FLAGS) -o $@ $<

time.o: time.c
	$(CC) $(CC_FLAGS) -o $@ $<

keyboard.o: keyboard.c
	$(CC) $(CC_FLAGS) -o $@ $<

printk.o: printk.c
	$(CC) $(CC_FLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -rf $(TARGET)
