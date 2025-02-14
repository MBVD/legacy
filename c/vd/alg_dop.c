#include <stdio.h>

int alg_matr(int ** a, int n, int i, int j){
  int index1 = 0, index2 = 0;
  int **buf;
  for (int k = 0; k<n; k++){
    if (k == i){
      continue;
    }
    index1 = 0;
    for (int l = 0; l<n; l++){
      if (l == j){
        continue;
      }
      buf[index1][index2] = a[k][l];
      index1 ++;
    }
    index2++;
  }
}

int main(){

}