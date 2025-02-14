#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BUF_MAX 256

char* readline(){
    int n = BUF_MAX, i = 0, c;
    char* buf = (char*)malloc(n*sizeof(char));
    while((c=getchar())!='\n'){
        if (i == n-1){
            n*=2;
            buf = (char*)realloc(buf, n*sizeof(char));
        }
        buf[i++] = c;
    }
    buf[i] = '\0';
    return buf;
}

int main(){
    char* s1 = readline();
    char* s2 = readline();
    int diff[BUF_MAX];
    for (int i = 0; i<BUF_MAX; i++){
        diff[i] = 0;
    }
    for (int i = 0; i<strlen(s1); i++){
        diff[s1[i]]++;
    }
    for (int i = 0; i<strlen(s2); i++){
        diff[s2[i]]--;
    }
    for (int i = 0; i<BUF_MAX; i++){
        if (diff[i] != 0)
        printf("1 otlivchaetsya ot 2 na %d %c \n", diff[i], (char)i);
    }
    free(s1);
    free(s2);
}