%include "io64.inc"
section .data
    BUF_MAX equ 256
    nullptr equ 0
    null equ 0
    NUM equ 1
    UNARY equ 2
    BINARY equ 3
section .bss
    a resq 1
    struc node
        type resq 1
        op resb 1
        value resq 1
        variable resb 1
        left resq 1
        right resq 1
    endstruc
section .text
extern malloc
extern printf
global main

atof: ; int atof(char* str)
    enter 0, 0
    push rbx
    xor rax, rax
    push rdi
    mov rdi, [rbp + 16]
    push rcx
    xor rcx, rcx
.while:
    cmp byte[rdi + rcx], `\0`
    je .end_while
    imul rax, rax, 10
    add al, byte[rdi + rcx] 
    sub al, '0'
    inc rcx
    jmp .while
.end_while:
    pop rcx
    pop rdi
    pop rbx
    leave
    ret
    

safe_malloc:
    enter 0, 0
    push rcx
    push rdx
    push r9
    push r10
    push r8
    mov rcx, rdi
    ALIGN_STACK
    call malloc
    UNALIGN_STACK
    pop r8
    pop r10
    pop r9
    pop rdx
    pop rcx 
    leave
    ret
strncpy:; void strncpy(char* source, char* target, int size)
    enter 32, 0
    push rsi
    mov rsi, qword[rbp + 16]; source
    push rdi
    mov rdi, qword[rbp + 24]; target
    push r9
    mov r9, qword[rbp + 32]; size
    push rcx
    xor rcx, rcx
.while:
    cmp rcx, qword[rbp + 32]
    jge .end_while
    mov r9b, byte[rsi + rcx]
    mov [rdi + rcx], r9b
    inc rcx
    jmp .while
.end_while: 
    mov byte[rdi + rcx], `\0`
    pop rcx
    pop r9
    pop rdi
    pop rsi
    leave
    ret

print_string:; void print_string(char* str)
    enter 16, 0
    push rdi
    mov rdi, [rbp + 16]
    push rcx
    xor rcx, rcx
.for:
    cmp byte[rdi + rcx], `\0`
    je .end_for
    PRINT_CHAR [rdi + rcx]
    inc rcx
    jmp .for
.end_for:
    NEWLINE
    pop rcx
    pop rdi 
    leave
    ret

readline:
    enter 32, 0
    push rcx
    push rdi
    mov rdi, BUF_MAX
    push rdx
    call safe_malloc; rax - адресс для нашей строки
    push rsi
    mov rsi, rax
    xor rcx, rcx
.for:
    GET_CHAR al 
    cmp al, `\n`
    je .end_for
    mov [rsi + rcx], al; buf[i] = char
    inc rcx
    jmp .for
.end_for:
    mov byte[rsi + rcx], `\0`
    mov rax, rsi
    pop rsi
    pop rdx
    pop rdi
    pop rcx
    leave
    ret
    
strlen: ;strlen(char* str)
    enter 16, 0
    push rdi
    push rcx
    mov rdi, [rbp + 16]
.for:
    cmp byte[rdi + rcx], `\0`
    je .end_for
    inc rcx
    jmp .for
.end_for:
    mov rax, rcx
    pop rcx
    pop rdi
    leave 
    ret
    
; char** split_string(char* buf, char delimeter)
split_string:
    enter 32, 0
    push qword[rbp + 16]
    call strlen; rax - strlen(buf)
    add rsp, 8
    push r9
    xor r9, r9
    push rdi
    mov rdi, rax
    imul rdi, rdi, 8
    call safe_malloc; rax - (char**)words
    push rcx
    xor rcx, rcx
    mov rdi, qword[rbp + 16]; buf
.for:
    cmp byte[rdi + rcx], `\0`
    je .end_for
.while:
    push r8
    mov r8b, byte[rbp + 24]
    cmp [rdi + rcx], r8b; cmp buf[rcx], delimeter
    jne .end_while
    inc rcx
    jmp .while
.end_while:
    push rdx
    xor rdx, rdx
    push rbx
