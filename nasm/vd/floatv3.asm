%include "io64.inc"

section .data
    print f_format db "%d %g, %d, %g", 10, 0
    x dd 5
    y dq 3.14
    z dq 3
    w dq 2.7
    
section .text

extern printf

global CMAIN

CMAIN:
    enter 0, 0
    
    lea rdi, [printf_format]
    xor rsi, rsi
    mov esi, [x]
    
    movsd xmm0, [y]
    xor rdx, rdx
    mov edx, [z]
    movsd xmm1, [w]
    mov rax 2
    call printf
    
    xor rax, rax
    leave
    ret