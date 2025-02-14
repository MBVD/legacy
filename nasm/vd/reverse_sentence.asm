%include "io64.inc"

section .data
    nullptr equ 0

section .bss
    buffer resb 256
    addr resq 16

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
    call reverse_sentence
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

    
;void reverse_sentence(addr)
reverse_sentence:
    enter 0, 0

    xor rcx, rcx
    mov rdi, [rbp + 16] ;addr
    mov rdx, rdi
    
.count_words:
    mov rsi, [rdx + 8 * rcx]
    cmp rsi, nullptr
    je .end_count_words
    inc rcx
    jmp .count_words
    
.end_count_words:
    dec rcx
    
.print_reverse:
    cmp rcx, 0
    jl .end_print_reverse
    mov rsi, [rdi + 8 * rcx]
    
.inner_cycle:
    lodsb
    cmp al, `\0`
    je .end_inner_cycle
    PRINT_CHAR al
    jmp .inner_cycle
    
.end_inner_cycle:
    PRINT_CHAR ' '
    dec rcx
    jmp .print_reverse
    
.end_print_reverse:
    NEWLINE
    leave
    ret
