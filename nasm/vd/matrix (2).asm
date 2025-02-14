section .data
    scanf_format db "%lf", 0
    printf_format db "%lf ", 0
    n dq 3
    m dq 3

section .bss
    A resq 256
    A_rows resq 16
    B resq 256
    B_rows resq 16
    C resq 256
    C_rows resq 16

section .text

    %macro SAVE 0
        push rdi
        push rsi
        push rdx
        push rcx
        push r8
        push r9
    %endmacro

    %macro RESTORE 0
        pop r9
        pop r8
        pop rcx
        pop rdx
        pop rsi
        pop rdi
    %endmacro


    %macro ALIGN_STACK 0
        enter 0, 0
        and rsp, -16
    %endmacro

    %macro UNALIGN_STACK 0
        leave
    %endmacro

extern printf
extern scanf
extern putchar

global main
main:
    enter 0, 0
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


    xor rax, rax
    leave
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
    
    SAVE
    lea rdi, [scanf_format]
    lea rsi, [rsi + 8 * rdx]    ;A[i][j]
    ALIGN_STACK
    call scanf
    UNALIGN_STACK
    RESTORE

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
    SAVE
    lea rdi, [printf_format]
    movsd xmm0, [rsi + 8 * rdx]
    mov rax, 1
    
    ALIGN_STACK
    call printf
    UNALIGN_STACK
    RESTORE
    
    inc rdx
    jmp .cycle
.end_cycle:
    SAVE
    mov rdi, 10
    call putchar
    RESTORE
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
        