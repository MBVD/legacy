%include "io64.inc"

section .bss
    string resb 256
    string1 resb 256
    used1 resq 256
    used2 resq 256

section .text
global main
main:
    mov rbp, rsp; for correct debugging
    ;read
    lea rdi, [string]
read_string:
    mov rax, 0
    GET_CHAR al
    cmp al, '.'
    je end_read
    stosb
    jmp read_string
end_read:
    mov byte[rdi], `\0`
    lea rsi, [string]
print_string:
    lodsb
    cmp al, `\0`
    je end_print
    PRINT_CHAR al
    jmp print_string
end_print:
    PRINT_CHAR `\n`
    mov rbx, 0; number
    lea rsi, [string]
for: 
    mov rax, 0
    lodsb
    cmp al, `\0`
    je end_for
    ; num = num * 16
    mov rcx, rax; rcx = char
    mov rax, rbx
    mov rdx, 16
    mul rdx; rax = rax*dx
    mov rbx, rax
    mov rax, rcx
    ; number = number * 16
    cmp al, '0'
    jge if1
    jmp else
    if1:
        cmp al, '9'
        jg else
        mov r8, 0
        mov r8, rax
        sub rax, '0'; str[i] - '0'
        add rbx, rax
        jmp else_end
    else:
        cmp al, 'A'
        jge if2
        jmp else2
        if2:
            cmp al, 'F'
            jg else2
            mov r8, 0
            mov r8, rax
            sub rax, 'A'
            add rax, 10
            add rbx, rax
            jmp else_end
        else2:
    else_end:
    jmp for 
end_for:
    PRINT_DEC 8, rbx
    ret
    
   