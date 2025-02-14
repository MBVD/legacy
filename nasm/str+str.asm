%include "io64.inc"

section .text

extern malloc
extern realloc
extern free
extern strlen
extern printf

readline:
    enter 48, 0
    mov qword[rbp-8], 0 ;str
    mov qword[rbp-16], 0 ;ch - al
    mov qword[rbp-24], 0 ;size
    mov qword[rbp-32], 16 ;capacity

    mov rsi, [rbp-32]
    imul rsi, 1
    call malloc
    mov qword[rbp-8], rax

.while:
    GET_CHAR al
    cmp al, ` `
    je .endwhile
    cmp al, `\n`
    je .endwhile

    mov rsi, [rbp-8]
    mov [rsi + size], al
    mov rsi, [rbp - 24]
    mov rdi, [rbp - 32]
    cmp rsi, rdi
    jne .endwhile

    mov rsi, [rbp - 32]
    imul rsi, 2
    mov [rbp-32], rsi
    
    mov rsi, [rbp-8]
    mov rdi, [rbp-32]
    imul rdi, 1
    call realloc
    mov [rbp-8], rax
.endwhile:
    mov rsi, [rbp-8]
    mov rdi, [rbp-24]
    mov [rsi + rdi], `\0`
    mov rax, [rbp-8]

;char* sum_of_str(char* str1, char* str2)

sum_of_str:
    enter 40, 0
    mov [rbp - 8], rdi ;str1
    mov [rbp - 16], rsi; str2

    


