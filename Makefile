rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

ARCH = i386
C_SOURCES = $(call rwildcard,*,*.c)
ASM_SOURCES = $(call rwildcard,libc/*,*.asm)
ASM_KERNEL_SOURCES = $(call rwildcard,kernel/*,*.asm)
ASM_ARCH_SOURCES = $(call rwildcard,arch/$(ARCH)*,*.asm)
OBJ := $(C_SOURCES:.c=.o)
ASM_OBJ := $(ASM_SOURCES:.asm=.o)
ASM_OBJ += $(ASM_KERNEL_SOURCES:.asm=.o)
ASM_OBJ += $(ASM_ARCH_SOURCES:.asm=.o)
CROSS = $(HOME)/opt/cross/
CC = $(CROSS)bin/i686-elf-gcc
AS = yasm
# ARCH = $(shell uname -m)
QEMU = $(shell find /usr/bin -name "qemu-system-$(ARCH)")
CCFLAGS = -std=c99 -m32 -ffreestanding -nostdlib -static-libgcc -lgcc \
		  -I. -Ilibc/include -Iinclude  -Ilibc/arch/$(ARCH)/ \
		  -fno-asynchronous-unwind-tables -fdiagnostics-color=auto \
		  -ggdb -Wall -Wno-pointer-sign \
		  -D_POSIX_SOURCE

all: os-image

%.bin: %.asm
	@$(AS) -i asm/ $^ -f bin -o $@
	@echo "AS $<"

%.o: %.asm
	@$(AS) $^ -f elf -g DWARF2 -o $@
	@echo "AS $<"

asm/kernel.bin: asm/kernel_entry.o $(OBJ) $(ASM_OBJ)
	@ln -fs $(shell $(CC) -print-file-name=libgcc.a)
	@echo "LN libgcc.a"

	@ld -o kernel.elf \
	  -zmuldefs \
	  -m elf_i386 \
	  --entry=main \
	  -Ttext=0x10000 \
	  $^ \
	  libgcc.a
	@echo "LD kernel.elf"

	@objcopy --only-keep-debug kernel.elf kernel.sym
	@cp kernel.elf kernel_strip.elf
	@strip --remove-section=.note.gnu.property kernel_strip.elf 2>/dev/null
	@objcopy -O binary kernel_strip.elf asm/kernel.bin
	@echo "OC kernel.bin"

%.o: %.c $(C_SOURCES) libc/include/bits/alltypes.h libc/include/bits/syscall.h
	@$(CC) $(CCFLAGS) -c $< -o $@

os-image: asm/boot_sect.bin asm/kernel.bin
	@cat $^ > os-image

debug:
	gdb \
		-ex="target remote :1234" \
		-ex="symbol-file kernel.elf" \
		-ex="set disassembly-flavor intel"

debug-qemu: os-image
	$(QEMU) -drive format=raw,file=$^ \
		-drive format=raw,file=fat8.img \
		-m 256M \
		-monitor stdio -m 1 -s -S \
		-d guest_errors,cpu_reset,int

clean:
	@for f in `find . -path "./fatmnt" -prune \
		-o -name "*.bin" \
		-o -name "*.o" \
		-o -name "os-image" \
		-o -name "*.elf" \
		-o -name "*.sym" \
		-o -wholename "libc/include/bits/*.h" \
		-o -name "libgcc.a"`; do \
		rm $$f; \
	done

tags:
	ctags -R

run: os-image
	$(QEMU) -drive format=raw,file=$^ \
		-drive format=raw,file=fat8.img \
		-m 256M

libc/include/bits/syscall.h: libc/arch/$(ARCH)/bits/syscall.h.in
	@cp $< $@
	@sed -ine 's|__NR_|SYS_|p' $@
	@echo "SED $<"

libc/include/bits/alltypes.h: libc/arch/$(ARCH)/bits/alltypes.h.in libc/include/alltypes.h.in tools/mkalltypes.sed
	@mkdir -p libc/include/bits/
	@sed -f tools/mkalltypes.sed libc/arch/$(ARCH)/bits/alltypes.h.in libc/include/alltypes.h.in > $@
	@echo "SED $<"

