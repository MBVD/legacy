%include "macro.inc"

section .data
    frmt db "Hello %s %d", 10, 0
    string db "world", 0

section .text

global main

main:
    enter 16, 0

    ;mov qword[rbp - 8], nullptr
    ;mov qword[rbp - 16], nullptr

    ;lea rdi, [rbp - 8]
    ;lea rsi, [rbp - 16]
    ;lea rdx, [arr]
    ;mov rcx, [n]
    ;call push_back_array

    ;mov rdi, [rbp - 8]
    ;call print_list

    PRINTF frmt, string, 65

    xor rax, rax
    leave
    ret