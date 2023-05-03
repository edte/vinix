# DISK := /dev/sdc
DISK := os.img

# rules

# Default starting position
all:  clean build flash run 

build:
	cd boot && make
	cd kernel && make

flash:
	sudo dd if=boot/boot.bin   of=$(DISK) bs=512 conv=notrunc count=1 
	sudo dd if=boot/loader.bin of=$(DISK) bs=512 conv=notrunc seek=1
	sudo dd if=kernel/kernel.bin 	   of=$(DISK) bs=512 conv=notrunc seek=20

run:
	bochs -q


.PHONY: clean

clean:
	cd boot && make clean
	cd kernel && make clean
