section .text
calculate_det: ;float calculate_det(float** matrix, int n)
    enter 40, 0
    mov [rbp - 8], rdi; matrix
    mov [rbp - 16], rdi; int n
    mov qword[rbp - 24], 0; int i
    mov qword[rbp - 32], 0; float ans = 0
.for:
    mov rcx, [rbp - 24]
    cmp rcx, [rbp - 16]
    jge .end_for
    mov qword[rbp - 40], 0; int j = 0
.inner_for:
    mov rdx, [rbp - 40]
    cmp rdx, [rbp - 16]
    jge .end_inner_for
    mov rcx,  [rbp - 24]
    mov rdx, [rbp - 40]; j
    cmp rcx,rdx
    jne .ignore

    mov rdi, [rbp - 8]
    mov rdi, [rdi + 8 * rcx]; matrix[i]
    movsd xmm0, [rbp - 32]; ans
    movsd xmm1, [rdi + 8 * rdx]; matrix[i][j]
    mulsd xmm0, xmm1
    movsd [rbp - 32], xmm0
.ignore:
    inc qword[rbp - 40]    
    jmp .inner_for
.end_inner_for:
    inc qword[rbp - 24]
    jmp .for
.end_for:
    movsd xmm0, [rbp - 32]
.exit:
    leave
    ret
