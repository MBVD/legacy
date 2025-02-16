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
    