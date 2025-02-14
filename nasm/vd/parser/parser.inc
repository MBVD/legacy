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
parse:; node* parse(char** words)
    enter 0, 0
    push 0; i
    push rbx
    push rdi
    mov rdi, [rbp + 16]
    lea rbx, [rbp - 8]; &i
    
    push rbx
    push rdi
    call parse_sum_expr
    add rsp, 16
    
    pop rdi
    pop rbx
    leave
    ret
    
parse_sum_expr: ; node* parse_sum_expr(char** words, int* i)
    enter 0, 0
    push rcx; -8
    push rdi; -16
    push rsi; -24
    push rdx; -32
    xor rdx, rdx
    push qword[rbp + 24]; i
    push qword[rbp + 16]; words
    call parse_mult_expr
    add rsp, 16
    push rax; rbp-40 - left 
    mov rcx, [rbp + 24]
    mov rcx, [rcx]; *(i)
    mov rdi, [rbp + 16]; words
.while:
    cmp qword[rdi + rcx * 8], nullptr
    je .end_while
    mov rsi, [rdi + rcx * 8]
    cmp byte[rsi], '+'
    jne .test2
    je .good
.test2:
    cmp byte[rsi], '-'
    jne .end_while
    jmp .good
.good:
    mov dl, byte[rsi]; + or -
    mov rcx, qword[rbp + 24]
    inc qword[rcx]
    push qword[rbp + 24]; i
    push qword[rbp + 16]; words
    call parse_sum_expr
    add rsp, 16
    mov rcx, [rbp + 24]
    mov rcx, [rcx]; *(i)
    push rax; right
    push qword[rbp - 40]; left
    push null
    push rdx
    push BINARY
    call create_node
    add rsp, 40
    PRINT_STRING "created binary + node adress: "
    PRINT_DEC 8, rax
    NEWLINE
    jmp .while
.end_while:
    pop r15
    pop rdx
    pop rsi
    pop rdi
    pop rcx
    leave
    ret
 
parse_mult_expr:; node* parse_mult_expr(char** words, int* i)
    enter 0, 0
    push qword[rbp + 24]; i
    push qword[rbp + 16]; words
    call parse_unary_expr
    add rsp, 16
    push rdi; -8
    mov rdi, [rbp + 16]; words
    push rcx; -16
    push rdx; -24
    push rdi; -32
    push rax; -40
    mov rcx, [rbp + 24]
    mov rcx, [rcx]
.while:
    cmp qword[rdi +  rcx * 8], nullptr
    je .end_while
    mov rsi, [rdi + rcx * 8]
    cmp byte[rsi], '*'
    jne  .test2
    je .good
.test2:
    cmp byte[rsi], '/'
    jne .test3
    je .good
.test3:
    cmp byte[rsi], '^'
    jne .end_while
    je .good
.good:
    xor rdx, rdx
    mov dl, byte[rsi]; * or / or ^
    push rdx
    ;PRINT_STRING "HERE"
    mov rcx, qword[rbp + 24]
    inc qword[rcx]
    push qword[rbp + 24]; i
    push qword[rbp + 16]; words
    call parse_mult_expr
    add rsp, 16
    
    PRINT_STRING "right node: "
    PRINT_DEC 8, rax
    NEWLINE
    PRINT_STRING "left node: "
    PRINT_DEC 8, [rbp - 40]
    NEWLINE
    pop rdx
    push rax; right
    push qword[rbp - 40]; left
    push null
    push rdx
    push BINARY
    call create_node
    PRINT_STRING "created binary * node adress: "
    PRINT_DEC 8, rax
    PRINT_CHAR [rax + op]
    NEWLINE
    add rsp, 40
    mov rcx, qword[rbp + 24]
    mov rcx, [rcx]
    jmp .while
.end_while:
    pop r15
    pop rdi
    pop rdx
    pop rcx
    pop rdi
    leave
    ret
   
parse_unary_expr:; parse_unary_expr(char** words, int* i)
    enter 0, 0
    push rdi
    push rcx
    push rdx
    xor rdx, rdx
    mov rdi, [rbp + 16]; words
    mov rcx, [rbp + 24]; i
    mov rcx, [rcx]; *i
    push rsi
    mov rsi, [rdi + rcx * 8]
    cmp byte[rsi], '+'
    jne .test2
    jmp .if
.test2:
    cmp byte[rsi], '-'
    jne .else
    jmp .if
.if:
    mov dl, byte[rsi]
    mov rcx, qword[rbp + 24]
    inc qword[rcx]
    push qword[rbp + 24]; i
    push qword[rbp + 16]; words
    call parse_unary_expr
    add rsp, 16
    push nullptr
    push rax
    push null
    push rdx
    push UNARY
    call create_node
    PRINT_STRING "created unary node adress: "
    PRINT_DEC 8, rax
    NEWLINE
    add rsp, 40
    jmp .exit
.else: 
    cmp byte[rsi], '('
    jne .digit
    mov rcx, qword[rbp + 24]
    inc qword[rcx]
    push qword[rbp + 24]
    push qword[rbp + 16]
    call parse_sum_expr
    add rsp, 16
    mov rcx, [rbp + 24]; i
    mov rcx, [rcx]; *i
    mov rsi, [rdi + rcx * 8]
    cmp rsi, ')'
    jne .error
    mov rcx, qword[rbp + 24]
    inc qword[rcx]
    jmp .exit 
.error:
    PRINT_STRING "missing )"
    NEWLINE
    jmp .exit
.digit:
    xor rdx, rdx
    mov rsi, [rdi + 8 * rcx]; words[i]
.while:
    cmp byte[rsi + rdx], `\0`
    je .end_while
    cmp byte[rsi + rdx], '0'
    jl .variable
    cmp byte[rsi + rdx], '9'
    jg .variable 
    jmp .else2
.variable:
    push nullptr
    push nullptr
    push qword[rsi]
    push null
    push NUM
    call create_node 
    PRINT_STRING "created num x node adress: "
    PRINT_DEC 8, rax
    PRINT_CHAR [rax + variable]
    NEWLINE
    add rsp, 40
    mov rcx, qword[rbp + 24]
    inc qword[rcx]
    jmp .exit
.else2:
    inc rdx
    jmp .while
.end_while:
    push rsi
    call atof
    add rsp, 8
    mov r15, rax
    mov rcx, qword[rbp + 24]
    inc qword[rcx]
    ; rax - число
    push nullptr
    push nullptr
    push nullptr
    push null
    push NUM
    call create_node
    PRINT_STRING "created num d node adress: "
    PRINT_DEC 8, rax
    NEWLINE
    add rsp, 40
    mov [rax + value], r15
    
.exit:
    pop r15
    pop rsi
    pop rdx
    pop rcx
    pop rdi
    leave
    ret