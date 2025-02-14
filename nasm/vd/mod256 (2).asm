%include "io64.inc"

section .data
section .text
global main
main:
    mov rbp, rsp; for correct debugging
    GET_DEC 8, rax
    mov rcx, rax
    shr rcx, 8
    shl rcx, 8
    xor rax, rcx 
    PRINT_DEC 8, rax
    ret