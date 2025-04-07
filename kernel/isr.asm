extern isr_handler
[bits 32]

global isr0

isr0:
    cli
    pusha
    call isr_handler
    popa
    sti
    iret
