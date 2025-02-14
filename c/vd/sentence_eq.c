#include <stdio.h>
#include <stdlib.h>

int eq (char** a, char** b){
    int n = 0, m = 0;
    while (a[n++]!=NULL);
    while (b[m++]!=NULL);
    if (n > m){
        return 1;
    }
    if (n < m){
        return -1;
    }
    for (int i = 0; i<n; i++){
        if (strcmp(a[i], b[i])){
            return strcmp(a[i], b[i]);
        }
    }
    return 0;
}