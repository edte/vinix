# Programs
ASM			:=	nasm
CC 			:= gcc

# Flags
ASM_FLAGS	:= -I ../include/  -f elf64
CC_FLAGS 	:= -static -mcmodel=large  -fno-stack-protector -fno-builtin -m64 -c

# var
TARGET 	:= head.o init.o 

# rules


# Default starting position
all:  $(TARGET)

t:
	gcc -E  head.S > head.s
	as --64 -o head.o head.s
	gcc -static  -mcmodel=large -fno-builtin -m64 -c init.c  -fno-stack-protector
	gcc -static  -mcmodel=large -fno-builtin -m64 -c printk.c -fno-stack-protector

head.o: head.S
	gcc -E  head.S > head.s
	as --64 -o head.o head.s

init.o:	init.c
	$(CC) $(CC_FLAGS) -o $@ $<






	# ld -b elf64-x86-64 -z muldefs -o system head.o init.o printk.o -T Kernel.lds 



	# objcopy -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary system kernel.bin
	# cp kernel.bin ..


.PHONY: clean

clean:
	rm -rf $(TARGET)
	rm -rf head.s
