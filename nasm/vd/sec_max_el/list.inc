section .data
    BUF_MAX equ 256
    null_ptr equ 0
    null equ 0
section .bss
    printf1_str db "%s", 10, 0
    struc node
        next resq 1
        prev resq 1
        data resq 1
    endstruc
section .text
create_node: ;node* create_node(char* word, node* prev, node* next)
    enter 24, 0
    mov [rbp - 8], rdi; word
    mov [rbp - 16], rsi; prev
    mov [rbp - 24], rdx; next

    mov rdi, node_size
    ALIGN_STACK
    call malloc
    UNALIGN_STACK
    mov rdi, [rbp - 8]
    mov qword[rax + data], rdi
    mov rsi, [rbp - 16]
    mov [rax + prev], rsi
    mov rdx, [rbp - 24]
    mov [rax + next], rdx

    leave
    ret

print_list: ;void print_list(node* head)
    enter 16, 0
    mov [rbp - 8], rdi; head
    mov [rbp - 16] , rdi; tmp
.while:
    mov rdi, [rbp - 16]
    cmp rdi, null_ptr
    je .end_while
    mov rdi, [rbp - 8]
    mov rsi, [rbp - 16]
    mov rsi, [rsi + data]
    lea rdi, [printf1_str]
    mov rsi, [rbp - 16]
    mov rsi, [rsi + data]
    mov rax, 1
    ALIGN_STACK
    call printf
    UNALIGN_STACK
    mov rdi, [rbp - 16]
    mov rdi, [rdi + next]
    mov [rbp - 16], rdi
    jmp .while
.end_while:
    leave
    ret

get_list:
    enter 32, 0
    mov [rbp - 8], rdi; node** head
    mov [rbp - 16], rsi; n
    mov rdi, [rdi]
    mov [rbp - 32], rdi; node* tmp
    mov qword[rbp - 24], 0; n
.for:
    mov rcx, [rbp - 24]
    cmp rcx, [rbp - 16]
    jge .end_for
    mov rdi, [rbp - 8]
    mov rdi, [rdi]
    cmp rdi, nulllptr
    je .rebase_head
    mov rsi, [rbp - 32]; tmp
    GET_DEC 8, rdi
    mov rdx, null_ptr
    call create_node
    mov rdi, [rbp - 32]
    mov [rdi + next], rax
    mov [rbp - 32], rax
    inc qword[rbp - 24]
    jmp .for
.rebase_head:
    GET_DEC 8, rdi
    mov rsi, null_ptr
    mov rdx, null_ptr
    call create_node
    mov rdi, [rbp - 8]
    mov [rdi], rax
    mov [rbp - 32], rax
.next
    inc qword[rbp - 24]
    jmp .for 
.end_for:
    leave
    ret