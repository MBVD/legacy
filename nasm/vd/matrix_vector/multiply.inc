section .text
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
    vextractf128 xmm0, ymm1, 1
    vaddsd xmm3, xmm0, xmm1

    
    add qword[rbp - 40], 4
    jmp .inner_for
.end_inner_for:
    mov rdi, [rbp - 48]
    mov rcx, [rbp - 32]
    movsd [rdi + 8 * rcx], xmm3
    inc qword[rbp - 32]
    jmp .for
.end_for:
    mov rax, [rbp - 48]
    leave
    ret