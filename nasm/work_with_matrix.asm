%include "io64.inc"

section .bss
    A resq 256
    A_rows resq 16
    m resq 1
    n resq 1

section .text
global main
main:
    GET_UDEC 8, [m]
    GET_UDEC 8, [n]
    lea rdi, [A]
    lea rsi, [A_rows]
    push qword[n]
    push qword[m]
    push rsi
    push rdi
    call init_matrix
    add rsp, 32
    
    lea rdi, [A_rows]
    push qword[n]
    push qword[m]
    push rdi
    call read_matrix
    
    add rsp, 24
    
    lea rdi, [A_rows]
    push qword[n]
    push qword[m]
    push rdi
    call print_matrix
    
    add rsp, 24
    
    

    ret
;void init_matrix(A, A_rows, m, n)
init_matrix:
    enter 8, 0
    
    mov rax, 8
    imul rax, [rbp + 40] 
    mov [rbp - 8], rax ; 8 * n
    
    mov rdi, [rbp + 16] ;address of A
    mov rsi, [rbp + 24] ;address of A_rows
    
    xor rcx,rcx
.cycle:
    cmp rcx, [rbp + 32] ;i < m
    jnl .end_cycle
    mov [rsi + 8 * rcx], rdi
    add rdi, [rbp - 8]
    inc rcx
    jmp .cycle   
    
.end_cycle:
    leave
    ret
    
read_matrix:
    enter 0,0
    
    mov rdi, [rbp + 16] ; A_rows
    xor rcx,rcx
.main_cycle:
    cmp rcx, [rbp + 24] ;i< m rsi, [rdi + rcx
    jnl .end_main_cycle
    
    
   mov rsi, [rdi + rcx *8]; rsi = A_rows[i]
   xor rdx, rdx
.cycle:
    cmp rdx, [rbp + 32] ; j < n
    jnl .end_cycle
    
    GET_UDEC 8, [rsi + 8 *rdx]; A[i][j]
    inc rdx
    jmp .cycle
.end_cycle:
    inc rcx
    jmp .main_cycle

.end_main_cycle:
    leave
    ret
    
print_matrix:
    enter 0,0
    
    mov rdi, [rbp + 16] ; A_rows
    xor rcx,rcx
    
.main_cycle:
    cmp rcx, [rbp + 24] ;i< m
    jnl .end_main_cycle
    
    
   mov rsi, [rdi + rcx *8]; rsi = A_rows[i]
   xor rdx, rdx
.cycle:
    cmp rdx, [rbp + 32] ; j < n
    jnl .end_cycle
    
    PRINT_DEC 8, [rsi + 8 *rdx]; A[i][j]
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
    
    
;void sum_of_matrix(A_rows, B_rous, C_rows, m , n)
sum_of_matrix:
    enter 0, 0
    mov rdi, [rbp + 16]
    mov rsi, [rbp + 24]
    mov rbx, [rbp + 32]
    xor rdx, rdx
.main_cycle:
    cmp rcx, [rbp + 24] ;i< m
    jnl .end_main_cycle
    
    
   mov r8, [rdi + rcx *8];  A_rows[i]
   mov r9, [rsi + rcx *8];  B_rows[i]
   mov r10, [rbx + rcx *8];  C_rows[i]
   xor rdx, rdx
.cycle:
    cmp rdx, [rbp + 32] ; j < n
    jnl .end_cycle
    
    ;PRINT_DEC 8, [rsi + 8 *rdx]; A[i][j]
    mov rax, [rdi + 8 * rdx]
    add rax, [rsi + 8 * rdx]
    mov [rbx + 8 * rdx], rax
    inc rdx
    jmp .cycle
.end_cycle:
    inc rcx
    jmp .main_cycle
.end_main_cycle:
    leave
    ret
    
    
    
    
    

