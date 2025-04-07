; kernel/boot_header.asm
BITS 32

section .multiboot
align 4
    dd 0x1BADB002              ; magic number
    dd 0x00                    ; flags
    dd - (0x1BADB002 + 0x00)   ; checksum

section .text
global multiboot_entry
multiboot_entry:
    ; Just jump to kernel_main (defined in kernel.c)
    extern kernel_main
    call kernel_main
    cli
.hang:
    hlt
    jmp .hang
