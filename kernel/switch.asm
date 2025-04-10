; kernel/switch.asm
; Enhanced context switching with full state preservation

global context_switch
global timer_interrupt_handler

extern schedule

; Main context switch function
; Arguments:
;   eax = pointer to current task's esp
;   ebx = pointer to next task's esp
context_switch:
    ; Save all general purpose registers
    pusha
    
    ; Save current stack pointer
    mov [eax], esp
    
    ; Load next task's stack pointer
    mov esp, [ebx]
    
    ; Restore all general purpose registers
    popa
    
    ret

; Timer interrupt handler
timer_interrupt_handler:
    ; Save all registers
    pusha
    
    ; Call C scheduler function
    call schedule
    
    ; Restore registers
    popa
    
    ; Return from interrupt
    iret
