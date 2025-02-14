%include "io64.inc"

;MAMA,MYLA,RAMU,RAMA,MYLA,MAMU.
;I,NE,CERKOV,I,NE,KABAK,NICHEGO,NE,SVYATO,NET,REBYATA,VSE,NE,TAK,VSE,NE,TAK,REBYATA.
;EH,RAS,DA,ESCHE,RAS,DA,ESCHE,MNOGO,MNOGO,MNOGO,MNOGO,RAS,DA,ESCHE,RAS,VSE,NE,TAK,REBYATA.
;A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A.



section .bss

struc node
    prevptr resq 1
    wordptr resq 1
    cntr resq 1
    nextptr resq 1
endstruc

section .data
nullptr equ 0
printf_str db "%d - %s;", 10, 0

section .text

extern malloc
extern strcmp
extern strcpy
extern printf

create_node: ; node* create_node(char* word, node* prev, node* next) rdi-word rsi-next
    enter 0, 0 ;rbp-8 word rbp - 16 next rbp-24 prev rbp-32 temp
    
    push rdi
    push rsi
    push rdx
    
    mov rdi, node_size
    ALIGN_STACK
    call malloc
    UNALIGN_STACK
    push rax
    
    mov rdi, [rbp - 32]
    mov rsi, [rbp - 8]
    mov [rdi + wordptr], rsi ; temp->word = word
    
    mov qword[rdi + cntr], 1    
       
    mov rsi, [rbp - 16]
    mov [rdi + nextptr], rsi ; temp->next = next
    
    mov rsi, [rbp - 24]
    mov [rdi + prevptr], rsi ;temp->prev = prev
    
    mov rax, [rbp - 32]
    
    leave
    ret

insert: ;node* insert(node* root, char* word)  rbp-8 - root rbp-16 - word rbp - 24 reserv_ root
    enter 0, 0
    
    push rdi
    push rsi
    
    mov rdi, [rbp - 8]
    cmp rdi, nullptr
    je .if1
    
    push rdi
    
    
    .while:
        mov rdi, [rbp - 8]
        mov rdi, [rdi + wordptr]
        mov rsi, [rbp - 16]
        ALIGN_STACK
        call strcmp
        UNALIGN_STACK
        cmp eax, 0
        jg .insertback
        je .noinsert
        jmp .continuesearch
    
    .continuesearch:
        mov rdi, [rbp - 8]
        cmp qword[rdi + nextptr], nullptr
        je .insertfront
        mov rdi, [rdi + nextptr]
        mov [rbp - 8], rdi
        jmp .while
    
    .insertback:
        mov rdi, [rbp - 16]
        mov rsi, [rbp - 8]
        mov rdx, [rsi + prevptr]
        
        ;PRINT_STRING "insert back "
        ;PRINT_STRING [rdi]
        ;NEWLINE
        
        call create_node
        mov rdi, [rbp - 8]
        mov rdi, [rdi + prevptr]
        mov qword[rdi + nextptr], rax
        mov rax, [rbp - 24]
        
        leave
        ret 
        
    .noinsert:
        mov rdi, [rbp - 8]
        inc qword[rdi + cntr]
        mov rax, [rbp - 24]
        
        ;mov rdi, [rbp - 16]
        ;PRINT_STRING "counter increase "
        ;PRINT_STRING [rdi]
        ;NEWLINE
        
        leave
        ret
        
    .insertfront:
        mov rdi, [rbp - 16]
        mov rsi, nullptr
        mov rdx, [rbp - 8]
        
        ;PRINT_STRING "insert front "
        ;PRINT_STRING [rdi]
        ;NEWLINE
        
        call create_node
        mov rdi, [rbp - 8]
        mov qword[rdi + nextptr], rax
        mov rax, [rbp - 24]
        
        leave
        ret
    
    .if1: ;if(root == NULL)
        mov rdi, [rbp - 16]
        mov rsi, nullptr
        mov rdx, nullptr
        
        ;PRINT_STRING "create node "
        ;PRINT_STRING [rdi]
        ;NEWLINE
        
        call create_node
        
        leave
        ret

