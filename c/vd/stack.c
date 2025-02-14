#include <stdio.h>
#include <stdlib.h>
#define MAX_LEN 256

struct stack{
    int buf[MAX_LEN];
    unsigned top;

};

int push (struct stack* st, int n){
    if (st->top == MAX_LEN){
        return -1;
    }
    st->buf[st->top++] = n;
    return 0;
}

int pop(struct stack* st){
    if (st->top == 0){
        return -1;
    }
    return st->buf[--st->top];
}

void print(struct stack s){
    for (int i = 0; i<s.top; i++){
        printf("%d ", s.buf[i]);
    }
    printf("\n");
}

int main(){
    struct stack s;
    int a, b;
    scanf("%d%d", &a, &b);
    push(&s, a);
    printf("%d", pop(&s));
}