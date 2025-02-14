%include "io64.inc"
%include "macro.inc"
%include "perv.inc"
%include "parser.inc"
%include "helper.inc"
%include "create_node.inc"

section .text
extern malloc
global main  

main:
    mov rbp, rsp; for correct debugging
    call readline
    PRINT_DEC 8, rsp
    PRINT_CHAR ' ' 
    PRINT_DEC 8, rbp
    NEWLINE 
    mov rdx, rax
    xor rcx, rcx
    PRINT_STRING 'Our string : '
    push rax
    call print_string
    PRINT_STRING "our words adress: "
    PRINT_DEC 8, rax
    NEWLINE
    add rsp, 8
    
    push ' '
    push rax
    call split_string
    add rsp, 16
    
    push rax
    call parse
    add rsp, 8
    
    push rax
    call print_tree
    add rsp, 8
    
    NEWLINE
    
    push rax
    call perv_tree_node
    add rsp, 8
    
    push rax
    call print_tree
    add rsp, 8
    NEWLINE
    ret
