#include <stdio.h>
#include <stdlib.h>

struct stats {
    int num_views;  // 4 байта
    short sum;      // 2 байта
    char pad[2];    // 2 байта (для выравнивания до границы 4 байт)
};


typedef struct system_f_alternative {
    char a;
    struct stats e;
    long d;
    int * b;
    int c[3];
    short f;
} system_f_alternative;

int main(){
    printf("%d", sizeof(system_f_alternative));
}