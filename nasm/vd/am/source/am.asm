%include "io64.inc"
%include "macro.inc"
section .data
    scanf_str db "%lf", 0
    printf_str db "%g", 0 0
section .text

extern printf
extern scanf
extern malloc
global main

scan_array:; scan_array(int n, float** array)
    enter 16, 0
    mov [rbp - 8], rdi; int n 
    mov [rbp - 16], rsi; float** array
    mov rsi, [rsi]; float* array
    xor rcx, rcx
.while:
    cmp rcx, qword[rbp - 8]
    jge .end_while
    lea rdi, [scanf_str]
    lea rsi, [rsi + 8 * rcx]
    push rcx
    ALIGN_STACK
    call scanf
    UNALIGN_STACK
    pop rcx
    ;PRINT_STRING "SUCCESS"
    ;NEWLINE
    mov rsi, [rbp - 16]; float ** array
    mov rsi, [rsi]; float * array
    inc rcx
    ;PRINT_DEC 8, rcx
    ;NEWLINE 
    jmp .while
.end_while:
    xor rax, rax
    leave
    ret

am: ;float am(float* arr, n)
    enter 32, 0
    mov [rbp - 8], rdi; array
    mov [rbp - 16], rsi; n
    mov qword[rbp - 24], 0; ans
    mov qword[rbp - 32], 0; i
    xor rcx, rcx
.while:
    mov rcx, [rbp - 32]
    cmp rcx, qword[rbp - 16]
    jge .end_while
    mov rdi, [rbp - 8]; array
    movsd xmm0, [rbp - 24]
    movsd xmm1, [rdi + 8 * rcx]
    addsd xmm0, xmm1

    ;lea rdi, [printf_str]
    ;mov rax, 1
    ;ALIGN_STACK
    ;call printf
    ;UNALIGN_STACK
    ;PRINT_CHAR ' '
    ;mov rdi, [rbp - 8]
    
    movsd [rbp - 24], xmm0
    inc qword[rbp - 32]
    jmp .while
.end_while:
    movsd xmm0, [rbp - 24]
    mov rax, [rbp - 16]
    cvtsi2sd xmm1, rax
    divsd xmm0, xmm1
    leave
    ret

print_array: ;print_array(float* array, int n)
    enter 24, 0
    mov [rbp - 8], rdi; array
    mov [rbp - 16], rsi; n
    mov qword[rbp - 24], 0
    xor rcx, rcx
.while:
    mov rcx, [rbp - 24]
    cmp rcx, qword[rbp - 16]
    jge .end_while
    mov rdi, [rbp - 8]; array
    movsd xmm0, [rdi + 8 * rcx]
    lea rdi, [printf_str]
    mov rax, 1
    ALIGN_STACK
    call printf
    UNALIGN_STACK
    PRINT_CHAR ' '
    inc qword[rbp - 24]
    jmp .while
.end_while:
    NEWLINE
    leave
    ret

main:
    enter 16, 0
    PRINT_STRING "n: "
    GET_DEC 8, rdi
    mov [rbp - 8], rdi; n
    imul rdi, rdi, 8
    ALIGN_STACK
    call malloc
    UNALIGN_STACK
    mov [rbp - 16], rax
    mov rdi, [rbp - 8]
    lea rsi, [rbp - 16]
    call scan_array

    ;mov rax, [rbp - 16]
    ;movsd xmm0, [rax]; array
    ;lea rdi, [printf_str]
    ;mov rax, 1
    ;ALIGN_STACK
    ;call printf
    ;UNALIGN_STACK

    mov rdi, [rbp - 16]
    mov rsi, [rbp - 8]
    call print_array

    mov rdi, [rbp - 16]
    mov rsi, [rbp - 8]
    PRINT_STRING "AM array: "
    call am
    lea rdi, [printf_str]
    mov rax, 1
    ALIGN_STACK
    call printf
    UNALIGN_STACK
    NEWLINE
    leave
    ret