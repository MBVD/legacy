%include "io64.inc"
section .data
    arr db '1234985980', 0
section .text
    
global CMAIN
CMAIN:
    mov rax, 0
    mov rbx, 0 ;sum
    lea rsi, [arr]
cycle:
    cmp byte[rsi], `\0`
    je end_cycle
    imul rbx, 10
    lodsb
    sub al, '0'
    add rbx, rax
    jmp cycle
end_cycle:
    PRINT_DEC 8, rbx
    ret