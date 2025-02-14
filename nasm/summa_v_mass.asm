%include "io64.inc"
section .data
    arr db 1, 2, 3, 4, 5, 6
    arr_length equ $ - arr ; dlina massiva
section .text
    global CMAIN
CMAIN:
    mov rbp, rsp; for correct debugging
    mov rdi, arr 
    mov rcx, arr_length
    call summ   
    PRINT_DEC 8, rax
    NEWLINE
    jmp end

summ:
    xor rax, rax            
next:
    movzx rdx, byte [rdi]   ; tekushiy byte iz mas
    add rax, rdx            
    inc rdi                 
    dec rcx
    jnz next
    ret
end:
    ret