parce:
    enter 0, 0 ;rbp - 8 = word_array, rbp - 16 = word[8], rbp-24 temp
    
    mov rdi, 8 * 8
    ALIGN_STACK
    call malloc
    UNALIGN_STACK
    push rax ;word_array[8]
    
    xor rcx, rcx
    .for1:
        cmp rcx, 8
        jge .endfor1
        mov rdi, [rbp - 8]         
        mov qword[rdi + 8 * rcx], nullptr
        inc rcx
        jmp .for1
    
    .endfor1:
        xor bl, bl ; char c
        mov rdi, 9
        ALIGN_STACK
        call malloc ;char word[8]
        UNALIGN_STACK
        push rax ;word[8]

        
        xor rcx, rcx
        xor r12, r12
    .while1:
        GET_CHAR bl
        cmp bl, '.'
        mov rdx, rcx
        je .if2
        
        cmp bl, ','
        mov rdx, rcx
        je .if1
        
        mov rdi, [rbp - 16]
        mov [rdi + rcx], bl
                        
        inc rcx
        jmp .while1
        
        .if1:
            mov r12, 1
            jmp .vstavka_probelov
            
        .if2:
            mov r12, 2
            jmp .vstavka_probelov
        
        .vstavka_probelov:
            cmp rdx, 8
            jge .konets_vstavki
            mov rdi, [rbp - 16]
            mov byte[rdi + rdx], ' '
            inc rdx
            jmp .vstavka_probelov
        .konets_vstavki:
            mov byte[rdi + 8], `\0`
            mov rdi, 9
            
            push rcx
            ALIGN_STACK
            call malloc
            UNALIGN_STACK
            pop rcx
            
            push rax ;temp
            mov rdi, rax
            mov rsi, [rbp - 16]
            
            push rcx
            ALIGN_STACK
            call strcpy
            UNALIGN_STACK
            pop rcx
            
            mov rdi, [rbp - 8]
            dec rcx
            mov rdi, [rdi + 8 * rcx]
            mov rsi, [rbp - 24]
            
            push rcx
            call insert
            pop rcx
            
            mov rdi, [rbp - 8]
            mov [rdi + rcx * 8], rax
            inc rcx
            xor rcx, rcx
            cmp r12, 1
            pop rax ;temp
            je .while1
                
            mov rax, [rbp - 8]
            leave
            ret

print_node_array: ;void print_node_array(node** word_array) rdi - word_array
    enter 0, 0 ;rbp - 8 word_array rbp - 16 present_root
    
    push rdi
    
    mov r13, 1   
    .for2:
        cmp r13, 20
        jg .end
        xor rcx, rcx
        .for1:
            cmp rcx, 8
            jge .end_for1
            mov rdi, [rbp - 8]
            mov rdi, [rdi + rcx * 8]
            push rdi
            
            .while2:
                mov rdi, [rbp - 16]
                cmp rdi, nullptr
                je .end_while2
                mov rsi, [rbp - 16]
                mov rax, [rsi + cntr]
                cmp rax, r13
                jne .next_root
                mov rsi, [rsi + wordptr]
                mov rsi, rax
                mov rdx, [rbp - 16]
                mov rdx, [rdx + wordptr]
                lea rdi, [printf_str]
                mov rax, 0
                push rcx
                ALIGN_STACK
                call printf
                UNALIGN_STACK
                pop rcx
                
                .next_root:
                    pop rdi
                    mov rdi, [rdi + nextptr]
                    push rdi
                    jmp .while2
                
            .end_while2:
            pop rdi
            inc rcx
            jmp .for1
        .end_for1:
            inc r13
            jmp .for2
            
    .end:
        leave
        ret

global CMAIN
CMAIN:
    mov rbp, rsp; for correct debugging
    call parce
    mov rdi, rax
    call print_node_array
    ret