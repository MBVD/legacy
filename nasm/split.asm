%include "io64.inc"

section .data
    nullptr equ 0
    string1 db "Happy New Year"

section .bss
    buffer resb 256
    addr resq 16

section .text
global CMAIN
CMAIN:
    mov rbp, rsp; for correct debugging
    
    lea rdi, [string1]
    push addr
    push buffer
    push rdi
    call split
    add rsp, 24
     
    push addr
    call print_sentence
    add rsp, 8
    ret

; split(string, buffer, addr)    
split:
    enter 0, 0
    mov rdi, [rbp + 24] ; buffer
    mov rsi, [rbp + 16] ; string
    mov rbx, [rbp + 32] ; addr
    xor rcx, rcx
.main_cycle:
    mov [rbx + 8 * rcx], rdi
.inner_cycle:
    lodsb
    cmp al, ' '
    je .end_inner_cycle
    cmp al, `\0`
    je .end_inner_cycle
    
    stosb
    jmp .inner_cycle
.end_inner_cycle:
    mov byte[rdi], `\0`
    inc rdi
    inc rcx
    cmp al, `\0`
    je .end_main_cycle
    jmp .main_cycle
.end_main_cycle:
    mov qword[rsi + 8 * rcx], nullptr
    
    mov rax, rcx
    
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
