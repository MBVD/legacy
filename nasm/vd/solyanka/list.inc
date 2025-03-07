section .data
    BUF_MAX equ 256
    null_ptr equ 0
    null equ 0
section .bss
    printf1_str db "%s", 10, 0
    struc node
        next resq 1
        prev resq 1
        word_ptr resq 1
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
    mov qword[rax + word_ptr], rdi
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
    mov rsi, [rsi + word_ptr]
    lea rdi, [printf1_str]
    mov rsi, [rbp - 16]
    mov rsi, [rsi + word_ptr]
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