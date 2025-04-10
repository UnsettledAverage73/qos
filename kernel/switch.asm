; kernel/switch.asm
; Enhanced context switching with full state preservation

global context_switch
global timer_interrupt_handler

extern schedule

; Main context switch function
; Arguments:
;   eax = pointer to current task's context
;   ebx = pointer to next task's context
context_switch:
    ; Save current context
    pusha
    pushf
    mov ecx, [esp + 44]    ; Get return address (eip)
    mov [eax + 28], ecx    ; Save eip
    mov ecx, [esp + 4]     ; Get eflags
    mov [eax + 32], ecx    ; Save eflags
    mov [eax + 20], esp    ; Save esp
    mov [eax + 24], ebp    ; Save ebp
    
    ; Load next context
    mov esp, [ebx + 20]    ; Load esp
    mov ebp, [ebx + 24]    ; Load ebp
    push dword [ebx + 32]  ; eflags
    push dword [ebx + 28]  ; eip
    popf                   ; Restore flags
    popa                   ; Restore general registers
    ret                    ; Jump to new eip

; Timer interrupt handler
timer_interrupt_handler:
    ; Save all registers
    pusha
    push ds
    push es
    push fs
    push gs
    
    ; Call C scheduler function
    call schedule
    
    ; Restore registers
    pop gs
    pop fs
    pop es
    pop ds
    popa
    
    ; Return from interrupt
    iret
