%include "io64.inc"
section .bss
    arr resq 10
section .text
global main
main:
    GET_DEC 8, rbx
    push rbx,
    push 10
    push 0
    push [arr]
    call bin_poisk
    ret
    
bin_poisk:
    enter 8, 0
    test [rbp+16], [rbp+24]
    je .end_l
    
    push q
    leave
    ret
    
; int bin_poisk(int* arr ,int l, int r, int x){
;   if (r-l <= 0){
;       return l;
;   int m = (l+r)/2;
;   if (arr[m] => x){
;       return bin_poisk(arr, l, m-1, x);
;   }else{
;       return bin_poisk(arr, m+1, r, x);
;   }
; 