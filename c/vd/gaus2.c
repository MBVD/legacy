#include<stdio.h>
#include<stdlib.h>

double** create_matrix(int m, int n){
    double** matr = (double**) malloc(m*sizeof(double*));
    for (int i = 0; i<m; i++){
        matr[i] = (double*)malloc(n*sizeof(double));
        for (int j = 0; j<n; j++){
            scanf("%lf", &matr[i][j]);
        }
    }
    return matr;
}

void delete (double** matr, int n){
    for (int i = 0; i<n; i++){
        free(matr[i]);
    }
}

int min(int a, int b){
    if (a >= b){
        return b;
    }else{
        return a;
    }
}

void gauss(double** matr, int m, int n){
    for  (int i = 0; i<min(m, n); i++){
        for (int j = i+1; j<m; j++){
            double x = matr[j][i] / matr[i][i]; 
            for (int k = 0; k<n; k++){
                matr[j][k] -= matr[i][k] * x;
            }
        }
    }
}

int main(){
    int m, n; 
    scanf("%d%d", &m, &n);
    double** matrix = create_matrix(m, n);


    delete(matrix, m);
}