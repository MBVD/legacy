section .data
    printf_str2 db "%g %g", 0, 0
section .text
extern malloc
solution: ;int*(int** matrix, int* ans, n)
    enter 56, 0
    mov [rbp - 8], rdi; matrix
    mov [rbp - 16], rsi; ans
    mov [rbp - 24], rdx; n
    mov rdi, [rbp - 24]
    imul rdi, rdi, 8
    ALIGN_STACK
    call malloc
    UNALIGN_STACK
    mov [rbp - 32], rax; a
    mov qword[rbp - 40], 0; i
.for:
    mov rcx, [rbp - 40]; i
    cmp rcx, [rbp - 24]; i < n
    jge .end_for
    mov rax, 0
    cvtsi2sd xmm0, rax
    movsd [rbp - 48], xmm0; tmp = 0
    mov qword[rbp - 56], 0
.inner_for:
    mov rdx, [rbp - 56]; j
    cmp rdx, [rbp - 40]; j<i
    jge .end_inner_for
    mov rdi, [rbp - 8];
    mov rdi, [rdi + 8 * rcx]
    movsd xmm0, [rdi + 8 * rdx]
    mov rdi, [rbp - 32]; a
    movsd xmm1, [rdi + 8 * rdx]; a[j]
    mulsd xmm0, xmm1
    movsd xmm1, [rbp - 48]
    addsd xmm0, xmm1
    movsd [rbp - 48], xmm0
    inc qword[rbp - 56]
    jmp .inner_for
.end_inner_for:
    mov rcx, [rbp - 40]
    mov rdi, [rbp - 16]; ans
    movsd xmm2, [rdi + 8 * rcx]; ans[i]
    movsd xmm3, [rbp - 48]; tmp
    subsd xmm2, xmm3
    mov rdi, [rbp - 8]
    mov rdi, [rdi + 8 * rcx]
    movsd xmm4, [rdi + 8 * rcx]
    divsd xmm2, xmm4
    mov rdi, [rbp - 32]; a
    movsd [rdi + 8 * rcx], xmm2
    inc qword[rbp - 40]
    jmp .for    
.end_for:
    mov rax, [rbp - 32]
    leave 
    ret