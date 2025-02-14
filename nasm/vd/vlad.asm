section .data
    nullptr equ 0
    printf_format db "%lld ", 10, 0
    node_word_size equ 9

section .bss
    struc node
        word resq 9 
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

%macro ALIGN_STACK 0
        enter 0, 0
        and rsp, -16
%endmacro

%macro UNALIGN_STACK 0
        leave
%endmacro



;Node* createNode(char* word, int order) {
;    Node* newNode = (Node*)malloc(sizeof(Node));
;    strcpy(newNode->word, word);
;    newNode->count = 1;
;    newNode->order = order;
;    newNode->left = newNode->right = NULL;
;    return newNode;
;}



 void createNode(word, order): ;rdi rsi
 create_node:
    enter 16,0

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
    pop qword[rax+word]
    mov rdi, rax+word
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
