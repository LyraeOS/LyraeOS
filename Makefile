.SUFFIXES:

override OUTPUT := LyraeOS

CFLAGS := -g -O2 -pipe

CPPFLAGS :=

NASMFLAGS := -g

LDFLAGS :=

override CC_IS_CLANG := $(shell ! $(CC) --version 2>/dev/null | grep -q '^Target: '; echo $$?)

override CFLAGS += \
	-Wall \
	-Wextra \
	-std=gnu11 \
	-ffreestanding \
	-fno-stack-protector \
	-fno-omit-frame-pointer \
	-fno-stack-check \
	-fno-lto \
	-fno-PIC \
	-ffunction-sections \
	-fdata-sections \
	-m64 \
	-march=x86-64 \
	-mabi=sysv \
	-mno-80387 \
	-mno-mmx \
	-mno-red-zone \
	-mcmodel=kernel \
	-Werror

override CPPFLAGS := \
	-I src \
	$(CPPFLAGS) \
	-MMD \
	-MP

override NASMFLAGS := \
	-f elf64 \
	$(patsubst -g,-g -F dwarf,$(NASMFLAGS)) \
	-Wall

override LDFLAGS += \
	-m elf_x86_64 \
	-nostdlib \
	-static \
	-z max-page-size=0x1000 \
	--gc-sections \
	-T linker.lds \
	-no-pie

override SRCFILES := $(shell find -L src -type f 2>/dev/null | LC_ALL=C sort)
override CFILES := $(filter %.c,$(SRCFILES))
override ASFILES := $(filter %.S,$(SRCFILES))
override NASMFILES := $(filter %.asm,$(SRCFILES))
override OBJ := $(addprefix obj/,$(CFILES:.c=.c.o) $(ASFILES:.S=.S.o) $(NASMFILES:.asm=.asm.o))
override HEADER_DEPS := $(addprefix obj/,$(CFILES:.c=.c.d) $(ASFILES:.S=.S.d))

.PHONY: all
all: bin/$(OUTPUT).iso
image: bin/image.hdd

-include $(HEADER_DEPS)

bin/$(OUTPUT): linker.lds $(OBJ)
	mkdir -p "$(dir $@)"
	ld $(LDFLAGS) $(OBJ) -o $@

obj/%.c.o: %.c
	mkdir -p "$(dir $@)"
	cc $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Compilation rules for *.S files.
obj/%.S.o: %.S
	mkdir -p "$(dir $@)"
	cc $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Compilation rules for *.asm (nasm) files.
obj/%.asm.o: %.asm
	mkdir -p "$(dir $@)"
	nasm $(NASMFLAGS) $< -o $@

bin/$(OUTPUT).iso: limine-binary/limine bin/$(OUTPUT)
	rm -rf bin/iso_root
	mkdir -p bin/iso_root/boot
	cp -v bin/$(OUTPUT) bin/iso_root/boot/
	mkdir -p bin/iso_root/boot/limine
	cp -v limine.conf bin/iso_root/boot/limine/
	mkdir -p bin/iso_root/EFI/BOOT
	cp -v limine-binary/limine-bios.sys limine-binary/limine-bios-cd.bin limine-binary/limine-uefi-cd.bin bin/iso_root/boot/limine/
	cp -v limine-binary/BOOTX64.EFI bin/iso_root/EFI/BOOT/
	cp -v limine-binary/BOOTIA32.EFI bin/iso_root/EFI/BOOT/
	xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
		-apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		bin/iso_root -o bin/$(OUTPUT).iso
	./limine-binary/limine bios-install bin/$(OUTPUT).iso

.PHONY: run-bios
run-bios: bin/$(OUTPUT).iso
	qemu-system-x86_64 -cdrom bin/$(OUTPUT).iso -m 2G -serial stdio
.PHONY: run-efi
run-efi: edk2-ovmf-bins bin/$(OUTPUT).iso
	qemu-system-x86_64 \
		-M q35 \
		-drive if=pflash,unit=0,format=raw,file=edk2-ovmf-bins/ovmf-code-x86_64.fd,readonly=on \
		-cdrom bin/$(OUTPUT).iso \
		-m 8G \
		-serial stdio \
		-d int,cpu_reset -D qemu.log

# PLEASE DO NOT RUN UNLESS YOU CHECK YOUR SDB
usb: bin/$(OUTPUT).iso
	sudo dd if=bin/$(OUTPUT).iso of=/dev/sdb oflag=direct bs=1M status=progress

.PHONY: clean
clean:
	rm -rf bin obj

.PHONY: distclean
distclean: clean
	rm -rf edk2-ovmf-bins limine-binary

edk2-ovmf-bins:
	curl -L https://github.com/osdev0/edk2-ovmf-stable-bins/releases/latest/download/edk2-ovmf-bins.tar.gz | gunzip | tar -xf -

limine-binary/limine:
	rm -rf limine-binary
	curl -L https://github.com/Limine-Bootloader/Limine/releases/latest/download/limine-binary.tar.gz | gunzip | tar -xf -
	$(MAKE) -C limine-binary
