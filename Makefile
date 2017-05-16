rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
C_SOURCES = $(call rwildcard,*,*.c)
HEADERS = $(wildcard */*.h)
OBJ := $(C_SOURCES:.c=.o)
CROSS = $(HOME)/opt/cross/
CC = $(CROSS)bin/i686-elf-gcc
# ARCH = $(shell uname -m)
ARCH = i386
CCFLAGS = -std=c99 -m32 -ffreestanding -nostdlib -static-libgcc -lgcc -I. -Ilibc/include -Ilibc/arch/$(ARCH)/
all: os-image

%.bin: %.asm
	nasm -i asm/ $^ -f bin -o $@

%.o: %.asm
	nasm $^ -f elf -o $@

asm/kernel.bin: asm/kernel_entry.o asm/idt.o $(OBJ)
	ln -fs $(shell $(CC) -print-file-name=libgcc.a)

	ld -o $@ \
	  -m elf_i386 \
	  --entry=main \
	  -Ttext=0x1000 \
	  -Tdata=0x5000 \
	  --oformat binary \
	  $^ \
	  libgcc.a

%.o: %.c $(C_SOURCES) libc/include/bits/alltypes.h
	$(CC) $(CCFLAGS) -c $< -o $@

os-image: asm/boot_sect.bin asm/kernel.bin asm/empty.bin
	cat $^ > os-image

debug:
	gdb --eval-command="target remote :1234"

clean:
	rm -fr asm/*.bin asm/*.o os-image
	rm -fr kernel/*.o drivers/*.o
	rm -fr libc/include/bits/alltypes.h
	rm -fr libgcc.a

tags:
	ctags -R

run: os-image
ifeq ("$(ARCH)", "x86_64")
	qemu-system-i386 -drive format=raw,file=$^
else
	qemu-system-x86_64 -drive format=raw,file=$^
endif

libc/include/bits/alltypes.h: libc/arch/$(ARCH)/bits/alltypes.h.in libc/include/alltypes.h.in tools/mkalltypes.sed
	mkdir -p libc/include/bits/
	sed -f tools/mkalltypes.sed libc/arch/$(ARCH)/bits/alltypes.h.in libc/include/alltypes.h.in > $@

