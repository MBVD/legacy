#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 256

typedef struct queue {
    int buf[MAX_LEN];
    int len;
} queue;

int push (queue* q, int n){
    if (q->len == MAX_LEN){
        return -1;
    }
    q->buf[q->len++] = n;
    return 0;
}

int pop (queue* q){
    if (q -> len == 0){
        return -1;
    }
    int res = q -> buf[0];
    for (int i = 0; i<q->len-1; i++){
        q->buf[i] = q->buf[i+1];
    }
    q -> len--;
    return res;
}

int main(){
    queue q = {.len = 0};
    push(&q, 3);
    printf("%d",pop(&q));
}