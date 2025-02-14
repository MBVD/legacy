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

mull_array:
    enter 0, 0
    mov rdi, [rbp + 16];a_rows
    mov rsi, [rbp + 24];b_rows
    mov rdx, [rbp + 32];n
    mov rbx, [rbp + 40];m
    push rbx
    push rdx
    lea r10, [ans_rows]
    push r10
    lea r10, [ans]
    push r10
    call init_matrix
    xor rcx, rcx
    add rsp, 32
    mov rdi, [rbp + 16];a_rows
    mov rsi, [rbp + 24];b_rows 
    mov rdx, [rbp + 32];n
    mov rbx, [rbp + 40];m
.loop:
    cmp rcx, rdx
    jge .end_loop
    xor r8, r8
.inner_loop1:
    cmp r8, rdx
    jge .end_inner_loop1
    xor r9, r9   
.inner_loop2:
    cmp r9, rbx
    jge .end_inner_loop2    
    mov r11, [rdi + 8 * rcx]
    mov r12, [r11 + 8 * r9] ; a[i][k]
    mov r11, r12
    
    mov r12, [rsi + 8 * r9]
    mov r13, [r12 + 8 * r8] ; b[k][j]
    mov r12, r13
    lea r15, [ans_rows] 
    mov r10, [r15 + 8 * rcx] ;ans[i]
    mov rax, r11
    mov r15, rdx
    mul r12
    mov rdx, r15
    add [r10 + 8 * r8], rax
    
    
    inc r9   
    jmp .inner_loop2    
.end_inner_loop2:
    inc r8
    jmp .inner_loop1
.end_inner_loop1:
    inc rcx
    jmp .loop
.end_loop:
    lea rax, [ans]
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
     
main:
    mov rbp, rsp; for correct debugging
    lea rdi, [a]
    lea rsi, [a_rows]
    GET_DEC 8, [n]
    GET_DEC 8, [m]
    push qword[m]
    push qword[n]
    push rsi
    push rdi
    call init_matrix
    lea rsi, [a_rows]
    add rsp, 32
    push qword[m]
    push qword[n]
    push rsi
    call read_matrix
    lea rsi, [a_rows]
    add rsp, 24
    push qword[m]
    push qword[n]
    push rsi
    call print_matrix
    add rsp, 24
    
    PRINT_CHAR `\n`
    
    lea rdi, [b]
    lea rsi, [b_rows]
    GET_DEC 8, [n]
    GET_DEC 8, [m]
    push qword[m]
    push qword[n]
    push rsi
    push rdi
    call init_matrix
    lea rsi, [b_rows]
    add rsp, 32
    push qword[m]
    push qword[n]
    push rsi
    call read_matrix
    lea rsi, [b_rows]
    add rsp, 24
    push qword[m]
    push qword[n]
    push rsi
    call print_matrix
    add rsp, 24
    
    push qword[n]
    push qword[m]
    lea rdi, [b_rows]
    push rdi
    lea rdi, [a_rows]
    push rdi
    call mull_array
    add rsp, 32
    
    PRINT_CHAR `\n`
    
    mov qword[n], 3
    lea rsi, [ans_rows]
    push qword[n]
    push qword[n]
    push rsi
    call print_matrix
    add rsp, 24
    
    ret