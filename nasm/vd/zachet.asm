%include "io64.inc"
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

multiply:; float* (float** matrix, float* vector, int n)
    enter 48, 0
    mov [rbp - 8], rdi; matrix
    mov [rbp - 16], rsi; vector
    mov [rbp - 24], rdx; n
    mov qword[rbp - 32], 0; i
    mov rdi, [rbp - 24]
    imul rdi, rdi, 8
    ALIGN_STACK
    call malloc
    UNALIGN_STACK
    mov [rbp - 48], rax
.for:
    mov rcx, [rbp - 32]
    cmp rcx, [rbp - 24]
    jge .end_for
    mov qword[rbp - 40], 0; j
.inner_for:
    mov rdx, [rbp - 40]
    cmp rdx, [rbp - 24]
    jge .end_inner_for
    mov rdi, [rbp - 8]; matrix
    mov rcx, [rbp - 32] 
    mov rdi, [rdi + 8 * rcx]; matrix[i]
    vmovupd ymm1, [rdi + 8 * rdx]; matrix[i][4 * j]
    mov rdi, [rbp - 16]; vector
    vmovupd ymm2, [rdi + 8 * rdx]; vector[j * 4]
    vmulpd ymm1, ymm2, ymm1
    vhaddpd ymm1, ymm1, ymm1
    vhaddpd ymm1, ymm1, ymm1
    vextractf128 xmm1, ymm1, 1
    addsd xmm0, xmm1

    
    add qword[rbp - 40], 4
    jmp .inner_for
.end_inner_for:
    mov rdi, [rbp - 48]
    mov rcx, [rbp - 32]
    movsd [rdi + 8 * rcx], xmm0
    inc qword[rbp - 32]
    jmp .for
.end_for:
    mov rax, [rbp - 48]
    leave
    ret
            
scan_array:; scan_array(int n, float** array)
    enter 16, 0
    mov [rbp - 8], rdi; int n 
    mov [rbp - 16], rsi; float** array
    mov rsi, [rsi]; float* array
    xor rcx, rcx
.while:
    cmp rcx, qword[rbp - 8]
    jge .end_while
    lea rdi, [scanf_string]
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
    lea rdi, [printf_string]
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

usage_vector:
    enter 16, 0
    mov [rbp - 8], rdi; n
    imul rdi, rdi, 8
    ALIGN_STACK
    call malloc
    UNALIGN_STACK
    mov [rbp - 16], rax
    mov rdi, [rbp - 8]
    lea rsi, [rbp - 16]
    call scan_array
    mov rax, [rbp - 16]
    leave
    ret
    
    
    
create_matrix:; int** create_matrix(int n, int m)
    enter 32, 0
    mov [rbp - 8], rdi
    mov [rbp - 16], rsi
    imul rdi, rdi, 8; n * 8
    ALIGN_STACK
    call malloc
    UNALIGN_STACK
    mov [rbp - 24], rax; int** ans
    mov qword[rbp - 32], 0
.for:
    mov rdx, [rbp - 32]
    cmp rdx, [rbp - 8]
    jge .end_for
    mov rdi, [rbp - 24]
    mov rdi, [rbp - 16]; m
    imul rdi, rdi, 8; m * 8
    ALIGN_STACK
    call malloc
    UNALIGN_STACK
    mov rdx, [rbp - 32]
    mov rdi, [rbp - 24]
    mov [rdi + 8 * rdx], rax; ans[i] = malloc(sizeof(long long) * m)
    inc qword[rbp - 32]
    jmp .for
.end_for:
    mov rax, [rbp - 24]
    leave
    ret


get_matrix: ;void get_matrix(int** matrix, int n, int m)
    enter 40, 0
    mov [rbp - 8], rdi; matrix
    mov [rbp - 16], rsi; n
    mov [rbp - 24], rdx; m
    mov qword[rbp - 32], 0; i
.for:
    mov rcx, [rbp - 32]
    cmp rcx, [rbp - 16]
    jge .end_for
    mov qword[rbp - 40], 0; j = 0
.inner_for: 
    mov rdx, [rbp - 40]
    cmp rdx, [rbp - 24]
    jge .end_inner_for
    mov rdi, [rbp - 8]
    mov rdi, [rdi + 8 * rcx]

    lea rsi, [rdi + 8 * rdx]
    lea rdi, [scanf_string]
    push rcx
    ALIGN_STACK
    call scanf
    UNALIGN_STACK
    pop rcx

    inc qword[rbp - 40]
    jmp .inner_for
.end_inner_for:
    inc qword[rbp - 32]
    jmp .for
.end_for:
    leave
    ret

print_matrix:; void print_matrix(int** matrix, n, m)
    enter 40, 0
    mov [rbp - 8], rdi; matrix
    mov [rbp - 16], rsi; n
    mov [rbp - 24], rdx; m
    mov qword[rbp - 32], 0
.for:
    mov rcx, [rbp - 32]
    cmp rcx, [rbp - 16]
    jge .end_for
    mov qword[rbp - 40], 0; j
.inner_for:
    mov rdx, [rbp - 40]
    cmp rdx, [rbp - 24]
    jge .end_inner_for
    mov rdi, [rbp - 8]
    mov rdi, [rdi + 8 * rcx]
    movsd xmm0, [rdi + 8 * rdx]
    lea rdi, [printf_string]
    push rcx
    ALIGN_STACK
    call printf
    UNALIGN_STACK
    pop rcx
    PRINT_CHAR ' '
    inc qword[rbp - 40]
    jmp .inner_for
.end_inner_for: 
    NEWLINE
    inc qword[rbp - 32]
    jmp .for
.end_for:   
    leave
    ret


usage:; int** usage()
    enter 24, 0
    mov [rbp - 8], rdi
    mov rsi, [rbp - 8]
    call create_matrix
    mov [rbp - 24], rax
    mov rdi, [rbp - 24]
    mov rsi, [rbp - 8]
    mov rdx, [rbp - 8]
    call get_matrix
    mov rax, [rbp - 24]
    leave
    ret
    
    
