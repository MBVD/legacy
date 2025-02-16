%include "io64.inc"
section .bss
    brk resq 0            ; Текущий адрес вершины кучи
section .data
    size dq 0 
    brk_size dq 0
head:
    dq 0; used
    dq 0; adress
    dq 0; size
    dq 0; next
tail:
    dq 0
    dq 0 
    dq 0
    dq 0
section .text
global main

main:
    mov rbp, rsp; for correct debugging
    lea rdi, [brk]
    mov rax, [rdi + 8]
    mov rax, [rdi + 16]
    mov qword[rdi + 16], 1 
    PRINT_DEC 8, [rdi + 16]
