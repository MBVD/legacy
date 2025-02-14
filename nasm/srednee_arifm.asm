%include "io64.inc"


section .text


global CMAIN
CMAIN:
    GET_DEC 8, rcx
    mov rcx, 0      
    mov rdx, 0   ; count
    mov rax, 0  ; sum
   
cycle:  
    cmp rcx, rdx   
    jg cycle_body   
    jmp cycle_end   
cycle_body:   
    GET_DEC 8, rbx   
    add rax, rbx   
    inc rdx   
    jmp cycle   
cycle_end:   
    mov rdx, 0  
    div rcx  ; srednee
    PRINT_DEC 8, rax
    ret