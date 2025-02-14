%include "io64.inc"

section .bss
    A resq 256
    A_rows resq 16
    B resq 256
    B_rows resq 16
    C resq 256
    C_rows resq 16
    m resq 1
    n resq 1 

section .text
global CMAIN
CMAIN:
    mov rbp, rsp; for correct debugging
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

    lea rdi, [B]
    lea rsi, [B_rows]
    push qword[n]
    push qword[m]
    push rsi
    push rdi
    call init_matrix
    add rsp, 32

    lea rdi, [B_rows]
    push qword[n]
    push qword[m]
    push rdi
    call read_matrix
    add rsp, 24
    
    lea rdi, [C]
    lea rsi, [C_rows]
    push qword[n]
    push qword[m]
    push rsi
    push rdi
    call init_matrix
    add rsp, 32

    lea rdi, [A_rows]
    lea rsi, [B_rows]
    lea rbx, [C_rows]
    push qword[n]
    push qword[m]
    push rbx
    push rsi
    push rdi
    call sum_of_matrix
    add rsp, 40

    lea rdi, [C_rows]
    push qword[n]
    push qword[m]
    push rdi
    call print_matrix
    add rsp, 24
    
    push qword[n]
    lea rdi, [A_rows]
    push rdi
    call check_matrix
    add rsp, 16
    PRINT_DEC 8, rax

    ret
    

;void init_matrix(A, A_rows, m, n)
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
    
;void read_matrix(A_rows, m, n)
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


;void print_matrix(A_rows, m, n)
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

; void sum_of_matrix(A_rows, B_rows, C_rows, m, n)        
sum_of_matrix:
    enter 0, 0
    mov rdi, [rbp + 16] ; A_rows
    mov rsi, [rbp + 24] ; B_rows
    mov rbx, [rbp + 32] ; C_rows
    xor rcx, rcx
.main_cycle:
    cmp rcx, [rbp + 40]; i < m
    jnl .end_main_cycle
    
    mov r8, [rdi + 8 * rcx] ; A_rows[i]
    mov r9, [rsi + 8 * rcx] ; B_rows[i]
    mov r10, [rbx + 8 * rcx] ; C_rows[i]
    
    xor rdx, rdx
.cycle:
    cmp rdx, [rbp + 48]; j < n
    jnl .end_cycle
    
    mov rax, [r8 + 8 * rdx]
    add rax, [r9 + 8 * rdx]
    mov [r10 + 8 * rdx], rax
    inc rdx
    jmp .cycle
.end_cycle:
    
    inc rcx
    jmp .main_cycle
.end_main_cycle:
    leave 
    ret

;int check_matrix(natrix_rows, n)
check_matrix:
    enter 0, 0
    mov rax, 1; flag = 1
    mov rdi, [rbp + 16]; A_rows
    xor rcx, rcx
.main_cycle:
    cmp rcx, [rbp + 24]; i < m
    jnl .end_main_cycle
    
    mov rsi, [rdi + rcx * 8]; rsi = A_rows[i]
    mov rdx, rcx
    inc rdx
.cycle:
    cmp rdx, [rbp + 24]; j < n
    jnl .end_cycle
    mov r8, [rdi + rdx * 8]; a[j]
    mov r9, [r8 + 8 * rcx]; a[j][i]
    cmp [rsi + 8 *rdx], r9; a[i][j] ? a[j][i]
    je .true
    mov rax, 0
    jmp .end_main_cycle
.true:
    inc rdx
    jmp .cycle
.end_cycle:
    inc rcx
    jmp .main_cycle
.end_main_cycle:
    leave
    ret
        
