%include "io64.inc"

section .data
    n dq 10

section .bss
    a resq 10

section .text
global main
main:
    mov rbp, rsp; for correct debugging
    xor rcx, rcx
    lea rdi, [a]
.loop:
    cmp rcx, qword[n]
    jge .end
    GET_DEC 8, rax
    stosq
    inc rcx
    jmp .loop
.end:
    push qword[n]
    lea rdi, [a]
    push rdi
    call mas
    PRINT_DEC 8, rax
    ret
mas:
    enter 0, 0 
    mov rax, 0 ;ans
    mov rcx, 0 ;i

.loop:
    cmp rcx, [rbp + 24]
    jge .end
    mov rdi, [rbp + 16]
    mov rdx, qword[rdi + 8 * rcx]
    add rax, rdx
    inc rcx
    jmp .loop
.end:   
    leave
    ret