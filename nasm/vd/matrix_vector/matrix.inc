section .text
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
    