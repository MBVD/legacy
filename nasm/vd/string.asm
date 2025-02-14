%include "io64.inc"

section .bss
    string resb 256
    string1 resb 256

section .text
global CMAIN
CMAIN:
    mov rbp, rsp; for correct debugging
    ;read
    lea rdi, [string]
read_string:
    GET_CHAR al
    cmp al, '.'
    je end_read
    stosb
    jmp read_string
end_read:
    mov byte[rdi], `\0`
    ;
    
    ;print
    lea rsi, [string]
print_string:
    lodsb
    cmp al, `\0`
    je end_print
    PRINT_CHAR al
    jmp print_string
end_print:
    ;
    
    ;copy
    lea rsi, [string]
    lea rdi, [string1]    
copy_string:
    cmp byte[rsi], `\0`
    je end_copy
    movsb
    jmp copy_string
end_copy:
    mov byte[rdi], `\0`     
    ;
    lea rsi, [string1]
print_string1:
    lodsb
    cmp al, `\0`
    je end_print1
    PRINT_CHAR al
    jmp print_string1
end_print1:
    ret