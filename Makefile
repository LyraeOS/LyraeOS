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
	-Werror \
	-nostdlib

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

override ZIG := zig
override ZIGFLAGS := build-obj -target x86_64-freestanding -O ReleaseSmall -fno-emit-bin -fno-emit-h -I src
override SRCFILES := $(shell find -L src -type f 2>/dev/null | LC_ALL=C sort)
override CFILES := $(filter %.c,$(SRCFILES))
override ASFILES := $(filter %.S,$(SRCFILES))
override NASMFILES := $(filter %.asm,$(SRCFILES))
override OBJ := $(addprefix obj/,$(CFILES:.c=.c.o) $(ASFILES:.S=.S.o) $(NASMFILES:.asm=.asm.o))
override ZIGFILES := $(filter %.zig,$(SRCFILES))
override OBJ += $(addprefix obj/,$(ZIGFILES:.zig=.zig.o))
override HEADER_DEPS := $(addprefix obj/,$(CFILES:.c=.c.d) $(ASFILES:.S=.S.d))

.PHONY: all
all: bin/$(OUTPUT)
image: bin/image.hdd

-include $(HEADER_DEPS)

bin/$(OUTPUT): linker.lds $(OBJ)
	mkdir -p "$(dir $@)"
	ld $(LDFLAGS) $(OBJ) -o $@

obj/%.c.o: %.c
	mkdir -p "$(dir $@)"
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@

obj/%.S.o: %.S
	mkdir -p "$(dir $@)"
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@

obj/%.asm.o: %.asm
	mkdir -p "$(dir $@)"
	nasm $(NASMFLAGS) $< -o $@

obj/%.zig.o: %.zig
	mkdir -p "$(dir $@)"
	$(ZIG) $(ZIGFLAGS) $< -femit-bin=$@

bin/image.hdd: bin/$(OUTPUT)
	dd if=/dev/zero bs=1M count=0 seek=64 of=bin/image.hdd
	PATH=$PATH:/usr/sbin:/sbin sgdisk bin/image.hdd -n 1:2048 -t 1:ef00 -m 1
	./limine/limine bios-install bin/image.hdd
	mformat -i bin/image.hdd@@1M
	mmd -i bin/image.hdd@@1M ::/EFI ::/EFI/BOOT ::/boot ::/boot/limine
	mcopy -i bin/image.hdd@@1M bin/$(OUTPUT) ::/boot
	mcopy -i bin/image.hdd@@1M limine.conf limine/limine-bios.sys ::/boot/limine
	mcopy -i bin/image.hdd@@1M limine/BOOTX64.EFI ::/EFI/BOOT
	mcopy -i bin/image.hdd@@1M limine/BOOTIA32.EFI ::/EFI/BOOT

run: bin/image.hdd
	qemu-system-x86_64 -hda bin/image.hdd -accel kvm -m 2G
# PLEASE DO NOT RUN UNLESS YOU CHECK YOUR SDB
usb: bin/image.hdd
	sudo dd if=bin/image.hdd of=/dev/sdb oflag=direct bs=1M status=progress

# Remove object files and the final executable.
.PHONY: clean
clean:
	rm -rf bin obj
