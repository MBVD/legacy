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