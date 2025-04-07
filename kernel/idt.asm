; kernel/idt.asm

global idt_flush      ; Make it accessible from C

idt_flush:
    mov eax, [esp + 4]  ; Get the pointer argument from C
    lidt [eax]          ; Load the IDT using lidt
    ret
