#include <stdio.h>
#include <stdlib.h>

void strdev(float* a, int n){
    int i = 0; 
    while(a[i++]==0);
    float dev = a[i];
    for (int i = 0; i<n; i++){
        a[i] /= dev;
    }
    return a;
}

float** gauss(float** a){ // только нужно передать массив котоыр будет содержать элементы отличные от нуля, потом выйдем премиум версия 
    int n = 0;
    while(a[n++]!=NULL);
    for (int i = 0; i<n; i++){
        strdev(a[i], n); // привел строку в вид где есть 1 на 1 месте если выполняется условие функции gauss
        for (int j = i+1; j < n; j++){
            for (int k = 0; k<n; k++){
                a[j][k] -= a[j][0] * a[i][k]; // вычитаем из строки строку выше чтоб получилось 0 в 1 не нулевом элементе верхней строки
            }
        }
    }
}
