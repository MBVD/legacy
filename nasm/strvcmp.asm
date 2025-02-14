%include "io64.inc"

section .data
    nullptr equ 0
section .bss
    str1 resb 256
    str2 resb 256
    addr1 resq 16
    addr2 resq 16
section .text
global CMAIN
CMAIN:
    mov rbp, rsp; for correct debugging
    push addr1
    push str1
    call read_sentence
    add rsp, 16
    mov r8, rax     ;количество слов в str1
    
    push addr2
    push str2
    call read_sentence
    add rsp, 16
    mov r9, rax     ;количество слов в str2
    
    push r8
    push r9 
    push addr2
    push addr1
    call strvcmp
    add rsp, 32
    ret
    
read_sentence:
    enter 0, 0
    push rdi
    push rsi
    push rcx
    mov rdi, [rbp + 16]    ;str
    mov rsi, [rbp + 24]    ;addr
    
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
    
    pop rdi
    pop rsi
    pop rcx
    
    leave
    ret
    
   
strvcmp:
    enter 0, 0
    
    push rdi
    push rsi
    push rbx
    push rdx
    push rcx
    push r8
    push r9
    
    mov rbx, [rbp + 16] ;addr1
    mov rdx, [rbp + 24] ;addr2
    mov r9, [rbp + 32]  ;words_str2
    mov r8, [rbp + 40]  ;words_str1
    cmp r8, r9
    jg .end_greater
    jl .end_less
    
    xor rcx, rcx
    
.start_cycle:
    mov rdi, [rbx + 8 * rcx]
    mov rsi, [rdx + 8 * rcx]
    jmp .check
    
.check:
    cmpsb       ;сравнение rsi ? rdi и сдвиг
    je .equal
    jg .end_less
    jl .end_greater

.equal:
    cmp byte[rdi - 1], `\0`
    je .end_word
    jmp .check
    
.end_greater:
    mov rax, 1
    jmp .end
    
.end_less:
    mov rax, -1
    jmp .end
    
.end_word:
    cmp byte[rdi], nullptr
    je .end_equal
    inc rcx
    jmp .start_cycle
    
.end_equal:
    xor rax, rax
    jmp .end
    
.end:
    PRINT_DEC 8, rax
    pop r9
    pop r8
    pop rcx
    pop rdx
    pop rbx
    pop rsi
    pop rdi
    
    leave
    ret