rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
C_SOURCES = $(call rwildcard,*,*.c)
ASM_SOURCES = $(call rwildcard,libc/*,*.asm)
ASM_KERNEL_SOURCES = $(call rwildcard,kernel/*,*.asm)
OBJ := $(C_SOURCES:.c=.o)
ASM_OBJ := $(ASM_SOURCES:.asm=.o)
ASM_OBJ += $(ASM_KERNEL_SOURCES:.asm=.o)
CROSS = $(HOME)/opt/cross/
CC = $(CROSS)bin/i686-elf-gcc
# ARCH = $(shell uname -m)
ARCH = i386
QEMU = $(shell find /usr/bin -name "qemu-system-$(ARCH)")
CCFLAGS = -std=c99 -m32 -ffreestanding -nostdlib -static-libgcc -lgcc -I. -Ilibc/include -Iinclude  -Ilibc/arch/$(ARCH)/ -fno-asynchronous-unwind-tables -fdiagnostics-color=auto -ggdb
all: os-image

%.bin: %.asm
	@nasm -i asm/ $^ -f bin -o $@
	@echo "AS $<"

%.o: %.asm
	@nasm $^ -f elf -F dwarf -g -o $@
	@echo "AS $<"

asm/kernel.bin: asm/kernel_entry.o asm/idt.o $(OBJ) $(ASM_OBJ)
	@ln -fs $(shell $(CC) -print-file-name=libgcc.a)
	@echo "LN libgcc.a"

	@ld -o kernel.elf \
	  -zmuldefs \
	  -m elf_i386 \
	  --entry=main \
	  -Ttext=0x1000 \
	  $^ \
	  libgcc.a
	@echo "LD kernel.elf"

	@objcopy --only-keep-debug kernel.elf kernel.sym
	@objcopy -O binary kernel.elf asm/kernel.bin
	@echo "OC kernel.bin"

%.o: %.c $(C_SOURCES) libc/include/bits/alltypes.h libc/include/bits/syscall.h
	@$(CC) $(CCFLAGS) -c $< -o $@
	@echo "CC $<"

os-image: asm/boot_sect.bin asm/kernel.bin
	@cat $^ > os-image

debug:
	gdb \
		-ex="target remote :1234" \
		-ex="symbol-file kernel.elf" \
		-ex="set disassembly-flavor intel"

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

