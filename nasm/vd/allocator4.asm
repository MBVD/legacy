%include "io64.inc"
section .bss
    brk resb 8192            ;массив куда складываем
    structs resq 3 * 1000     ; массив структур
section .data
    structs_size dq -1
    brk_size dq 0
    size dq 0
head:
    dq 0
    dq 0
    dq 0
section .text
global main

sbrk: ; выделим новую память
    enter 0, 0
    mov rcx, [rbp+16]
    inc qword[structs_size]; увеличиваем индекс массива где хранятся структуры
    mov rdx, qword[structs_size]
    imul rdx, rdx, 24
    lea rdi, [structs + rdx]; structs[stricts_size]
    mov qword[rdi], 1; structs[stricts_size][used] = 1
    mov r8, [brk_size]
    mov [rdi + 16], rcx; structs[stricts_size][16] = size
    lea rax, [brk]; rax = &brk
    add rax, [brk_size]; rax += brk_size
    mov [rdi + 8], rax; structs[stricts_size][index] = rax
    add [brk_size], rcx; brk_size += size 
    leave
    ret
    
    
find_free_place:; возвращает -1 если ничего не находит и аддрес если есть свободное пространство
    mov rax, -1
    enter 0, 0
    mov rcx, [rbp + 16]
    xor rdx, rdx
.for:
    cmp rdx, qword[structs_size]
    jge .end_for
    imul rdx, rdx, 24
    lea rdi, [structs + rdx]; structs[rdx]
    cmp qword[rdi], 0; used == 0
    je .not_used
    jmp .ignore
.not_used:
    cmp [rdi + 16], rcx; cmp structs[rdx][size], size
    jge .found
    jmp .ignore
.found:
    inc qword[structs_size]
    mov r9, [structs_size]
    imul r9, r9, 24
    lea rsi, [structs + r9]
    mov qword[rsi], 0 ; used = 0
    
    mov r10, [rdi + 8]
    add r10, 8
    mov [rsi + 8], r10 ;structs[rdx][adress] + 8 
    mov r10, [rdi + 16]
    sub r10, rcx
    mov [rsi + 16], r10 ;size = [rdi + 16] - rcx
    ; добавить новый элемент в массив structs который будет содержать index: structs[rdx][adress] + 1
    ; и size = [rdi + 16] - rcx
    ; used = 0
    mov [rdi + 16], rcx; mov structs[rdx][size], size обноввили наш structна размер
    mov qword[rdi], 1; structs[used] = 1
    mov rax, qword[rdi + 8]; brk[structs[rdx][index]]
    jmp .end_for 
.ignore:    
    inc rdx
    jmp .for
.end_for:
    leave
    ret
    
    
    
malloc:
    enter 0, 0
    mov rcx, [rbp + 16]; size
    push rcx
    call find_free_place
    cmp rax, -1
    jne .return
    add rsp, 8
    push rcx
    call sbrk
    add rsp, 8
.return:
    leave
    ret
    
    
free:
    enter 0, 0
    mov rcx, [rbp + 16];адресс который нам надо освободить
    lea rdi, [structs]
.for:
    cmp [rdi + 16], rcx; structs[i][index], rcx
    je .found
    jmp .not_found
.found:
    mov rax, 1
    jmp .end_for
.not_found:
    add rdi, 24
    jmp .for
.end_for:
    cmp rax, -1
    jne .good
    jmp .return
.good:
    mov qword[rdi], 0 ; structs[pos][used] = 0 = free
    mov rax, 0 ; good 
.return:
    leave
    ret

main:
    mov rbp, rsp; for correct debugging
    GET_DEC 8, [size]
    mov rbx, [size]
    push rbx
    call malloc
    PRINT_DEC 8, rax
    
    mov r8, rax
    add rsp, 8
    GET_DEC 8, [size]
    mov rdx, [size]
    push rdx
    call malloc
    add rsp, 8
    PRINT_CHAR `\n`
    PRINT_DEC 8, rax
    PRINT_CHAR `\n`
    
    
    push r8
    call free
    add rsp,8
    push qword[size]
    
    call malloc
    add rsp, 8
    PRINT_DEC 8, rax
    PRINT_CHAR `\n`
    
    
    push qword[size]
    call malloc
    PRINT_DEC 8, rax
    PRINT_CHAR `\n`
    add rsp, 8
    ret
    ; TODO вместо массива струтур использовать next
    ; TODO выделять адресса делящиеся на 8
