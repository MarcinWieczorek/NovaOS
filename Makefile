C_SOURCES = $(wildcard */*.c)
HEADERS = $(wildcard */*.h)
OBJ := $(C_SOURCES:.c=.o)
CC = gcc
ARCH = $(shell uname -m)
CCFLAGS = -std=c99 -m32 -ffreestanding -I .
all: os-image

%.bin: %.asm
	nasm -i asm/ $^ -f bin -o $@

%.o: %.asm
	nasm $^ -f elf -o $@

asm/kernel.bin: asm/kernel_entry.o $(OBJ)
	ld -o $@ \
	  -m elf_i386 \
	  --entry=main \
	  -Ttext=0x1000 \
	  -Tdata=0x3000 \
	  --oformat binary \
	  $^

%.o: %.c $(C_SOURCES)
	$(CC) $(CCFLAGS) -c $< -o $@

os-image: asm/boot_sect.bin asm/kernel.bin asm/empty.bin
	cat $^ > os-image

clean:
	rm -fr asm/*.bin asm/*.o os-image
	rm -fr kernel/*.o drivers/*.o

run: os-image
ifeq ("$(ARCH)", "x86_64")
	qemu-system-x86_64 $^
else
	qemu-system-i386 $^
endif
