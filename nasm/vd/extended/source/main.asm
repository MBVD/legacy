%include "matrix.inc"
%include "io64.inc"
%include "vector.inc"
%include "solution.inc"


section .data

section .bss

section .text
global main
main:
    enter 24, 0
    GET_DEC 8, rdi
    mov [rbp - 16], rdi; n
    call usage
    mov [rbp - 8], rax; int** matrix
    mov rdi, [rbp - 8]
    mov rsi, [rbp - 16]
    mov rdx, [rbp - 16]
    PRINT_STRING "matrix:"
    NEWLINE
    call print_matrix
    mov rdi, [rbp - 16]; n
    call usage_vector
    mov [rbp - 24], rax; vector ans
    mov rdi, [rbp - 24]; vector ans
    mov rsi, [rbp - 16]
    PRINT_STRING "vector:"
    NEWLINE
    call print_array

    mov rdi, [rbp - 8]; matrix
    mov rsi, [rbp - 24]; ans
    mov rdx, [rbp - 16]; n
    call solution
    mov rdi, rax; otvet
    mov rsi, [rbp - 16]; n
    call print_array
    leave
    ret