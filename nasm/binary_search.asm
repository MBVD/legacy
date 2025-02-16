%include "io64.inc"
section .bss
    n resq 1
    arr resq 32
    x resq 1
section .data
    left dq 0
    right dq 4
section .text
global CMAIN
CMAIN:
    mov rbp, rsp; for correct debugging
    GET_DEC 8, [n]
    mov rcx, 0
    mov rbx, 1; rbx = old_count
    mov rdx, 1; rdx = count
read_arr:
    cmp rcx, [n]
    jnl end_read
    GET_DEC 8, rax
    mov [arr + 8 * rcx], rax
    inc rcx
    jmp read_arr
end_read:
    xor rcx, rcx
    xor rbx, rbx
    xor rdx, rdx
    
    GET_DEC 8, rax
    mov [x], rax
    
    mov r8, arr
    mov r9, [left]
    mov r10, [right]
    mov r11, [x]
    push r11 ;[rbp+40]
    push r10 ; [rbp+32]
    push r9 ; [rbp+24]
    push r8 ; [rbp+16]
    call binarySearch
    add rsp, 32  
    ; Print the result
    PRINT_DEC 8, rax

binarySearch:
    enter 0, 0
    push rcx
    push rdi
    push rsi
    push rdx
    mov rcx, [rbp+32]
    cmp rcx, [rbp+24]
    jl .badReturn
    mov rdi, rcx
    add rdi, [rbp+24]
    mov rax, rdi
    mov rsi, 2
    xor rdx, rdx ; Clear rdx for div
    div rsi
    mov rdi, rax
    mov rsi, [rbp+40]
    cmp [arr + 8*rdi], rsi
    je .midReturn
    jg .rightReturn
    jl .leftReturn
    
.midReturn:
    mov rax, rdi
    pop rdx
    pop rsi
    pop rdi
    pop rcx
    leave
    ret

.rightReturn:
    mov [rbp + 32], rdi
    push qword[rbp+32] ; binary_search(
    pop qword[rbp - 8]
    dec qword[rbp - 8]
    push rsi
    push qword[rbp - 8]
    push qword[rbp + 24]
    push r8
    call binarySearch
    add rsp, 32
    pop rdx
    pop rsi
    pop rdi
    pop rcx
    leave
    ret

.leftReturn:
    mov [rbp + 24], rdi
    push qword[rbp+24]
    pop qword[rbp - 8]
    inc qword[rbp - 8]
    push rsi
    push rcx
    push qword[rbp - 8]
    push r8
    call binarySearch
    add rsp, 32
    pop rdx
    pop rsi
    pop rdi
    pop rcx
    leave
    ret

.badReturn:
    mov rax, -1
    pop rdx
    pop rsi
    pop rdi
    pop rcx
    leave
    ret
