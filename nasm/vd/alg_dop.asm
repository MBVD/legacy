%include "io64.inc"

section .bss
    a resq 1000
    a_rows resq 1000
    b resq 1000
    b_rows resq 1000
    ans resq 1000
    ans_rows resq 1000
section .data
    n dq 0
    m dq 0
    i dq 0 
    j dq 0

section .text
global main

;void init_matrix(A, A_rows, n, m)
init_matrix:
    enter 8, 0
    
    mov rax, 8
    imul rax, [rbp + 40]
    mov [rbp - 8], rax ; 8 * n
    
    mov rdi, [rbp + 16]; address of A 
    mov rsi, [rbp + 24]; address of A_rows
    xor rcx, rcx
.cycle:
    cmp rcx, [rbp + 32]; i < m
    jnl .end_cycle
    mov [rsi + 8 * rcx], rdi
    add rdi, [rbp - 8]
    inc rcx
    jmp .cycle
.end_cycle:
    leave
    ret
    
   
read_matrix:
    enter 0, 0
    
    mov rdi, [rbp + 16]; A_rows
    xor rcx, rcx
.main_cycle:
    cmp rcx, [rbp + 24]; i < m
    jnl .end_main_cycle
    
    mov rsi, [rdi + rcx * 8]; rsi = A_rows[i]
    xor rdx, rdx
.cycle:
    cmp rdx, [rbp + 32]; j < n
    jnl .end_cycle
    
    GET_DEC 8, [rsi + 8 * rdx]; A[i][j]
    inc rdx
    jmp .cycle
.end_cycle:
    inc rcx
    jmp .main_cycle
.end_main_cycle:
    leave
    ret
    
print_matrix:
    enter 0, 0
    
    mov rdi, [rbp + 16]; A_rows
    xor rcx, rcx
.main_cycle:
    cmp rcx, [rbp + 24]; i < m
    jnl .end_main_cycle
    
    mov rsi, [rdi + rcx * 8]; rsi = A_rows[i]
    xor rdx, rdx
.cycle:
    cmp rdx, [rbp + 32]; j < n
    jnl .end_cycle
    
    PRINT_DEC 8, [rsi + 8 * rdx]; A[i][j]
    PRINT_CHAR ' '
    inc rdx
    jmp .cycle
.end_cycle:
    NEWLINE
    inc rcx
    jmp .main_cycle
.end_main_cycle:
    leave
    ret
     
alg_matrix: ; int alg_matrix(int**a, int n, int i, int j)
    enter 0, 0
    xor r8, r8
    xor r9, r9
    ; инициализация массива ответа
    mov rdx, qword[rbp + 24]
    dec rdx 
    push rdx
    push rdx
    lea rdi, [ans]
    lea rsi, [ans_rows]
    push rsi
    push rdi
    call init_matrix
    add rsp, 32
    ; инициализация массива ответа
    mov r15, [rbp+16]
    xor rcx, rcx
    xor r9, r9
.for:
    cmp rcx, qword[rbp + 24]
    jge .end_for
    cmp rcx, qword[rbp + 32]
    je .continue 
    xor rdx, rdx
    xor r9, r9
.inner_for:
    cmp rdx, qword[rbp + 24]
    jge .end_inner_for
    cmp rdx, qword[rbp + 40]
    je .continue1
    mov rsi, [ans_rows + 8 * r8]; buf[index1]
    mov rdi, [r15 + 8 * rcx]; a[k]
    mov rdi, [rdi + 8 * rdx]; a[k][l]
    mov [rsi + 8 * r9], rdi
    inc r9
.continue1:
    inc rdx
    jmp .inner_for
.end_inner_for:
    inc r8
.continue:
    inc rcx
    jmp .for
.end_for:
    mov rax, [ans_rows]
    leave
    ret

main:
    mov rbp, rsp; for correct debugging
    lea rdi, [a]
    lea rsi, [a_rows]
    GET_DEC 8, [n]
    push qword[n]
    push qword[n]
    push rsi
    push rdi
    call init_matrix
    
    
    lea rsi, [a_rows]
    add rsp, 32
    push qword[n]
    push qword[n]
    push rsi
    call read_matrix
    
    
    lea rsi, [a_rows]
    add rsp, 24
    push qword[n]
    push qword[n]
    push rsi
    call print_matrix
    add rsp, 24
    
    GET_DEC 8, [i]
    GET_DEC 8, [j]
    push qword[j]
    push qword[i]
    push qword[n]
    lea rdi, [a_rows]
    push rdi
    call alg_matrix
    add rsp, 32
    
    lea rsi, [ans_rows]
    mov rdx, qword[n]
    dec rdx
    push rdx
    push rdx
    push rsi
    call print_matrix
    add rsp, 24
    
    
    ret