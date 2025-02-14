%include "io64.inc"

section .bss
    array resq 10
section .data
    size dq 10
    l dq 0
    r dq 9
    m dq 0
    tmp dq 0

section .text
global main
main:
    mov rbp, rsp 
    mov rcx, 0
    mov rdx, 0
    mov r9, 0
    mov rbx, 0
    lea rdi, [array]
    mov rsi, [array] 

read_arr:
    cmp rcx, 10 
    jge end_read 
    mov rax, 0
    GET_DEC 8, rax
    stosq
    inc rcx
    jmp read_arr 

end_read:
    lea rsi, [array]
    mov rcx, 10
    PRINT_STRING "INput array" 
    PRINT_CHAR `\n`

print_arr:
    cmp rcx, 0 
    jle end_print
    lodsq
    PRINT_DEC 8, rax
    PRINT_CHAR ` `
    dec rcx
    jmp print_arr
end_print:
    lea rsi, [array]
    mov rax, [rsi]
    mov rcx, 0
buble_sort:
    cmp rcx, [size]
    jge buble_sort_end
    mov rdx, 0
    mov r9, [size]
    sub r9, rcx; size - i
    sub r9, 1; size - i - 1
    for:
        cmp rdx, r9
        jge for_end
        lea rsi, [array + 8*rdx]; arr[j]
        lea rdi, [array + 8*rdx + 8] ; arr[j+ 1]
        mov rax, [rsi]
        cmp rax, [rdi]
        jg if_true  
        jmp else
        if_true:
            mov r8, [rdi]; tmp = arr[j+1]
            mov [rdi], rax ; rdi = rsi arr[j+1] = arr[j]
            mov [rsi], r8 ; rsi = rdi arr[j] = tmp   
        else:
        inc rdx
        jmp for
    for_end:
        inc rcx
        jmp buble_sort   
buble_sort_end:
    lea rsi, [array]
    mov rcx, 10
    PRINT_CHAR `\n`
    PRINT_STRING "output array" 
    PRINT_CHAR `\n`
print_arr_1:
    cmp rcx, 0
    jle end_print_1
    lodsq
    PRINT_DEC 8, eax
    PRINT_CHAR ` `
    dec rcx
    jmp print_arr_1
end_print_1:
    mov rcx, 0
count_famous:
    mov rax, 1; cnt
    mov rbx, 0; el
    mov rcx, 0; i
    mov rdx, 0; mx
    cycle:
        cmp rcx, [size]
        jge cycle_end
        lea rdi, [array + 8*rcx]
        lea rsi, [array + 8*rcx + 8]
        mov r8, [rdi] 
        cmp r8, [rsi]
        je if
        jne else1
        if: 
            inc rax
            jmp else_end
        else1:
            cmp rax, rdx
            jg if2
            jle else2
            if2:
                mov rbx, [rdi]
                mov rdx, rax
            else2:
                mov rax, 1 
        else_end:
        inc rcx
        jmp cycle
    cycle_end:
        cmp rax, rdx
        jg if3
        jmp else3
        if3:
            mov r10, [size]
            lea rsi, [array + 8 * r10 - 8]
            mov rbx, [rsi]
        else3:
        PRINT_CHAR `\n`
        PRINT_DEC 8, rbx 
        PRINT_STRING " - "
        PRINT_DEC 8, rdx
        PRINT_STRING " raza"
    ret