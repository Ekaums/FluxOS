CC = x86_64-elf-gcc
LD = x86_64-elf-ld

# Nuke built-in rules and variables.
override MAKEFLAGS += -rR
 
# This is the name that our final kernel executable will have.
# Change as needed.
override KERNEL := myos
 
# Convenience macro to reliably declare user overridable variables.
define DEFAULT_VAR =
    ifeq ($(origin $1),default)
        override $(1) := $(2)
    endif
    ifeq ($(origin $1),undefined)
        override $(1) := $(2)
    endif
endef
 
override DEFAULT_CC := cc
$(eval $(call DEFAULT_VAR,CC,$(DEFAULT_CC)))
 
override DEFAULT_LD := ld
$(eval $(call DEFAULT_VAR,LD,$(DEFAULT_LD)))
 
# C flags.
override DEFAULT_CFLAGS := -g -O2 -pipe
$(eval $(call DEFAULT_VAR,CFLAGS,$(DEFAULT_CFLAGS)))
 
# C preprocessor flags. We set none by default.
override DEFAULT_CPPFLAGS :=
$(eval $(call DEFAULT_VAR,CPPFLAGS,$(DEFAULT_CPPFLAGS)))
 
# User controllable nasm flags.
override DEFAULT_NASMFLAGS := -F dwarf -g
$(eval $(call DEFAULT_VAR,NASMFLAGS,$(DEFAULT_NASMFLAGS)))
 
# User controllable linker flags. We set none by default.
override DEFAULT_LDFLAGS :=
$(eval $(call DEFAULT_VAR,LDFLAGS,$(DEFAULT_LDFLAGS)))
 
# Internal C flags
override CFLAGS += \
    -Wall \
    -Wextra \
    -std=gnu11 \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-lto \
    -fPIE \
    -m64 \
    -march=x86-64 \
    -mno-80387 \
    -mno-mmx \
    -mno-sse \
    -mno-sse2 \
    -mno-red-zone
 
# Internal C preprocessor flags
override CPPFLAGS := \
    -I src \
    $(CPPFLAGS) \
    -MMD \
    -MP
 
# Internal linker flags
override LDFLAGS += \
    -m elf_x86_64 \
    -nostdlib \
    -static \
    -pie \
    --no-dynamic-linker \
    -z text \
    -z max-page-size=0x1000 \
    -T linker.ld
 
# Internal nasm flags
override NASMFLAGS += \
    -Wall \
    -f elf64
 
# Use "find" to glob all *.c, *.S, and *.asm files in the tree and obtain the
# object and header dependency file names.
override CFILES := $(shell cd src && find -L * -type f -name '*.c')
override ASFILES := $(shell cd src && find -L * -type f -name '*.S')
override NASMFILES := $(shell cd src && find -L * -type f -name '*.asm')
override OBJ := $(addprefix obj/,$(CFILES:.c=.c.o) $(ASFILES:.S=.S.o) $(NASMFILES:.asm=.asm.o))
override HEADER_DEPS := $(addprefix obj/,$(CFILES:.c=.c.d) $(ASFILES:.S=.S.d))

# Default target.
.PHONY: all
all: bin/$(KERNEL) bootdisk
 
# Link rules for the final kernel executable.
bin/$(KERNEL): GNUmakefile linker.ld $(OBJ)
	mkdir -p "$$(dirname $@)"
	$(LD) $(OBJ) $(LDFLAGS) -o $@
 
# Include header dependencies.
-include $(HEADER_DEPS)
 
# Compilation rules for *.c files.
obj/%.c.o: src/%.c GNUmakefile
	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
 
# Compilation rules for *.S files.
obj/%.S.o: src/%.S GNUmakefile
	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Compilation rules for *.asm (nasm) files.
obj/%.asm.o: src/%.asm GNUmakefile
	mkdir -p "$$(dirname $@)"
	nasm $(NASMFLAGS) $< -o $@
 
bootdisk:
	dd if=/dev/zero bs=1M count=0 seek=64 of=image.hdd
	sgdisk image.hdd -n 1\:2048 -t 1:ef00
	./limine/limine bios-install image.hdd
	mformat -i image.hdd@@1M
	mmd -i image.hdd@@1M ::/EFI ::/EFI/BOOT
	mcopy -i image.hdd@@1M bin/myos limine.cfg limine/limine-bios.sys ::/
	mcopy -i image.hdd@@1M limine/BOOTX64.EFI ::/EFI/BOOT
	mcopy -i image.hdd@@1M limine/BOOTIA32.EFI ::/EFI/BOOT

qemu:
	qemu-system-x86_64 -D qemu.log -d int -M smm=off -hda image.hdd

bochs:
	bochs -f bochsrc.txt -q



# Remove object files and the final executable.
.PHONY: clean
clean:
	rm -rf bin obj
	rm -f image.hdd
	rm -f bochslog.txt
	rm -f qemu.log