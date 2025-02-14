%include "io64.inc"

section .bss
    buffer resb 256
    addr resq 16
section .data
    n dq 0
    m dq 0
    i dq 0 
    j dq 0
    null_ptr equ 0

section .text
global main
my_strcmp:
    enter 16, 0
    mov [rbp-8], rdi
    mov [rbp-16], rsi
    mov rdi, [rbp + 16]; a
    mov rsi, [rbp + 24]; b
    xor r11, r11
.while:
    ;PRINT_CHAR [rdi + r11]
    ;PRINT_CHAR [rsi + r11]
    ;NEWLINE
    ;PRINT_DEC 8, rdi
    ;PRINT_CHAR ' '
    ;PRINT_DEC 8, rsi
    ;NEWLINE
    xor rbx, rbx
    mov bl, [rdi + r11] 
    cmp bl, [rsi + r11]
    jne .end_while
    inc r11
    cmp byte[rdi + r11], `\0`
    je .end_while
    cmp byte[rsi + r11], `\0`
    je .end_while
    
    inc r11
    jmp .while
.end_while: 
    mov al, [rdi + r11]
    cmp al, [rsi + r11]
    je .return0
    jmp .ignore
.return0:
    mov rax, 0
.ignore:
    mov rdi, [rbp - 8]
    mov rsi, [rbp - 16]
    leave
    ret
    
unic_words:; int unic_words(char ** s)
    enter 8, 0
    mov rdi, [rbp + 16]; s
    xor rax, rax
    mov qword[rbp - 8], 0; ans
    xor rcx, rcx; i
.for:
    cmp qword[rdi + 8 * rcx], null_ptr
    je .end_for
    mov r8, 1; unic
    xor rdx, rdx; j
.inner_for:
    cmp rdx, rcx
    je .ignore
    cmp qword[rdi + 8 * rdx], null_ptr
    je .end_inner_for
    
    push qword[rdi + 8 * rdx]
    push qword[rdi + 8 * rcx] 
    call my_strcmp
    add rsp, 16
     
    ;PRINT_DEC 8, rax
    ;PRINT_CHAR ' '
    cmp rax, 0
    je .not_unic
    jmp .ignore
.not_unic:
    mov r8, 0
    jmp .end_inner_for
.ignore:
    inc rdx
    jmp .inner_for
.end_inner_for:
    add [rbp-8], r8
.end_inner_for_inc:
    inc rcx
    jmp .for
.end_for:
    mov rax, qword[rbp-8]
    leave
    ret
read_sentence:
    enter 0, 0
    push rdi
    push rsi
    push rcx
    
    mov rdi, [rbp + 16]; buffer
    mov rsi, [rbp + 24]; addr
            
    xor rcx, rcx
.main_cycle:
    mov [rsi + 8 * rcx], rdi 
.inner_cycle:
    GET_CHAR al
    cmp al, ' '
    je .end_inner_cycle
    cmp al, `\n`
    je .end_inner_cycle
    
    stosb
    jmp .inner_cycle
.end_inner_cycle:
    mov byte[rdi], `\0`
    inc rdi
    inc rcx
    cmp al, `\n`
    je .end_main_cycle
    jmp .main_cycle
.end_main_cycle:
    mov qword[rsi + 8 * rcx], null_ptr
    mov rax, rcx
    
    pop rcx
    pop rsi
    pop rdi
    leave
    ret
    
;void print_sentence(addr)
print_sentence:
    enter 0, 0

    xor rcx, rcx
    mov rdi, [rbp + 16]; addr
.main_cycle:
    mov rsi, [rdi + 8 * rcx]
    cmp rsi, null_ptr
    je .end_main_cycle
.inner_cycle:
    lodsb
    cmp al, `\0`
    je .end_inner_cycle
    jmp .inner_cycle
.end_inner_cycle:
    inc rcx
    jmp .main_cycle
.end_main_cycle:
    leave
    ret

main:
    mov rbp, rsp; for correct debugging
    mov rax, buffer
    push addr
    push buffer
    call read_sentence
    add rsp, 16
    
    push addr
    call print_sentence
    add rsp, 8
    
    lea rdi, [addr]
    push rdi
    call unic_words
    add rsp, 8
    PRINT_DEC 8, rax
    ret
    