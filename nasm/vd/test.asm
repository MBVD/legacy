%include "io64.inc"

section .text
global main
main:
    ;write your code here
    xor rax, rax
    GET_DEC 8, rax
    idiv rax, 8
    and rax, 9
    PRINT_DEC 8, rax
    ret