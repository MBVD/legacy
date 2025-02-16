%include "io64.inc"
section .data
    string1 db "owl"
    string2 db "Mycar"
section .text

global main
main:
    mov rbp, rsp; for correct debugging
    mov rbx, 0 ;i
    mov rdx, 0 ;j
    mov rcx, 0 ;k
cycle:
    cmp byte[string1 +8*rbx], `\0`
    je end
cycle2:
    cmp byte[string2 + 8*rcx], `\0`
    je continue
    mov al, byte[string1 + 8*rdx]
    cmp byte[string2 + 8*rcx], al
    jne continue
    inc rdx
    inc rcx
    jmp cycle2
continue:
    cmp rcx, 0 
    jng end_0
    cmp byte[string2 + 8*rcx], `\0`
    jne end_0
    PRINT_DEC 1, 1
    jmp end
end_0:
    PRINT_DEC 1, 0
    jmp end
end:
    ret