.while2:
    mov rbx, rcx
    add rbx, rdx
    cmp byte[rdi + rbx], r8b; cmp buf[rcx + rdx], delimeter
    je .end_while2
    cmp byte[rdi + rbx], `\0`;
    je .end_while2 
    cmp byte[rdi + rbx], '+';
    je .end_while2
    cmp byte[rdi + rbx], '-';
    je .end_while2
    cmp byte[rdi + rbx], '*';
    je .end_while2
    cmp byte[rdi + rbx], '/';
    je .end_while2
    cmp byte[rdi + rbx], '(';
    je .end_while2
    cmp byte[rdi + rbx], ')';
    je .end_while2
    cmp byte[rdi + rbx], '^';
    je .end_while2
    inc rdx
    jmp .while2
.end_while2:
.if:
    cmp rdx, 0
    jng .if2
    inc rdx
    mov rdi, rdx
    dec rdx
    mov r8, rax; теперь r8 - words
    call safe_malloc
    mov rdi, qword[rbp + 16]; buf
    mov [r8 + r9 * 8], rax
    mov rax, r8; снова words в rax
    
    mov r12, rdi
    add r12, rcx
    mov r13, [rax + r9 * 8]
    push rdx
    push r13
    push r12
    call strncpy
    add rsp, 24
    ; TODO копирование строки yes
    push r11
    mov r11, qword[rax + r9 * 8]
    mov byte[r11 + rdx], `\0`
    add rcx, rdx
    inc r9
.if2:
    cmp byte[rdi + rbx], `+`;
    je .if2_true
    cmp byte[rdi + rbx], `-`;
    je .if2_true
    cmp byte[rdi + rbx], `*`;
    je .if2_true
    cmp byte[rdi + rbx], `/`;
    je .if2_true
    cmp byte[rdi + rbx], `(`;
    je .if2_true
    cmp byte[rdi + rbx], `)`;
    je .if2_true
    cmp byte[rdi + rbx], `^`;
    je .if2_true
    jmp .ignore
.if2_true:
    mov rdi, 2
    mov r8, rax; теперь r8 - words
    call safe_malloc
    mov rdi, qword[rbp + 16]; buf
    mov [r8 + r9 * 8], rax
    mov rax, r8; снова в rax - words
    mov r12, rdi
    add r12, rcx
    mov r13, [rax + r9 * 8]
    push 1
    push r13
    push r12
    call strncpy
    add rsp, 24
    ; TODO копирование строки yes
    push r11
    mov r11, qword[rax + r9 * 8]
    mov byte[r11 + 1], `\0`
    inc rcx
    inc r9
.ignore:
    jmp .for
.end_for:
    mov qword[rax + r9 * 8], nullptr
    pop r9
    pop rbx
    pop rdx
    pop r8
    pop rcx
    pop rdi
    leave
    ret 

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
    
         
print_tree: ;void print_tree(node* root)
    enter 32, 0
    push rdi
    mov rdi, [rbp + 16]
    cmp qword[rdi + type], BINARY
    jne .else
    push qword[rdi + left]
    call print_tree
    add rsp, 8
    PRINT_CHAR [rdi + op]
    push qword[rdi + right]
    call print_tree
    add rsp, 8
    jmp .exit
.else:
     cmp qword[rdi + type], UNARY
     jne .else2
     PRINT_CHAR [rdi + op]
     push qword[rdi + left]
     call print_tree
     add rsp, 8
.else2:
    cmp qword[rdi + variable], nullptr
    jne .print_variable
    jmp .print_value 
.print_variable:
    PRINT_CHAR [rdi + variable]
    jmp .exit
.print_value:
    PRINT_DEC 8, [rdi + value]
    
.exit:
    pop rdi
    leave
    ret   
    
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
    jmp .else
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

main:
    mov rbp, rsp; for correct debugging
    call readline
    PRINT_DEC 8, rsp
    PRINT_CHAR ' ' 
    PRINT_DEC 8, rbp
    NEWLINE 
    mov rdx, rax
    xor rcx, rcx
    PRINT_STRING 'Our string : '
    push rax
    call print_string
    PRINT_STRING "our words adress: "
    PRINT_DEC 8, rax
    NEWLINE
    add rsp, 8
    
    push ' '
    push rax
    call split_string
    add rsp, 16
    
    push rax
    call parse
    add rsp, 8
    
    push rax
    call print_tree
    add rsp, 8
    
    NEWLINE
    
    push rax
    call perv_tree_node
    add rsp, 8
    
    push rax
    call print_tree
    add rsp, 8
    ret