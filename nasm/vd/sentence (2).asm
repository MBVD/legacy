%include "io64.inc"

section .data
    nullptr equ 0

section .bss
    buffer resb 256
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

; strvlen Батырхана и Юрия
word_counter:
    enter 0, 0
    push rsi
    push rcx
    mov rsi, [rbp + 16]; addr
    xor rcx, rcx
.main_cycle: 
    cmp qword[rsi + 8*rcx],nullptr
    je .end_main_cycle
    inc rcx
    jmp .main_cycle
.end_main_cycle:
    mov [wordc], rcx
    PRINT_STRING "Word number: "
    PRINT_DEC 8, [wordc]
    pop rcx
    pop rsi
    leave
    ret

;  strvcpy(buff, addr2)
strvcpy:
    enter 0, 0
    push rax
    push rsi
    push rdi
    push rcx
    xor rax, rax
    mov rsi, [rbp + 16] ;buffer
    mov rdi, [rbp + 24] ;addr2
    xor rcx, rcx
.main_cycle:
    mov [rdi + 8 * rcx], rsi
    cmp byte[rsi], `\0`
    je .end_main_cycle
.inner_cycle:
    lodsb
    cmp al, 0
    je .end_inner_cycle
    jmp .inner_cycle
.end_inner_cycle:
    inc rcx
    jmp .main_cycle
.end_main_cycle:
    inc rcx
    mov qword[rdi + 8 * rcx], 0
    NEWLINE
    mov rax, rdi
    pop rcx
    pop rdi
    pop rsi
    pop rax
    leave
    ret    
    
; char* strword(addr, word) ,поиск слова в предложении (Олег.Алена,Лаура)
strword:
    enter 0, 0
    
    push rbx
    push rcx
    push rsi
    push rdi
 
    mov rbx, [rbp + 16] ; addr
    mov rsi, [rbp + 24] ; word
    
    xor rcx, rcx
.cycle:
    mov rdi, [rbx + 8 * rcx]
    mov rsi, [rbp + 24]
    cmp rdi, nullptr
    jz .ret0
.f: 
    cmpsb
    jnz .end_f
    
    cmp byte[rsi - 1], \0
    jz .ret1
    
    jmp .f
.end_f:
    inc rcx
    jmp .cycle
.ret0:
    mov rax, nullptr
    jmp .end
.ret1:
    mov rax, [rbx + 8 * rcx]
.end:
    pop rdi
    pop rsi
    pop rcx
    pop rbx
    leave
    ret



;strncat(*str1, *str2, n) ;str1+str2 (Олег.Алена,Лаура)
strncat:
    enter 0, 0
    push rdi
    push rsi
    push rcx
    
    mov rdi, [rbp + 16] ;str1
    mov rsi, [rbp + 24]
    mov rax, \0
.strlen:
    scasb
    jz .end_strlen
    jmp .strlen
.end_strlen:
    inc rdi

    xor rcx, rcx ; i
.for:
    cmp rcx, [rbp + 32]
    jge .end_for
    cmp byte[rsi + rcx], \0
    jz .end_for
    mov al, byte[rsi + rcx] ;str2[i]
    mov byte[rdi + rcx], al
    
    inc rcx 
    jmp .for
.end_for:
    mov byte[rdi + rcx], \0
    mov rax, [rbp + 16]
    
    pop rcx
    pop rsi
    pop rdi
    leave
    ret