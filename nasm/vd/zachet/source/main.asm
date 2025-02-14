%include "io64.inc"
%include "matrix.inc"
%include "vector.inc"

section .data

section .bss

section .text
global main

main:
    mov rbp, rsp; for correct debugging
    enter 16, 0
    GET_DEC 8, rdi
    mov [rbp - 8], rdi; n
    call usage
    mov [rbp - 16], rax; matrix
    mov rdi, [rbp - 16]
    mov rsi, [rbp - 8]
    mov rdx , [rbp - 8]
    
    PRINT_STRING "OUR MATRIX"
    NEWLINE

    call print_matrix

    mov rdi, [rbp - 8]
    call usage_vector
    mov [rbp - 24], rax; vector
    mov rdi, [rbp - 24]
    mov rsi, [rbp - 8]
    PRINT_STRING "our vector"
    NEWLINE
    call print_array

    leave
    ret