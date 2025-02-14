%include "tree.inc"

section .text

global main

main:

    enter 8, 0

    mov qword[rbp - 8], nullptr

    lea rdi, [rbp - 8]
    mov rsi, nullptr
    mov rdx, 5
    call insert

    lea rdi, [rbp - 8]
    mov rsi, nullptr
    mov rdx, 3
    call insert

    lea rdi, [rbp - 8]
    mov rsi, nullptr
    mov rdx, 7
    call insert

    mov rdi, [rbp - 8]
    call print_tree

    xor rax, rax

    leave
    ret