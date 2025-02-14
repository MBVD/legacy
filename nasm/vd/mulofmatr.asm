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
    k resq 1

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

    lea rdi, [A_rows]
    push qword[m]
    push qword[n]
    push rdi
    call print_matrix
    add rsp, 24

    GET_UDEC 8, [k]
    lea rdi, [B]
    lea rsi, [B_rows]
    push qword[k]
    push qword[n]
    push rsi
    push rdi
    call init_matrix
    add rsp, 32

    lea rdi, [B_rows]
    push qword[k]
    push qword[n]
    push rdi
    call read_matrix
    add rsp, 24
    
    lea rdi, [B_rows]
    push qword[k]
    push qword[n]
    push rdi
    call print_matrix
    add rsp, 24
    
    lea rdi, [C]
    lea rsi, [C_rows]
    push qword[k]
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
    call product_of_matrix
    add rsp, 40

    lea rdi, [C_rows]
    push qword[m]
    push qword[k]
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
    
;void product_of_matrix(A_rows, A_columns, B_rows, B_columns, C_rows, C_columns, m, n, p)
product_of_matrix:
    enter 0, 0
    mov rdi, [rbp + 16] ; A_rows
    mov rsi, [rbp + 24] ; A_columns
    mov rdx, [rbp + 32] ; B_rows
    mov rbx, [rbp + 40] ; B_columns
    mov rbp, [rbp + 48] ; C_rows
    mov r10, [rbp + 56] ; C_columns
    mov r11, [rbp + 64] ; m
    mov r12, [rbp + 72] ; n
    mov r13, [rbp + 80] ; p

    xor rcx, rcx
.main_cycle:
    cmp rcx, r11 ; i < m
    jnl .end_main_cycle

    mov r8, [rdi + 8 * rcx] ; A_rows[i]

    xor rdx, rdx
.i_cycle:
    cmp rdx, r12 ; j < n
    jnl .end_i_cycle

    mov rax, 0

    xor r9, r9
.j_cycle:
    cmp r9, r13 ; k < p
    jnl .end_j_cycle

    mov r14, [r8 + 8 * rdx] ; A_rows[i][j]
    mov rax, 8 * r9 * 4  ; Вычисление промежуточного смещения
    mov r15, [rsi + 8 * rdx + rax] ; Использование rax как дополнительного смещения
 ; A_columns[j][k]
    imul r14, r15
    add rax, r14

    inc r9
    jmp .j_cycle
.end_j_cycle:

    mov rdi, rbp  ; Move base address (C rows) to rdi
    add rdi, 8 * rcx  ; Add offset for current row (i)
    add rdi, 8 * rdx  ; Add offset for current column (j)
    mov [rdi], rax  ; Store value from rax to calculated address


    inc rdx
    jmp .i_cycle
.end_i_cycle:

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