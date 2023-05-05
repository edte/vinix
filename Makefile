# Programs
LD		:= ld
OBJCOPY := objcopy

# Flags
LDFLAGS		= -Ttext $(PHYSICAL_KERNEL_ADDR)  -b elf64-x86-64 -z muldefs -T kernel.lds -e _start 
OBJCOPY_FLAGS  = -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary

# DISK := /dev/sdc
DISK := os.img
PHYSICAL_KERNEL_ADDR = 0x100000
OBJECTS = $(shell find . -type f -name '*.o')

# rules

# Default starting position
all: clean build pack flash run 

build:
	cd boot && make
	cd lib && make
	cd init && make
	cd kernel && make

pack: kernel.bin

kernel.bin: kernel.elf
	objcopy $(OBJCOPY_FLAGS) $< $@

# TODO: 这里 ld 时，-e 指定入口程序失败，只能手动把 head 作为第一个链接的文件来跑，之后想办法解决
kernel.elf: 
	echo $(OBJECTS)
	$(LD) $(LDFLAGS) ./init/head.o $(OBJECTS) -o $@

flash:
	sudo dd if=boot/boot.bin   of=$(DISK) bs=512 conv=notrunc count=1 
	sudo dd if=boot/loader.bin of=$(DISK) bs=512 conv=notrunc seek=1
	sudo dd if=kernel.bin 	   of=$(DISK) bs=512 conv=notrunc seek=20

run:
	bochs -q


.PHONY: clean  pack  build

clean:
	cd boot && make clean
	cd init && make clean
	cd lib && make clean
	cd kernel && make clean
	rm -rf kernel.bin
	rm -rf kernel.elf
	rm -rf ./bx_enh_dbg.ini
	rm -rf os.img.lock
