%include "io64.inc"

section .data
    arr dq 1

section .text

global CMAIN
CMAIN:
    mov rcx, 1
    lea rsi, [arr]
    lea rdi, [arr + 8 * rcx - 8]
    mov al, 1
check:
    cmp rsi, rdi
    jae end_check
    mov rbx, [rsi]
    cmp rbx, [rdi]
    jne bad_check
    add rsi, 8
    sub rdi, 8
    jmp check
bad_check:
    mov al, 0
end_check:
    
    
    cmp al, 0
    jne palindrom
    PRINT_STRING 'NOT PALINDROM'
    NEWLINE
    jmp end
palindrom:
    PRINT_STRING 'PALINDROM'
    NEWLINE
end:
    ret