section .data
    BUF_MAX equ 256
    nullptr equ 0
    null equ 0  
    NUM equ 1
    UNARY equ 2
    BINARY equ 3
section .bss
    struc node
        type resq 1
        op resb 1
        value resq 1
        variable resb 1
        left resq 1
        right resq 1
    endstruc
section .text
create_node:; node* create_node(type, char op, char variable, node* first, node* second)
    enter 0, 0
    push rdi
    mov rdi, node_size
    call safe_malloc
    ;PRINT_STRING "ok5"
    ;NEWLINE
    push rbx
    xor rbx, rbx
    mov rbx, [rbp + 16]; type 
    mov [rax + type], rbx
    mov bl, [rbp + 24]; op
    mov [rax + op], bl
    mov bl, [rbp + 32]; variable
    mov [rax + variable], bl
    mov qword[rax + left], nullptr
    mov qword[rax + right], nullptr
    cmp qword[rbp + 16], UNARY
    jne .not_unary
    mov rbx, [rbp + 40]; first
    mov [rax + left], rbx
.not_unary:
    cmp qword[rbp + 16], BINARY
    jne .not_binary
    mov rbx, [rbp + 40]
    mov [rax + left], rbx
    mov rbx, [rbp + 48]; second
    mov [rax + right], rbx
.not_binary:
    pop rbx 
    pop rdi
    leave
    ret