%include "list.inc"
%include "io64.inc"

section .data
    null_ptr equ 0
section .bss

section .text
global main
main:
    enter 24, 0
    mov qword[rbp - 8], null_ptr; head
    lea rdi, [rbp - 8]
    GET_DEC 8, rsi
    call get_list
    mov rdi, rax
    call print_list
    leave
    ret