#include <stdio.h>

extern float maximum(float* arr, int n);

int main(){
    float arr[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0};
    int n = sizeof(arr) / sizeof(float);
    printf("%f\n", maximum(arr, n));
}