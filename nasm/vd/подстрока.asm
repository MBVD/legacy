%include "io64.inc"
;2 строки нужно найти в первой строке поодстроку второй 
section .text
extern malloc
extern strlen
global CMAIN
CMAIN:
    mov rbp, rsp; for correct debugging
    enter 24, 0
    mov rdi, 9 
    ALIGN_STACK
    call malloc
    UNALIGN_STACK
    mov [rbp - 8], rax 
    mov qword[rbp - 16], 0
.for1:
    mov rdx, [rbp - 16]
    cmp rdx, 8
    jge .end_for1
    mov rdi, [rbp - 8]
    GET_CHAR al
    mov byte[rdi + rdx], al
    inc qword[rbp - 16]
    jmp .for1
.end_for1:
    mov rdi, [rbp - 8]
    mov byte[rdi + 8], `\0`
    GET_CHAR al
    mov rdi, 6
    ALIGN_STACK
    call malloc
    UNALIGN_STACK
    mov [rbp - 24], rax; s2
    mov qword[rbp - 16], 0
.for2:
    mov rdx, [rbp - 16]
    cmp rdx, 6
    jge .end_for2
    GET_CHAR al
    mov rdi, [rbp - 24]
    mov byte[rdi + rdx], al
    inc qword[rbp - 16]
    jmp .for2
.end_for2:
    mov rdi, [rbp - 24]
    mov byte[rdi + 5], `\0`
    
    mov rdi, [rbp - 8]
    mov rsi, [rbp - 24]
    call func
    cmp rax, 1
    je .print_true
    jmp .print_false
.print_true:
    PRINT_STRING "true"
    jmp .exit
.print_false:
    PRINT_STRING "false" 
.exit:
    leave
    ret
    
    
func:
    enter 48, 0
    mov [rbp - 8], rdi
    mov [rbp - 16], rsi 
    ALIGN_STACK 
    call strlen
    UNALIGN_STACK
    mov [rbp - 24], rax; strlen1
    mov rdi, [rbp - 16]
    ALIGN_STACK
    call strlen
    UNALIGN_STACK
    mov [rbp - 32], rax
    cmp [rbp - 24], rax
    jl .return0
    mov qword[rbp - 40], 0; i
.for:
    mov rcx, [rbp - 40]
    cmp rcx, [rbp - 24]
    jge .end_for
    mov qword[rbp - 48], 0; j
.while:
    mov rdx, [rbp - 48]; j
    add rdx, [rbp - 40]; j + i
    cmp rdx, [rbp - 24]
    jge .end_while
    mov rdx, [rbp - 48]
    cmp rdx, [rbp - 32]
    jge .end_while
    mov rdx, [rbp - 48]
    add rdx, [rbp - 40]; j + i
    mov rdi, [rbp - 8]; s1
    mov al, [rdi + rdx]; s1[i + j]
    mov rdx, [rbp - 48]; j
    mov rsi, [rbp - 16]
    mov bl, [rsi + rdx]; s2[j]
    cmp al, bl
    jne .end_while
    inc qword[rbp - 48]; j++
    jmp .while
.end_while:
    mov rdx, [rbp - 32]
    cmp [rbp - 48], rdx
    jge .return1
    inc qword[rbp - 40]; i++
    jmp .for 
.end_for: 
.return0:
    mov rax, 0
    jmp .exit
.return1:
    mov rax, 1
    jmp .exit
.exit:
    leave 
    ret