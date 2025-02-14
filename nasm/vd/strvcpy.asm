%include "io64.inc"

section .data
    nullptr equ 0

section .bss
    buffer resb 256
    buffer2 resb 256
    addr resq 16
    addr2 resq 16

section .text
global CMAIN
CMAIN:
    mov rbp, rsp; for correct debugging
    mov rax, buffer
    push addr
    push buffer
    call read_sentence
    add rsp, 16
    
    push addr
    call print_sentence
    add rsp, 8

    
    push addr2
    push addr
    push buffer2
    push buffer
    call strvcpy
    add rsp, 32
    
    push addr2
    call print_sentence
    add rsp, 8
    ret
    
;int read_sentence(buffer, addr)
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
    mov qword[rsi + 8 * rcx], nullptr
    mov rax, rcx
    
    pop rcx
    pop rsi
    pop rdi
    leave
    ret
    
;void print_sentence(addr)
print_sentence:
    NEWLINE
    enter 0, 0
    push rcx
    push rdi
    xor rcx, rcx
    mov rdi, [rbp + 16]; addr
.main_cycle:
    mov rsi, [rdi + 8 * rcx]
    cmp rsi, nullptr
    je .end_main_cycle
.inner_cycle:
    lodsb
    cmp al, `\0`
    je .end_inner_cycle
    PRINT_CHAR al
    jmp .inner_cycle
.end_inner_cycle:
    PRINT_CHAR ' '
    inc rcx
    jmp .main_cycle
.end_main_cycle:
    NEWLINE
    pop rdi
    pop rcx
    leave
    ret

; int strlen(addr) Мади
strvlen:
    enter 0, 0
    push rdx
    push rcx
    push rdi
    xor rdx, rdx
    xor rcx, rcx
    mov rdi, [rbp + 16]
.main_cycle:
    mov rsi, [rdi + 8 * rcx]
    cmp rsi, nullptr
    je .end_main_cycle
.inner_cycle:
    lodsb
    cmp al, `\0`
    je .end_inner_cycle
    jmp .inner_cycle
 .end_inner_cycle:
    inc rdx
    inc rcx
    jmp .main_cycle
 .end_main_cycle:
    mov rax, rdx
    pop rdi
    pop rcx
    pop rdx
    leave
    ret



;  strvcpy(buff, addr2)
strvcpy:
    enter 0, 0
    
    mov rsi, [rbp + 16] ; buff1
    mov rdi, [rbp + 24] ; buff2
    mov r8, [rbp + 32] ; addr1
    mov r9, [rbp + 40] ; addr2
    
    xor rcx, rcx
.for_i:
    mov qword[r9 + 8 * rcx], rdi
    cmp byte[rsi], `\0`
    je .end_for_i
.inner_for:
    lodsb
    cmp al, `\0`
    je .end_inner_for
    stosb
    jmp .inner_for
.end_inner_for:
    inc rdi
    inc rcx
    jmp .for_i
    
.end_for_i:
    inc rcx
    mov qword[r9 + 8 * rcx], 0
    NEWLINE
    mov rax, rdi
    leave
    ret 
    