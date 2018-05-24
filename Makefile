rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
C_SOURCES = $(call rwildcard,*,*.c)
HEADERS = $(wildcard */*.h)
OBJ := $(C_SOURCES:.c=.o)
CROSS = $(HOME)/opt/cross/
CC = $(CROSS)bin/i686-elf-gcc
# QEMU=qemu-system-x86_64
QEMU = $(shell find /usr/bin -name "qemu-system-*")
# ARCH = $(shell uname -m)
ARCH = i386
CCFLAGS = -std=c99 -m32 -ffreestanding -nostdlib -static-libgcc -lgcc -I. -Ilibc/include -Ilibc/arch/$(ARCH)/ -fno-asynchronous-unwind-tables -fdiagnostics-color=auto
all: os-image

%.bin: %.asm
	@nasm -i asm/ $^ -f bin -o $@
	@echo "AS $<"

%.o: %.asm
	@nasm $^ -f elf -o $@
	@echo "AS $<"

asm/kernel.bin: asm/kernel_entry.o asm/idt.o $(OBJ)
	@ln -fs $(shell $(CC) -print-file-name=libgcc.a)
	@echo "LN libgcc.a"

	@ld -o $@ \
	  -zmuldefs \
	  -m elf_i386 \
	  --entry=main \
	  -Ttext=0x1000 \
	  -Tdata=0x6100 \
	  --oformat binary \
	  $^ \
	  libgcc.a
	@echo "LD kernel.bin"

%.o: %.c $(C_SOURCES) libc/include/bits/alltypes.h libc/include/bits/syscall.h
	@$(CC) $(CCFLAGS) -c $< -o $@
	@echo "CC $<"

os-image: asm/boot_sect.bin asm/kernel.bin asm/empty.bin
	@cat $^ > os-image

debug:
	gdb --eval-command="target remote :1234"

clean:
	rm -fr asm/*.bin asm/*.o os-image kernel/*.o drivers/*.o libc/include/bits/*.h libgcc.a

tags:
	ctags -R

run: os-image
	$(QEMU) -drive format=raw,file=$^

libc/include/bits/syscall.h: libc/arch/$(ARCH)/bits/syscall.h.in
	@cp $< $@
	@sed -ine 's|__NR_|SYS_|p' $@
	@echo "SED $<"

libc/include/bits/alltypes.h: libc/arch/$(ARCH)/bits/alltypes.h.in libc/include/alltypes.h.in tools/mkalltypes.sed
	@mkdir -p libc/include/bits/
	@sed -f tools/mkalltypes.sed libc/arch/$(ARCH)/bits/alltypes.h.in libc/include/alltypes.h.in > $@
	@echo "SED $<"

