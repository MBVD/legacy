%include "io64.inc"

section .text
global CMAIN
CMAIN: 
    GET_DEC 8, rax
    push rax
    call near_deg_of_two ;vsegda
    add rsp, 8
    PRINT_DEC 8, rax
        
        
    xor rax, rax
    ret
    
    
    
near_deg_of_two:
    enter 8, 0 ;rsp-8
    push rbx
    mov [rsp-8], qword 1
    mov rbx, [rbp+16] ; parametr peredali 
    
.cycle:
    cmp [rbp-8], rbx
    jg .end_cycle
    shl qword[rbp-8], 1
    jmp .cycle
.end_cycle:
    shr qword[rbp-8], 1         
    pop rbx        
    leave
    ret    
