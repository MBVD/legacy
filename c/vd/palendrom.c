#include <stdio.h>
#include <stdlib.h>

int palendrom (int* a){
    int raz = 0;
    while (a[raz++] != NULL);
    raz++;
    for (int i = 0; i<raz/2; i++){
        if (a[i] != a[raz - i]){
            return 0;
        }
    }
    return 1;
}

int main(){
    int n = getchar();
    printf('hello');
    int* a = (int*)calloc(n, sizeof(int));
    for (int i = 0; i<n; i++){
        int x = getchar();
        a[i] = x;
    }
    if (palendrom(a)){
        printf('true');
    }else{
        printf('false');
    }
    printf('hello2');
}