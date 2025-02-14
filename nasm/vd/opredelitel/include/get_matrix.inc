section .data
    scanf_string db "%lf", 0
section .text
get_matrix: ;void get_matrix(float** matrix, int n)
    enter 40, 0
    mov [rbp - 8], rdi; matrix
    mov [rbp - 16], rsi; n
    mov qword[rbp - 24], 0; i
.for:
    mov rcx, [rbp - 24]
    cmp rcx, [rbp - 16]
    jge .end_for
    mov qword[rbp - 32], 0; j
.inner_for:
    mov rdx, [rbp - 32]
    cmp rdx, [rbp - 16]
    jge .end_inner_for
    mov rdi, [rbp - 8]; matrix
    mov rcx, [rbp - 24]; i
    mov rdx, [rbp - 32]; j
    mov rdi, [rdi + 8 * rcx]; matrix[i]
    PRINT_DEC 8, rdi
    PRINT_CHAR ' '
    PRINT_DEC 8, rcx
    NEWLINE
    mov qword[rdi + 8 * rdx], 0
    mov qword[rbp - 40], 0; 
    lea rsi, [rbp - 40]
    PRINT_DEC 8, rsi
    PRINT_CHAR ' '
    PRINT_DEC 8, rdx
    NEWLINE
    movsd xmm0, [rbp - 40]
    movsd [rdi + 8 * rdx], xmm0; matrix[i][j] = xmm0 

    lea rdi, [scanf_string]
    mov rax, 1
    ALIGN_STACK
    call scanf
    UNALIGN_STACK
    PRINT_STRING "good scanf"
    NEWLINE

    mov rcx, [rbp - 24]; i
    mov rdx, [rbp - 32]; j
    mov rdi, [rbp - 8]
    mov rdi, [rdi + 8 * rcx]; matrix[i]
    movsd xmm0, [rbp - 40]; matrix[i][j]
    lea rdi, [rdi + 8 * rdx]
    NEWLINE
    PRINT_DEC 8, rdi
    PRINT_STRING " calling printf"
    NEWLINE
    mov rax, 2
    lea rdi, [printf_string]
    PRINT_STRING [printf_string]
    ALIGN_STACK
    call printf
    UNALIGN_STACK
    NEWLINE
    

    inc qword[rbp - 32]
    jmp .inner_for
.end_inner_for: 
    inc qword[rbp - 24]
    jmp .for
.end_for:
    xor rax, rax
.exit:
    leave
    ret