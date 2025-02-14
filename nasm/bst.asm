%include "io64.inc"

section .data
    nullptr equ 0
    printf_format db "%s, порядковый номер: %d ", 10, 0
    null equ 0

section .bss
    struc node
        word1 resq 1
        count resq 1
        order resq 1
        left resq 1
        right resq 1
    endstruc
    


section .text

    extern malloc
    extern printf
    extern strcpy
    extern strcmp

;        %macro SAVE 0
 ;       push rdi
  ;      push rsi
   ;     push rdx
    ;    push rcx
     ;   push r8
      ;  push r9
    ;%endmacro

;    %macro RESTORE 0
 ;       pop r9
  ;      pop r8
   ;     pop rcx
    ;    pop rdx
     ;   pop rsi
      ;  pop rdi
    ; %endmacro



global CMAIN

;void createNode(word, order): ;rdi rsi
create_node:
    enter 24,0

    mov [rbp - 8], rdi
    mov [rbp - 16], rsi

    mov rdi, node_size
   ; SAVE
    ALIGN_STACK
    call malloc ;malloc for root 
    UNALIGN_STACK
    ;RESTORE
    mov [rbp - 24], rax
    
    mov rdi, 9
    ;SAVE
    ALIGN_STACK
    call malloc   ; malloc for word1
    UNALIGN_STACK
    ;RESTORE
    mov rdi, [rbp - 24]
    mov [rdi+word1], rax ; 
    mov rdi, [rdi + word1]
    mov rsi, [rbp - 8]; word
    
      
    
    ;SAVE
    ALIGN_STACK
    call strcpy   
    UNALIGN_STACK
    ;RESTORE    
        
    mov rax, [rbp-24]
    mov qword[rax + count], 1
    
    push qword[rbp-16];order
    pop qword[rax + order] ;tmp->order=order

    mov qword[rax + left], nullptr

    mov qword[rax+right], nullptr
    leave
    ret

;Node* insertNode(Node* root, char* word, int order) rdi, rsi, rcx
insertNode:
    enter 24, 0

    mov [rbp-8], rdi ; root
    mov [rbp-16], rsi ; word 
    mov [rbp-24], rcx ; order

    cmp rdi, nullptr
    je .createnode1
    
    mov rdi, [rbp - 8] ;root
    mov rdi, [rdi+ word1] ; root->word
    mov rsi, rdi
    mov rdi, [rbp - 16]
    
   
    ;SAVE
    ALIGN_STACK
    call strcmp
    UNALIGN_STACK
    
       
    ;RESTORE
    mov rdi, [rbp - 8]
    mov rsi, [rbp - 16]
    
   ; PRINT_DEC 8, rax
   ; NEWLINE
    cmp eax, 0 
    jl .root_left
    jg .root_right
    jmp .else 


.createnode1:
    mov rdi, [rbp - 16]
    mov rsi, [rbp - 24]
    call create_node
    jmp .exit


.root_left:
    mov rdi, [rdi + left] 
    mov rsi, [rbp - 16];word
    mov rcx, [rbp - 24]; order
    call insertNode
    mov rdi, [rbp-8]
    mov [rdi + left], rax
    mov rax, [rbp-8] ;return root
    jmp .exit
.root_right:
    
    mov rdi, [rdi + right] 
    mov rsi, [rbp - 16]
    mov rcx, [rbp - 24]
    call insertNode
    mov rdi, [rbp-8]
    mov [rdi + right], rax
    mov rax, [rbp-8] ;return root
    jmp .exit
.else: ; = strcmp = 0
    inc qword[rdi+count]
    mov rax, [rbp-8] ;return root
    jmp .exit
.exit:
    leave
    ret


;void print_tree(node* root) ;rdi 

;void printTree(Node* root) rdi
print_tree:
    enter 8, 0
    mov [rbp-8], rdi

    cmp rdi, nullptr
    jne .else
    jmp .end
    

.else:
    mov rdi, [rbp-8]
    mov rdi, [rdi+left]
    call print_tree
    
    mov rdi, [rbp-8]
    mov rdi, [rdi+count]
    cmp rdi, 1 ;count = 1
    jne .inright
    
    mov rcx, [rbp-8]
    lea rdi, [printf_format]
    mov rsi, [rcx+word1]
    mov rdx, [rcx+order]
    
    ALIGN_STACK
    call printf
    UNALIGN_STACK
    
.inright:
    mov rdi, [rbp-8]
    mov rdi, [rdi+right]
    call print_tree

.end:
    leave
    ret




; Node* parser(Node* root, )
parser:
    enter 32, 0
    mov qword[rbp-16], 0 ; order = 
    mov [rbp - 8], rdi  ; root
    ;[rbp - 16] - order
    ;[rbp -24] - word1
    ;[rbp - 32] - ch
    ;rax - index 
    mov rdi, 9
    ;SAVE
    ALIGN_STACK
    call malloc
    UNALIGN_STACK
    ;RESTORE
    mov [rbp - 24], rax
    xor rax, rax 
    

    
.read_char_loop:
    GET_CHAR bl
 
 
    cmp bl, '.'
    je .end_loop
    
    cmp bl, ','
    jne .else 
    
.cycle:
    cmp rax, 8
    je .endcycle
    mov rdi, [rbp-24]
    mov byte[rdi+rax], ` ` ;word[index]
    inc rax
    jmp .cycle
    
.endcycle:
    mov rdi, [rbp-24] 
    mov byte[rdi + 9], `\0`

    mov rdi, [rbp-8] ;root
    mov rsi, [rbp - 24] ; word1
    inc qword[rbp -16] ;++order
    mov rcx, [rbp-16] 
    call insertNode
    mov [rbp-8], rax
    mov rax, 0
    jmp .read_char_loop    


.else:  
    mov rdi, [rbp - 24]   ;word   
    cmp rax, 8
    jnl .read_char_loop
    mov byte[rdi+rax], bl
    inc rax
    jmp .read_char_loop    

.end_loop:
    cmp rax, 0
    jng .end
    
    
.cycle1: ;spaces
    cmp rax, 8
    je .endcycle1
    mov rdi, [rbp-24]
    mov byte[rdi+rax], ` ` 
    inc rax
    jmp .cycle1
.endcycle1:
    mov rdi, [rbp-8]; root
    mov rsi, [rbp - 24]; word
    inc qword[rbp -16]; order++
    mov rcx, [rbp-16]
    call insertNode
    mov [rbp - 8], rax

    
.end: 
    mov rax, [rbp - 8]
    leave
    ret



CMAIN:
    mov rbp, rsp; for correct debugging
    enter 8, 0
    
    mov qword[rbp-8], nullptr    ;root = null
    mov rdi, [rbp-8]        
    call parser
    mov [rbp-8], rax        
    mov rdi, [rbp-8]        
    call print_tree
    xor rax, rax    
    leave
    ret 
    