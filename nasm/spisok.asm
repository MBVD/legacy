%include "io64.inc"

section .text
global CMAIN


parser:
    ;rax - count
    ;al - c
    ;bl - flag
    ;rdx - счетчик 2х точек
    xor rdx, rdx
    xor rdi, rdi
    mov bl, 1

.while:
    GET_CHAR al
    cmp al, `\n`
    je .end
    
    cmp al, `-`
    je .else
    cmp al, ` `
    je .else
    cmp al, `.`
    je .else
    cmp al, `0`
    jle .continue
    cmp al, `9`
    jge .continue
    jmp .else

.continue:
    mov bl, 0

.else:
    cmp al, `.`
    jne .continue_2
    inc rdx
    
.continue_2:
    cmp al, ` `
    jne .else_2

    cmp bl, 0
    je .else_3
    cmp rdx, 1
    jg .else_3
    inc rdi
    jmp .while
.else_3:
    mov bl, 1
    xor rdx, rdx
    jmp .while
.else_2:
    jmp .while

.end:
    ret

CMAIN:
    mov rbp, rsp; for correct debugging
    enter 8, 0
    mov qword[rbp-8], 0
    xor rax, rax
    xor rbx, rbx
    call parser
    mov [rbp-8], rdi
    
    PRINT_DEC 8, rdi

    leave
    ret
