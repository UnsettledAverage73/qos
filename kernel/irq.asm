; kernel/irq.asm

[BITS 32]

global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

extern irq_handler

; Each IRQ handler pushes its IRQ number (32–47),
; then jumps to a shared `irq_handler` defined in C

%macro IRQ_HANDLER 2
%1:
    cli
    push dword 0       ; Dummy error code (some CPUs need this)
    push dword %2      ; IRQ number
    jmp irq_common_stub
%endmacro

IRQ_HANDLER irq0, 32
IRQ_HANDLER irq1, 33
IRQ_HANDLER irq2, 34
IRQ_HANDLER irq3, 35
IRQ_HANDLER irq4, 36
IRQ_HANDLER irq5, 37
IRQ_HANDLER irq6, 38
IRQ_HANDLER irq7, 39
IRQ_HANDLER irq8, 40
IRQ_HANDLER irq9, 41
IRQ_HANDLER irq10, 42
IRQ_HANDLER irq11, 43
IRQ_HANDLER irq12, 44
IRQ_HANDLER irq13, 45
IRQ_HANDLER irq14, 46
IRQ_HANDLER irq15, 47

; Shared IRQ stub: saves registers, calls C handler, restores, and iret
irq_common_stub:
    pusha                     ; Save all general-purpose registers
    mov ax, ds
    push eax                  ; Save data segment

    mov ax, 0x10              ; Load kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call irq_handler          ; Call the C-level IRQ handler

    pop eax                   ; Restore data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                      ; Restore all general-purpose registers
    add esp, 8                ; Pop error code and IRQ number
    sti
    iret
