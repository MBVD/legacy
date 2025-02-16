%include "io64.inc"

section .data
    nullptr equ 0
    string1 db "123"
    string2 db "123"
    string3 db ""

section .bss
    buffer1 resb 256
    addr1 resq 16
    addr2 resq 16
    buffer2 resb 256

section .text
global CMAIN
CMAIN:
    mov rbp, rsp; for correct debugging
     
    call sum_of_digs
    
    lea rdi, [string3]
    push rdi
    call print_sentence
    add rsp, 8
    ret

sum_of_digs:
    enter 16, 0
    mov rbx, 0 ;i
    mov rdx, 0 ;j
    
    lea rdi, string1
    push rdi
    call strvlen
    add rsp, 8
    mov [rbp - 8], rax ;strvlen(string1)
    
    lea rdi, string2
    push rdi
    call strvlen
    add rsp, 8
    mov [rbp - 16], rax ;strvlen(string2)
    
    cmp [rbp - 8], rax ; len1 ? len2
    jg .summing_to_first
    jl .summing_to_second
    je .summing_to_first

.summing_to_first:
    mov rcx, [rbp - 8] 
    sub rcx, [rbp - 16] ; rcx = len1 - len2
    xor rdx, rdx
.adding_third_from_first:
    cmp rdx, rcx
    je .cycle
    mov al, byte[string1 + 8 * rdx]
    mov byte[string3 + 8 * rdx], al
    inc rdx
    jmp .adding_third_from_first
.cycle:   
    cmp byte[string1 + 8 * rcx], `\0`
    je .end
       
    
    mov al, byte[string1 + 8 * rcx]
    mov bl, byte[string2 + 8 * rcx]
    add al, bl
    sub al, 48
    mov byte[string3 + 8 * rcx], al
    
    inc rcx
    jmp .cycle
.summing_to_second:
    mov rcx, [rbp - 16] 
    sub rcx, [rbp - 8] ; rcx = len2 - len1
    xor rdx, rdx
.adding_third_from_second:
    cmp rdx, rcx
    je .cycle2
    mov al, byte[string2 + 8 * rdx]
    mov byte[string3 + 8 * rdx], al
    inc rdx
    jmp .adding_third_from_second
.cycle2:   
    cmp byte[string2 + 8 * rcx], `\0`
    je .end
       
    
    mov al, byte[string2 + 8 * rcx]
    mov bl, byte[string1 + 8 * rcx]
    add al, bl
    sub al, 48
    mov byte[string3 + 8 * rcx], al
    
    inc rcx
    jmp .cycle2

      
.end:
    leave 
    ret
    
    
    
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
    inc rcx
    jmp .main_cycle
.end_main_cycle:
    NEWLINE
    pop rdi
    pop rcx
    leave
    ret
