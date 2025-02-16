%include "io64.inc"

section .bss
    A resq 256
    A_rows resq 16
    M resq 256
    M_rows resq 16
    m resq 1
    n resq 1 
    row resq 1
    col resq 1
    size resq 1

section .text
global CMAIN
CMAIN:
    mov rbp, rsp; for correct debugging
    
    GET_UDEC 8, [m]
    GET_UDEC 8, [n]
    GET_UDEC 8, [row]
    GET_UDEC 8, [col]
    GET_UDEC 8, [size]
    
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

    lea rdi, [M]
    lea rsi, [M_rows]
    push qword[n]
    push qword[m]
    push rsi
    push rdi
    call init_matrix
    add rsp, 32
    
    ; [rbp + 16] - A_rows
; [rbp + 24] - n
; [rbp + 32] - row
; [rbp + 40] - col
; [rbp + 48] - M_rows
    
    lea rdi, [A_rows]
    lea rsi, [M_rows]
    push rsi
    push qword[col]
    push qword[row]
    push qword[n]
    push rdi
    call minor
    add rsp, 40
    
    lea rdi, [M_rows]
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
; void f(int **A, int **B, int **C, int m, int n, int q)        
multiply_matrix:
    enter 0, 0
    mov rdi, [rbp + 16] ; A_rows
    mov rsi, [rbp + 24] ; B_rows
    mov rbx, [rbp + 32] ; C_rows
    xor rcx, rcx ; i
.for_i:
    cmp rcx, [rbp + 40] ; i < m
    jnl .end_for_i
    xor r8, r8 ; j
.for_j:
    cmp r8, [rbp + 56] ; j < q
    jnl .end_for_j
    mov r9, qword[rbx + 8 * rcx] ; C_rows[i]
    mov qword[r9 + 8 * r8], 0 ; C_rows[i][j]
    xor r10, r10 ; k
.for_k:
    cmp r10, [rbp + 48] ; k < n
    jnl .end_for_k
    mov r11, qword[rdi + 8 * rcx] ; A_rows[i]
    mov rax, qword[r11 + 8 * r10] ;A_rows[i][k]
    mov r12, qword[rsi + 8 * r10] ; B_rows[k]
    mov rdx, qword[r12 + 8 * r8] ; B_rows[k][j]
    imul rdx
    add qword[r9 + 8 * r8], rax
    inc r10
    jmp .for_k
.end_for_k:
    inc r8
    jmp .for_j
.end_for_i:
    leave
    ret
.end_for_j:
    inc rcx
    jmp .for_i
    
; void transpose(int A_rows, int m)    
transpone_matrix:
    enter 0, 0
    mov rdi, [rbp + 16] ; A_rows
    xor rcx, rcx ; i
.for_i:
    cmp rcx, [rbp + 24] ; i < m
    jnl .end_for_i
    mov rsi, rcx ;j
    add rsi, 1
.for_j:
    cmp rsi, [rbp + 24] ; j < m
    jnl .end_for_j
    mov r8, qword[rdi + 8 * rcx] ; A_rows[i]
    mov r9, qword[r8 + 8 * rsi] ; A_rows[i][j] ; temp
    mov r10,qword[rdi + 8 * rsi]
    mov r11, qword[r10 + 8 * rcx] ; A_rows[j][i]
    mov qword[r8 + 8 * rsi], r11
    mov qword[r10 + 8 * rcx], r9
    inc rsi
    jmp .for_j

.end_for_j:
    inc rcx
    jmp .for_i
.end_for_i:
    leave
    ret
    
; A_rows
; void Minor(matrix, size, minor, row, col)
; [rbp + 16] - A_rows
; [rbp + 24] - size
; [rbp + 32] - row
; [rbp + 40] - col
; [rbp + 48] - M_rows
minor:
    enter 0, 0
    mov rdi, [rbp + 16] ; A_rows
    mov rsi, [rbp + 48] ; M_rows
    xor rcx, rcx ; i 
    xor rbx, rbx ; j
    xor r8, r8 ; x
    xor r11, r11 ; y
.for:
    cmp r8, [rbp + 24] ; x < n
    jnl .end_for
    cmp r8, [rbp + 32] ; x != row
    je .inc_rcx
    xor rbx, rbx ; j
.for2:
    cmp r11, [rbp + 24] ; y < n
    jnl .inc_rcx
    cmp r11, [rbp + 40] ; y != col
    je .inc_r11
    mov r9, [rdi + 8 * r8] ; A_rows[x]
    mov r10, [r9 + 8 * r11] ; A_rows[x][y]
    mov r12, [rsi + 8 * rcx] ; M_rows[i]
    mov [r12 + 8 * rbx], r10 ; M_rows[i][j] = A_rows[x][y]
    inc rbx
    inc r11
    jmp .for2
.inc_r11:
    inc r11
    jmp .for2
.inc_rcx:
    inc rcx
    inc r8
    jmp .for    
.end_for:
    leave
    ret
alg_ext:
    