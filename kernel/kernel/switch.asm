global switch_task

extern current_task

; void switch_task(task_t* next_task)
switch_task:
    ; Save current task's state
    mov eax, [current_task]
    mov [eax + 0], esp    ; Save ESP
    mov [eax + 4], ebp    ; Save EBP
    mov [eax + 8], ebx    ; Save EBX
    mov [eax + 12], esi   ; Save ESI
    mov [eax + 16], edi   ; Save EDI
    mov [eax + 20], eip   ; Save EIP

    ; Load next task's state
    mov eax, [esp + 4]    ; Get next_task parameter
    mov [current_task], eax
    mov esp, [eax + 0]    ; Restore ESP
    mov ebp, [eax + 4]    ; Restore EBP
    mov ebx, [eax + 8]    ; Restore EBX
    mov esi, [eax + 12]   ; Restore ESI
    mov edi, [eax + 16]   ; Restore EDI
    mov eax, [eax + 20]   ; Get saved EIP
    jmp eax               ; Jump to saved EIP 