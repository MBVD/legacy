%include "io64.inc"

section .bss
    string resb 256 
    string1 resb 256      

section .text
global CMAIN
CMAIN:
    ;read
    lea rdi, [string]
read_string:
    GET_CHAR al
    cmp al, '.'
    je end_read
    stosb ; al zapisi v rdi i sdvig na 1
        ;mov [rdi],al
        ;inc rdi
    jmp read_string
    ;
end_read:
    mov byte[rdi], '\0'
    ;
    ;print
    lea rsi, [string]
print_string:
    lodsb ;1 byte v al, sdvig
    cmp al, '\0'
    je end_print
    PRINT_CHAR al
    jmp print_string
end_print:
    ; copy
    lea rsi, [string]
    lea rdi, [string1]
copy_string:
    movsb
    cmp byte[rsi], '\0'
    je end_copy
    jmp copy_string
end_copy:
    mov byte[rdi], '\0'
    ;    
    lea rsi, [string]
print_string1:
    lodsb ;1 byte v al, sdvig
    cmp al, '\0'
    je end_print1
    PRINT_CHAR al
    jmp print_string1
end_print1:
    ret    
    