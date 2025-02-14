%include "io64.inc"

section .data
    x db 4
    char db 0
section .text
global main
main:
    
cycle:
    GET_CHAR char
    mov rax, '.'
    cmp al, [char]
    jne cycle_body
    jmp cycle_end
cycle_body:
    mov rax, 'a'
    cmp byte [char], al
    jl else
    mov rax, 'z'
    cmp byte [char], al
    jg else; char<'a' || char > 'z' => else
    sub rax, 'Z' ; rax = 'z'-'Z'
    mov rdx, [char]
    sub dl, al
    PRINT_CHAR rdx
    jmp cycle
else:
    mov rax, 'A'
    cmp [char], al
    jl else2
    mov rax, 'Z'
    cmp [char], al
    jg else2
    if2:
        sub rax, 'z'; 'Z'-'z'
        mov rdx, [char]
        sub dl, al;
        PRINT_CHAR rdx
        jmp cycle
    else2:
        PRINT_CHAR char
    jmp cycle
cycle_end:
    ret


      
