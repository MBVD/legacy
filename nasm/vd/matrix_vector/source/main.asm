%include "io64.inc"
%include "vector.inc"
%include "multiply.inc"
%include "matrix.inc"
section .data
    scanf_string db "%lf", 0
    printf_string db "%g", 0
section .text
extern malloc
extern scanf
extern printf
global main

main:
    mov rbp, rsp; for correct debugging
    enter 24, 0
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
    
    mov rdi, [rbp - 16]
    mov rsi, [rbp - 24]
    mov rdx, [rbp - 8]
    call multiply
    mov rdi, rax
    mov rsi, [rbp - 8]
    PRINT_STRING "Our answer"
    NEWLINE 
    call print_array
    
    leave
    ret