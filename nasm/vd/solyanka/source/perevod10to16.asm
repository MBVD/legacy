%include "io64.inc"
section .data
    n dq 21
section .text
global main
perevod:
    enter 8, 0
    cmp qword[rbp + 16], 0
    je .return
    xor rdx, rdx
    mov rax, [rbp + 16]
    mov rbx, 16
    idiv rbx
    mov [rbp - 8], rdx
    push rax
    call perevod
    mov rax, qword[rbp-8]
    cmp rax, 10
    jl .number
    jge .char
.number:
    PRINT_DEC 8, [rbp-8]
    jmp .return
.char:    
    add byte[rbp-8], 'A'
    sub byte[rbp-8], 10
    mov al, byte[rbp-8]
    PRINT_CHAR al
.return:
    leave
    ret
main:
    mov rbp, rsp; for correct debugging
    GET_DEC 8, n
    push qword[n]
    call perevod
    ret
    