%include "io64.inc"
section .bss
    brk resb 8192            ;массив куда складываем
    structs resb 25 * 1000     ; массив структур
struc node
    used resb 1; 1
    adress resq 1; 9
    sz resq 1; 17
    next resq 1; 25
endstruc
section .data
    structs_size dq -1
    brk_size dq 0
    size dq 0
    null_ptr equ 0
    head dq 0
    tail dq 0
    string dq 0
section .text
global main

create_node: ;node* create_node(used, adress, sz)
    enter 24, 0
    mov [rbp - 8], rax
    mov rax, [rbp + 16]; used
    mov [rbp - 16], rdi
    mov rdi, [rbp + 24]; adress
    mov [rbp - 24], rbx
    mov rbx, [rbp + 32]; sz
    mov [rbp - 32], rcx
    inc qword[structs_size]
    mov rcx, qword[structs_size]
    imul rcx, rcx, 25
    lea rsi, [structs + rcx]
    mov [rsi + used], al
    mov [rsi + adress], rdi
    mov [rsi + sz], rbx
    mov qword[rsi + next], null_ptr
    
    mov rax, rsi
    mov rdi, [rbp - 16]
    mov rbx, [rbp - 24]
    mov rcx, [rbp - 32]
    
    leave 
    ret
sbrk: ; выделим новую память из массива brk (suze)
    enter 24, 0
    mov [rbp - 8], rcx
    mov rcx, [rbp+16]; размер выделяемой памяти в байтах
    and rcx, 7
    cmp rcx, 0
    mov rcx, [rbp + 16]
    je .mod
    jne .n_mod
.n_mod:
    shr rcx, 3
    inc rcx
    shl rcx, 3
.mod:
    mov [rbp - 16], rbx
    mov rbx, qword[brk_size]
    add qword[brk_size], rcx
    lea rax, [brk + rbx]
    
    push rcx; sz
    push rax; adress
    push 1; used
    
    call create_node
    add rsp, 24
    
    
    mov [rbp - 24], r9
    mov r9, qword[head]
    cmp qword[head], null_ptr
    je .fill
    jmp .dont_fill
.fill:
    mov qword[head], rax ; head -> tmp
    mov qword[tail], rax 
    jmp .after
.dont_fill:
    mov r9, [tail]
    mov [r9 + next], rax; tail -> next = tmp
    mov [tail], rax; tail = tmp
.after:
    
    lea rax, [brk + rbx] 
    
    mov rbx, [rbp - 16]
    mov r9, [rbp - 24]
    mov rcx, [rbp - 8]
    leave
    ret
    
find_free_place:; возвращает -1 если ничего не находит и аддрес если есть свободное пространство
    enter 0, 0
    mov rax, -1
    mov rcx, [rbp + 16]; sz
    and rcx, 7
    cmp rcx, 0
    mov rcx, [rbp + 16]
    je .mod
    jne .n_mod
.n_mod:
    shr rcx, 3
    inc rcx
    shl rcx, 3
.mod:
    mov rdx, [head]
.for:
    cmp rdx, null_ptr
    je .end_for
    cmp byte[rdx + used], 0; used == 0
    je .not_used
    jmp .ignore
.not_used:
    cmp [rdx + sz], rcx; cmp structs[rdx][size], size
    jge .found
    jmp .ignore
.found:
    ;mov rdx, qword[rdx + next]; rdx = rdx -> next
    mov byte[rdx + used], 1; used = 1
    mov r8, [rdx + sz]
    mov [rdx + sz], rcx; sz = size
    sub r8, rcx; сколько осталось свободного места
    mov rsi, [rdx+adress]; первоначальный адресс блока
    add rsi, rcx; сдвинули адресс
    
    cmp r8, 0
    jne .create
    jmp .ignore_creation
.create:
    push r8
    push rsi
    push 0
    
    call create_node
    
    ;mov [head], rax
    mov r8, [rdx + next]
    mov [rax + next], r8; new_block->next = tmp -> next
    mov [rdx + next], rax; tmp -> next = new_block

    add rsp, 24
.ignore_creation:
    ; добавить новый элемент в массив structs который будет содержать index: structs[rdx][adress] + 1
    ; и size = [rdi + 16] - rcx
    ; used = 0
    mov rax, qword[rdx+adress]
    jmp .end_for 
.ignore:    
    mov rdx, qword[rdx + next] 
    jmp .for
.end_for:
    leave
    ret
    
    
    
malloc:
    enter 0, 0
    mov rcx, [rbp + 16]; size
    and rcx, 7
    cmp rcx, 0
    mov rcx, [rbp + 16]
    je .mod
    jne .n_mod
.n_mod:
    shr rcx, 3
    inc rcx
    shl rcx, 3
.mod:
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
    enter 16, 0
    mov [rbp - 8], rcx
    mov rcx, [rbp + 16];адресс который нам надо освободить
    mov rdi, [head]
    mov r13, [head]; prev
    mov [rbp-16], r13 ;prev
.for:
    cmp [rdi + adress], rcx; structs[i][index], rcx
    je .found
    jmp .not_found
.found:
    mov rax, 1
    jmp .end_for
.not_found:
    mov [rbp-16], rdi
    mov rdi, qword[rdi + next]
    jmp .for
.end_for:
    cmp rax, -1
    jne .good
    jmp .return
.good:
    mov r13, [rbp - 16]
    cmp byte[r13 + used], 0
    je .merge_prev
    jmp .ignore
.merge_prev:
    mov r15, [rdi + next]
    mov [r13 + next], r15; prev -> next = tmp -> next
    mov r15, [rdi + sz]
    add [r13 + sz], r15; prev -> sz += tmp -> sz
    mov rdi, r13; tmp = prev
.ignore:
    mov r13, [rdi + next]; tmp -> next
    cmp r13, 0
    je .ignore2
    cmp byte[r13 + used], 0
    je .merge_next
    jmp .ignore2
.merge_next:
    mov r15, [r13 + sz]; tmp -> next -> size
    add [rdi + sz], r15; tmp += tmp -> next -> size
    mov r15, [r13 + next]
    mov [rdi + next], r15; tmp -> next = tmp -> next -> next
.ignore2:
    mov byte[rdi + used], 0 ; structs[pos][used] = 0 = free
    
    mov rax, 0 ; good 
.return:
    mov rcx, [rbp - 8]
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
    add rax, qword[size]
    PRINT_CHAR '-'
    PRINT_DEC 8, rax
    NEWLINE
    add rsp, 8
    
    GET_DEC 8, [size]
    mov rdx, [size]
    push rdx
    call malloc
    add rsp, 8
    PRINT_DEC 8, rax
     mov r9, rax
    add rax, qword[size]
    PRINT_CHAR '-'
    PRINT_DEC 8, rax
    NEWLINE
    
    push r8
    call free
    add rsp,8
    
    push r9
    call free
    add rsp,8
    
    
    
    
    GET_DEC 8, [size]
    push qword[size]
    call malloc
    add rsp, 8
    PRINT_DEC 8, rax
    add rax, qword[size]
    PRINT_CHAR '-'
    PRINT_DEC 8, rax
    NEWLINE
    
    GET_DEC 8, [size]
    push qword[size]
    call malloc
    add rsp, 8
    PRINT_DEC 8, rax
    add rax, qword[size]
    PRINT_CHAR '-'
    PRINT_DEC 8, rax
    NEWLINE
 
    ret
    ; TODO вместо массива струтур использовать next
    ; TODO выделять адресса делящиеся на 8
    ; TODO решить проблему сливания блоков
