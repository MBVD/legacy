section .text
create_matrix: ;double** matrix(int n)
    enter 24, 0
    mov [rbp - 8], rdi; n
    imul rdi, rdi, 8; size_of address
    ALIGN_STACK
    call malloc
    UNALIGN_STACK
    mov [rbp - 16], rax; **matrix
    mov qword[rbp - 24], 0; i
.for:
    mov rcx, [rbp - 24]
    cmp rcx, qword[rbp - 8]
    jge .end_for
    mov rdi, [rbp - 8]; n
    imul rdi, rdi, 8; n * size of
    ALIGN_STACK
    call malloc
    UNALIGN_STACK
    mov rdi, [rbp - 16]
    mov rcx, [rbp - 24]
    mov [rdi + 8 * rcx], rax; matrix[i] = rax
    inc qword[rbp - 24]
    jmp .for
.end_for:
    mov rax, [rbp - 16]; matrix
.exit:
    leave
    ret
