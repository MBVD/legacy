%include "io64.inc"

section .data
    nullptr equ 0

section .bss
    buffer1 resb 256
    buffer2 resb 256
    addr1 resq 16
    addr2 resq 16

section .text
global CMAIN
CMAIN:
    mov rbp, rsp; for correct debugging
    mov rax, buffer1
    push addr1
    push buffer1
    call read_sentence
    add rsp, 16
    
    push addr1
    call print_sentence
    add rsp, 8
    
    mov rax, buffer2
    push addr2
    push buffer2
    call read_sentence
    add rsp, 16
    
    push addr2
    call print_sentence
    add rsp, 8
    
    push addr2
    push addr1
    call str_find
    add rsp, 24
    
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
    enter 0, 0

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
    leave
    ret
    
;int(addr1, addr2)
str_find:
    enter 0, 0
    
    xor rcx, rcx; счетчик
    mov rdi, [rbp + 16]; addr1
.main_cycle:
    mov r11, [rdi + 8 * rcx]; a[i]
    mov rsi, [r11]
    lodsb
    cmp al, `\0`
    je .not_find
    xor rsi, rsi
    
    xor rdx, rdx; счетчик
    
    mov rbx, [rbp + 24]; addr2
    
.inner_cycle:
    mov r12, [rbx + 8 * rdx]
    mov rsi, [r12]
    lodsb
    cmp al, `\0`
    je .find
    xor rsi, rsi
    xor rax, rax
    mov rsi, [r12]
    mov rax, [r11]
    cmp rax, rsi
    jne .inner_cycle_end
    inc rdx
    jmp .inner_cycle
.inner_cycle_end:
    jmp .main_cycle_end
.main_cycle_end:
    inc rcx
    jmp .main_cycle
.not_find:
    mov rax, nullptr
    jmp .end
.find:
    mov rax, [rdi]
    jmp .end
.end:
    leave
    ret