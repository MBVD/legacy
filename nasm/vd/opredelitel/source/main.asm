%include "io64.inc"
%include "macro.inc"
%include "calculate_det.inc"
%include "create_matrix.inc"
%include "get_matrix.inc"
section .data
    printf_string db "%g", 0, 0
section .text
extern malloc
extern scanf
extern printf
global main
main:
    enter 16, 0
    GET_DEC 8, rax
    mov [rbp - 8], rax
    mov rdi, [rbp - 8]
    ALIGN_STACK
    call create_matrix
    UNALIGN_STACK
    PRINT_STRING "good creation"
    NEWLINE
    mov [rbp - 16], rax; float** matrix
    mov rdi, [rbp - 16]
    mov rsi, [rbp - 8]
    ALIGN_STACK
    call get_matrix
    UNALIGN_STACK
    NEWLINE
    mov rdi, [rbp - 16]
    mov rsi, [rbp - 8]
    ALIGN_STACK
    call calculate_det
    UNALIGN_STACK
    
    lea rdi, [printf_string]
    mov rax, 1
    ALIGN_STACK
    call printf
    UNALIGN_STACK
    leave
    ret