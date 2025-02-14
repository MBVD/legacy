%include "io64.inc"
%include "matrix.inc"

section .text
global main
main:
    xor rax, rax
    call usage
    ret