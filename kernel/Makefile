# Compiler and linker
CC = i686-elf-gcc
LD = i686-elf-ld
CFLAGS = -ffreestanding -m32 -Wall -Wextra -O2 -Ikernel -fno-pie -fno-stack-protector -nostdlib -nostdinc -fno-builtin
LDFLAGS = -T linker.ld -nostdlib -z noexecstack

# Host compiler for tools
HOST_CC = gcc
HOST_CFLAGS = -Wall -Wextra -O2

# Source files
ASM_SRC = kernel/boot_header.asm kernel/isr.asm kernel/irq.asm kernel/idt.asm kernel/switch.asm
ASM_OBJ = $(ASM_SRC:.asm=.o)

C_SRC = kernel/kernel.c kernel/screen.c kernel/keyboard.c kernel/ports.c kernel/string.c \
        kernel/idt.c kernel/isr.c kernel/interrupts.c kernel/time.c kernel/irq.c \
        kernel/task.c kernel/scheduler.c kernel/memory.c \
        kernel/fs/vfs.c kernel/fs/initrd.c
C_OBJ = $(C_SRC:.c=.o)

OBJS = $(ASM_OBJ) $(C_OBJ)

KERNEL_BIN = kernel.bin
ISO_NAME = qos.iso
INITRD_NAME = initrd.img

# Tools
MKINITRD = tools/mkinitrd
INITRD_DIR = initrd

# Default target
all: $(KERNEL_BIN) $(INITRD_NAME) iso

# Assemble .asm files
%.o: %.asm
	nasm -f elf32 $< -o $@

# Compile .c files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build mkinitrd tool
$(MKINITRD): tools/mkinitrd.c
	$(HOST_CC) $(HOST_CFLAGS) -o $@ $<

# Create initial ramdisk
$(INITRD_NAME): $(MKINITRD) $(INITRD_DIR)
	$(MKINITRD) $(INITRD_NAME) $(INITRD_DIR)

# Link the kernel
$(KERNEL_BIN): $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

# Create bootable ISO using GRUB
iso: $(KERNEL_BIN) $(INITRD_NAME)
	mkdir -p iso/boot/grub
	cp $(KERNEL_BIN) iso/boot/kernel.bin
	cp $(INITRD_NAME) iso/boot/initrd.img
	printf 'set timeout=0\nset default=0\n\nmenuentry "QOS" {\n\tmultiboot /boot/kernel.bin\n\tmodule /boot/initrd.img\n\tboot\n}\n' > iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO_NAME) iso > /dev/null 2>&1

# Run with QEMU
run: $(ISO_NAME)
	qemu-system-i386 -cdrom $(ISO_NAME)

# Clean all build artifacts
clean:
	rm -f $(OBJS) $(KERNEL_BIN) $(ISO_NAME) $(INITRD_NAME) $(MKINITRD)
	rm -rf iso

# Create initrd directory if it doesn't exist
$(INITRD_DIR):
	mkdir -p $(INITRD_DIR)

.PHONY: all clean run 