%include "io64.inc"

section .data
    nullptr equ 0
    printf_format db "%s, порядковый номер: %lld\n ", 10, 0
    node_word_size equ 9
    null equ 0

section .bss
    struc node
        word1 resb 9
        count resq 1
        order resq 0
        left resq 1
        right resq 1
    endstruc


section .text

    extern malloc
    extern printf
    extern strcpy
    extern strcmp

global main

;void createNode(word, order): ;rdi rsi
 create_node:
    enter 24,0

    mov [rbp - 8], rdi
    mov [rbp - 16], rsi

    mov rdi, node_size
    ALIGN_STACK
    call malloc
    UNALIGN_STACK 

    mov rdi, node_word_size
    ALIGN_STACK
    call malloc
    UNALIGN_STACK
    
    push qword[rbp-16]
    pop qword[rax+word1]
    mov rdi, [rax+word1]
    mov rsi, [rbp - 8]
    ALIGN_STACK
    call strcpy
    UNALIGN_STACK
    
    mov qword[rax + count], 1
    
    push qword[rbp-24]
    pop qword[rax + order]

    mov qword[rax + left], nullptr

    mov qword[rax+right], nullptr
    leave
    ret

;Node* insertNode(Node* root, char* word, int order) rdi, rsi, rcx
insertNode:
    enter 24, 0

    mov [rbp-8], rdi ; root
    mov [rbp-16], rsi ; word 
    mov [rbp-24], rdx ; order

    cmp rdi, nullptr
    je .createnode1
.createnode1:
    mov rdi, [rbp - 8]
    mov rsi, [rbp - 16]
    call create_node
    jmp .exit

    mov rdi, [rbp - 8] ;root
    mov rdi, [rdi+ word1] ; root->word
    mov rsi, rdi
    mov rdi, [rbp - 16]
    ALIGN_STACK
    call strcmp
    UNALIGN_STACK
    cmp rax, 0
    mov rdi, [rbp - 8]
    mov rsi, [rbp - 16]
    jl .root_left
    jg .root_right
    jmp .else 


.root_left:
    mov rdi, [rdi + left] 
    mov rsi, [rbp - 16]
    mov rdx, [rbp - 24]
    call insertNode
    mov rdi, [rbp-8]
    mov [rdi + left], rax
    mov rax, [rbp-8] ;return root
    jmp .exit
.root_right:
    mov rdi, [rdi + right] 
    mov rsi, [rbp - 16]
    mov rdx, [rbp - 24]
    call insertNode
    mov rdi, [rbp-8]
    mov [rdi + right], rax
    mov rax, [rbp-8] ;return root
    jmp .exit
.else:
    inc qword[rdi+count]
    mov rax, [rbp-8] ;return root
    jmp .exit
.exit:
    leave
    ret


;void print_tree(node* root) ;rdi 

print_tree: 
    enter 8, 0

    cmp rdi, nullptr
    je .end

    mov [rbp - 8], rdi

    mov rdi, [rdi + left] 
    call print_tree
        
    mov rdi, [rdi + count]
    cmp rdi, 1
    jne .end


    lea rdi, [printf_format]
    mov rsi, [rbp - 8]
    mov rsi, [rsi + word1]

    mov r8, [rbp - 8]
    mov rdi, [rbp-8]
    mov rsi, [rbp - 24]
    inc qword[rbp -16]
    mov rcx, [rbp-16]
    call insertNode
    mov rdx, [r8 + order]
    ALIGN_STACK
    call printf
    UNALIGN_STACK

    mov rdi, [rbp - 8]
    mov rdi, [rdi + right]
    call print_tree
    
.end:
    leave
    ret


; Node* parser(Node* root, )
parser:
    enter 32, 0

    mov [rbp - 8], rdi  ; root
    ;[rbp - 16] - order
    ;[rbp -24] - word1
    ;[rbp - 32] - ch
    ;rax - index 
    xor rax, rax 

    
.read_char_loop:
    GET_CHAR [rbp - 16]
    mov al, byte[rbp-16] 
    cmp al, `.` 
    je .end_loop 
    
    cmp al, `,`
    jne .else 

    mov rdi, [rbp-24]
    mov rdi, [rdi + rax] ;word index
    mov rdi, '/0'

    mov rdi, [rbp-8]
    mov rsi, [rbp - 24]
    inc qword[rbp -16]
    mov rcx, [rbp-16]
    call insertNode
    mov [rbp-8], rax
    mov rax, 0
    jmp .read_char_loop    

.else:
    mov rdi, [rbp - 24]
    mov rdi, [rdi + rax]
    mov rdi, [rbp - 32]
    inc rax

.end_loop:
    cmp rax, 0
    jng .end

    mov rdi, [rbp-24]
    mov rdi, [rdi + rax] ;word index
    mov rdi, '/0'

    mov rdi, [rbp-8]
    mov rsi, [rbp - 24]
    inc qword[rbp -16]
    mov rcx, [rbp-16]
    call insertNode
    mov [rbp - 8], rax

.end: 
    mov rax, [rbp - 8]
    leave
    ret


main:
    mov rbp, rsp; for correct debugging
    
    xor rdi, rdi
    call parser
    mov rdi, rax 
    call print_tree
    mov rax, 0
    ret