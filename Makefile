C_SOURCES = $(wildcard */*.c)
HEADERS = $(wildcard */*.h)
OBJ := $(C_SOURCES:.c=.o)
CC = gcc
ARCH = $(shell uname -m)
CCFLAGS = -std=c99 -m32 -ffreestanding -I .
all: os-image

test:
	echo $(C_SOURCES)
	echo $(OBJ)
	echo $(ARCH)

asm/boot_sect.bin: asm/boot_sect.asm
	nasm -i asm/ $^ -f bin -o $@
	nasm asm/empty.asm -f bin -o asm/empty.bin

asm/kernel_entry.o: asm/kernel_entry.asm
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
	cat asm/boot_sect.bin asm/kernel.bin asm/empty.bin > os-image

clean:
	rm -fr asm/*.bin asm/*.o os-image
	rm kernel/*.o drivers/*.o

run: os-image
ifeq ("$(ARCH)", "x86_64")
	qemu-system-x86_64 $^
else
	qemu-system-i386 $^
endif
