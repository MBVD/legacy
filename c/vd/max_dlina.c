#include <stdio.h>
#include <stdlib.h>
 
int dl(int* mas, int n) {
    int maxdl = 1;
    int curdl = 1;
 
    for (int i = 1; i < n; i++) {
        if (mas[i] > mas[i - 1]) {
            curdl++;
        } else {
            if (curdl > maxdl) {
                maxdl = curdl;
            }
            curdl = 1;
        }
    }
    if (curdl > maxdl) {
        maxdl = curdl;
    }
    return maxdl;
}
 
int main() {
    int n;
    printf("Введите кол-во элементов: ");
    scanf("%d", &n);
 
    int* mas = (int*)malloc(n * sizeof(int));
    if (mas == NULL) {
        printf("Ошибка при выделении памяти\n");
        return 1;
    }
 
    printf("Введите элементы:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &mas[i]);
    }
 
    int maxdl = dl(mas, n);
 
    printf("Длина мах отрезка возрастания:  %d\n", maxdl);
 
    free(mas); // Free the dynamically allocated memory
 
    return 0;
}