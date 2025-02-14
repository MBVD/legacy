section .data
    null_ptr equ 0
    BUF_MAX equ 256
section .text
extern malloc
get_string:; возвращает указатель на созданную строку ввод до \n
    enter 16, 0
    mov rdi, BUF_MAX
    ALIGN_STACK
    call malloc
    UNALIGN_STACK
    mov [rbp - 8], rax
    mov qword[rbp - 16], 0; i
.while:
    GET_CHAR al 
    cmp al, `\n`
    je .end_while
    mov rdi, [rbp - 8]; word
    mov rcx, [rbp - 16]; i
    mov byte[rdi + rcx], al

    inc qword[rbp - 16]
    jmp .while
.end_while:
    mov rax, [rbp - 8]
    leave
    ret