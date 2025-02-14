%include "io64.inc"

section .data
    hex db 'A3F', 0 ; Ваша 16-ричная строка

section .text
    global CMAIN
CMAIN:
    mov rbp, rsp; for correct debugging
    mov rdi, hex ; Указатель на 16-ричную строку
    xor rcx, rcx ; Счетчик цифр

next_digit:
    movzx eax, byte [rdi+rcx]
    cmp al, 0
    je done
    inc rcx
    jmp next_digit

done:
    xor rax, rax ; Результат в rax
    xor rdx, rdx ; Для деления на 10
    xor rcx, rcx ; Счетчик цифр

convert:
    movzx rdi, byte [hex+rcx]
    sub rdi, 48
    cmp rdi, 9
    jle digit
    sub rdi, 7
digit:
    imul rax, rax, 1   
    add rax, rdi
    inc rcx
    cmp rcx, rdx
    jne convert
    PRINT_DEC 8, rax
    jmp exit
exit:
    ret
        
