%include "io64.inc"

section .bss
    string resb 256
    string1 resb 256

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
    lea rdi, [string1]
read_string1:
    mov rax, 0
    GET_CHAR al
    cmp al, '.'
    je end_read1
    stosb
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
    lea rdi, [string]
if1:
    cmp byte[rdi], `\0`
    je end
    lea rsi,[string1]
    mov rcx, 0
for:
    lea rdi, [string + rcx]
if2:
    mov bl, byte[rdi] 
    cmp bl, byte[rsi]
    jne else2
    mov r15, 0
while2:
    mov r14, rcx
    add r14, r15
    lea rdi, [string + r14]
    cmp byte[rdi], `\0`
    je while2_end
    lea rsi, [string1 + r15]
    mov bl, byte[rdi] 
    cmp bl, byte[rsi]
    jne while2_end
    inc r15 
    jmp while2
while2_end:
    cmp byte[rsi + r15], `\0`
    je end
else2:  
    inc rcx
    jmp for
end:
    PRINT_DEC 8, rcx
    ret