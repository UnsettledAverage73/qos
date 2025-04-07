# Compiler and linker
CC = i686-elf-gcc
LD = i686-elf-ld
CFLAGS = -ffreestanding -m32 -Wall -Wextra -Ikernel
LDFLAGS = -T linker.ld -nostdlib

# Source files
ASM_SRC = kernel/boot_header.asm kernel/isr.asm kernel/irq.asm kernel/idt.asm
ASM_OBJ = kernel/kernel_entry.o kernel/isr_asm.o kernel/irq_asm.o kernel/idt_asm.o

C_SRC = kernel/kernel.c kernel/screen.c kernel/keyboard.c kernel/ports.c kernel/string.c \
        kernel/idt.c kernel/isr.c kernel/interrupts.c kernel/time.c kernel/irq.c
C_OBJ = kernel/kernel.o kernel/screen.o kernel/keyboard.o kernel/ports.o kernel/string.o \
        kernel/idt.o kernel/isr.o kernel/interrupts.o kernel/time.o kernel/irq.o

OBJS = $(ASM_OBJ) $(C_OBJ)

KERNEL_BIN = kernel.bin
ISO_NAME = qos.iso

# Default target
all: $(KERNEL_BIN) iso

# Assemble .asm files
kernel/kernel_entry.o: kernel/boot_header.asm
	nasm -f elf32 $< -o $@

kernel/isr_asm.o: kernel/isr.asm
	nasm -f elf32 $< -o $@

kernel/irq_asm.o: kernel/irq.asm
	nasm -f elf32 $< -o $@

kernel/idt_asm.o: kernel/idt.asm
	nasm -f elf32 $< -o $@

# Compile .c files
kernel/%.o: kernel/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link the kernel
$(KERNEL_BIN): $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

# Create bootable ISO using GRUB
iso: $(KERNEL_BIN)
	mkdir -p iso/boot/grub
	cp $(KERNEL_BIN) iso/boot/kernel.bin
	printf 'set timeout=0\nset default=0\n\nmenuentry "QOS" {\n\tmultiboot /boot/kernel.bin\n\tboot\n}\n' > iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO_NAME) iso > /dev/null 2>&1

# Run with QEMU
run: $(ISO_NAME)
	qemu-system-i386 -cdrom $(ISO_NAME)

# Clean all build artifacts
clean:
	rm -f $(OBJS) $(KERNEL_BIN) $(ISO_NAME)
	rm -rf iso

