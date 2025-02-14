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
    lea rsi, [used1 + 8*rax]; used1[char]
    mov r8, [rsi]
    inc r8
    mov [rsi], r8
    jmp read_string
end_read:
    mov byte[rdi], `\0`
    lea rdi, [string1]
read_string1:
    mov rax, 0
    GET_CHAR al
    cmp al, '.'
    je end_read1
    stosb
    lea rsi, [used2 + 8*rax]; used1[char]
    mov r8, [rsi]
    inc r8
    mov [rsi], r8
    jmp read_string1
end_read1:
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
    lea rsi, [string1]
print_string1:
    lodsb
    cmp al, `\0`
    je end_print1
    PRINT_CHAR al
    jmp print_string1
end_print1:
    PRINT_CHAR `\n`
    mov rcx, 0
    for:
        cmp rcx, 256
        jge end_for
        lea rsi, [used1 + 8*rcx]
        lea rdi, [used2 + 8*rcx]
        mov rax, [rsi]
        mov rbx, [rdi]
        cmp rax, 0
        je else_end
        jmp good
        good:
        cmp rbx, 0
        je else_end
        PRINT_CHAR rcx
        else_end:
        inc rcx
        jmp for
    end_for:
        
    ret