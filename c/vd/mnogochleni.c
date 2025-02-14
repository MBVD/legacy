#include <stdio.h>
#include <stdlib.h>

int max(int a, int b){
    if (a > b){
        return a;
    }else{
        return b;
    }
}
int* summa (int* a, int* b, int n, int m){
    int mn, mx;
    mx = max(n, m);
    int* sm = calloc(mx, sizeof(int));
    for (int i = 0; i<mx; i++){
        if (i >= n){
            sm[i] = b[i];
        } else if (i >= m){
            sm[i] = a[i];
        } else{
            sm[i] = a[i] + b[i];
        }
    }
    return sm;
}

int main(){
    int n, m;
    scanf("%d%d", &n, &m);
    int a[n], b[m];
    for (int i = 0; i<n; i++){
        scanf("%d", &a[i]);
    }
    for (int i = 0; i < m; i++){
        scanf("%d", &b[i]);
    }
    int* mult = summa(a, b, n, m);
    for (int i = 0; i<max(n, m); i++){
        printf("%dx^%d ", mult[i], i);
    }
    putchar('\n');
    free(mult);
}