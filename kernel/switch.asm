; kernel/switch.asm
; Context switching assembly code

global context_switch

context_switch:
    ; Arguments: eax = current esp, ebx = current ebp, ecx = next esp, edx = next ebp
    ; Save current task's stack state
    mov [eax], esp
    mov [ebx], ebp

    ; Load next task's stack state
    mov esp, [ecx]
    mov ebp, [edx]

    ret
