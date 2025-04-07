; kernel/idt_flush.asm

global idt_flush

idt_flush:
    mov eax, [esp + 4]  ; address of idtp
    lidt [eax]
    ret
