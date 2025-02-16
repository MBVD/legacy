section .data
    printf_format db "%g", 10, 0
    scanf_format db "%lf%lf", 0
section .bss
    a resq 1
    b resq 1

section .text

extern printf
extern scanf

global main 

main:
    enter 0, 0

    lea rdi, [scanf_format] 
    lea rsi, [a]
    lea rdx, [b]
    call scanf

    movsd xmm0, [a]
    mulsd xmm0, [b]

    lea rdi, [printf_format]
    mov rax, 1
    call printf

    xor rax, rax
    leave
    ret