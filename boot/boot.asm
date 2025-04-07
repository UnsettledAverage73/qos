; boot/boot.asm - 512 bytes bootloader
BITS 16
org 0x7c00

start:
    cli                     ; Disable interrupts
    mov ax, 0x07C0
    add ax, 288
    mov ss, ax
    mov sp, 0xFFFF
    sti

    mov si, msg
    call print_string

    jmp $

print_string:
    mov ah, 0x0E
.next:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .next
.done:
    ret

msg db "QOS Booting... Welcome!", 0

times 510 - ($ - $$) db 0
dw 0xAA55
