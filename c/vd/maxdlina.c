#include <stdio.h>
#include <stdlib.h>

int dlina(int* a, int n){
    int c = 1, mx = 1; 
    for (int i = 1; i<n; i++){
        if (a[i] >= a[i-1]){
            c++;
        }else{
            if (mx < c){
                mx = c;
            }
            c = 1;
        }
    }
    if (c > mx){
        return c;
    }
    return mx;
}

int main(){
    int n;
    scanf("%d", &n);
    int a[n];
    for (int i = 0; i<n; i++){
        scanf("%d", &a[i]);
    }
    int ans = dlina(a, n);
    printf("%d", ans);
}