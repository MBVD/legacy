%include "io64.inc"
section .data
section .text

global main
main:
    mov rbp, rsp; for correct debugging
    GET_DEC 8, rax
    xor rcx, rcx
cycle:
    test rax, rax
    jz end
    shr rax, 1
    inc rcx
    jmp cycle
end:
    PRINT_DEC 8, rcx
    ret
    