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
perv_tree_node:; node* perv_tree_node(node* root) create_node(type, char op, char variable, node* first, node* second)
    enter 32, 0
    mov rdi, [rbp + 16]
    cmp qword[rdi + type], BINARY
    je .binary
    jmp .else
.binary:
    cmp byte[rdi + op], '+'
    je .plus_min
    cmp byte[rdi + op], '-'
    je .plus_min
    cmp byte[rdi + op], '^'
    je .exponent
    cmp byte[rdi + op], '*'
    je .mul_div
    cmp byte[rdi + op], '/'
    je .mul_div
.plus_min:
    push qword[rdi + left]
    call perv_tree_node
    add rsp, 8
    mov rdi, [rbp + 16]
   
    push rax; rbp - 8 -> left
    mov [rbp - 8], rax
    mov rdi, [rbp + 16]
    push qword[rdi + right]
    call perv_tree_node
    add rsp, 8
    
    mov rdi, [rbp + 16]
    
    push rax; 
    push qword[rbp - 8]
    push nullptr
    push qword[rdi + op]
    push BINARY
    call create_node
    add rsp, 40
    jmp .exit
.exponent:; create_node(type, char op, char variable, node* first, node* second)
    push nullptr
    push nullptr
    mov rsi, [rdi + left]
    push qword[rsi + variable]
    push null
    push NUM
    call create_node
    add rsp, 40
    mov [rbp - 8], rax; [rbp - 8]
    
    push nullptr
    push nullptr
    push null
    push null
    push NUM
    call create_node
    add rsp, 40
    mov rsi, [rdi + right]
    mov rdx, qword[rsi + value]
    inc rdx
    mov [rax + value], rdx
    mov [rbp - 16], rax; [rbp - 16]
    push qword[rbp - 16]; right
    push qword[rbp - 8]; left
    push null
    push '^'
    push BINARY
    call create_node
    add rsp, 40
    mov [rbp - 8], rax; left
    push nullptr
    push nullptr
    push null
    push null
    push NUM
    call create_node
    add rsp, 40
    mov [rax + value], rdx
    push rax
    push qword[rbp - 8]
    push null
    push '/'
    push BINARY
    call create_node
    add rsp, 40
    jmp .exit
.mul_div:
    mov rsi, [rdi + left]
    cmp byte[rsi + variable], null
    je .else2
    mov r15, [rsi + variable]
    mov [rbp - 8], r15; [rbp - 8]
    mov rsi, [rdi + right]
    mov r15, [rsi + value]
    mov [rbp - 16], r15; rbp - 16
    push qword[rdi + left]
    call perv_tree_node
    add rsp, 8
    mov [rbp - 24], rax; [rbp - 24]
    mov rdi, [rbp + 16]
    jmp .after
.else2:
    mov rsi, [rdi + right]
    mov r15, qword[rsi + variable]; 
    mov [rbp - 8], r15;rbp - 8
    mov rsi, [rdi + left]
    mov r15, [rsi + value]
    mov [rbp - 16], r15; rbp - 16
    push qword[rdi + right]
    call perv_tree_node
    add rsp, 8
    mov rdi, [rbp + 16]
    mov [rbp - 24], rax; [rbp - 24]
.after:
    push nullptr
    push nullptr
    push null
    push null
    push NUM
    call create_node
    add rsp, 40
    mov rdx, [rbp - 16]
    mov [rax + value], rdx
    push rax
    push qword[rbp - 24]
    push null
    push qword[rdi + op]
    push BINARY
    call create_node
    add rsp, 40 
    jmp .exit
.else:
  cmp qword[rdi + type], UNARY
  je .unary
  jmp .num
.unary:
    push qword[rdi + left]
    call perv_tree_node
    add rsp, 8
    mov rdi, [rbp + 16]
    push nullptr
    push rax
    push qword[rdi + variable]
    push qword[rdi + op]
    push UNARY
    call create_node
    add rsp, 40
    jmp .exit
.num:
    cmp byte[rdi + variable], null
    je .else3
    push nullptr
    push nullptr
    PRINT_STRING "variable: "
    PRINT_CHAR [rdi + variable]
    NEWLINE
    push qword[rdi + variable]
    push null
    push NUM
    call create_node
    add rsp, 40
    mov [rbp - 8], rax; [rbp - 8] tmp1
    push nullptr
    push nullptr
    push null
    push null
    push NUM
    call create_node
    mov qword[rax + value], 2
    push rax
    push qword[rbp - 8]
    push null
    push '^'
    push BINARY
    call create_node
    add rsp, 40
    mov [rbp - 8], rax; left
    push nullptr
    push nullptr
    push null
    push null
    push NUM
    call create_node
    mov qword[rax + value], 2
    add rsp, 8
    push rax
    push qword[rbp - 8]
    push null
    push '/'
    push BINARY
    call create_node
    add rsp, 40
    jmp .exit
.else3:
    push nullptr
    push nullptr
    push null
    push null
    push NUM
    call create_node
    add rsp, 40
    mov [rbp - 8], rax;[rbp - 8]
    mov rdx, [rdi + value]
    mov [rax + value], rdx
    push nullptr
    push nullptr
    push 'x'
    push null
    push NUM
    call create_node
    add rsp, 40 
    push rax
    push qword[rbp - 8]
    push null
    push '*'
    push BINARY
    call create_node
    add rsp, 40
    jmp .exit
.exit:
    leave
    ret  