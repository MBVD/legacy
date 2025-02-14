#include <stdio.h>

void perevod(int n){
  if (n == 0){
    return; 
  }
  int a = n%16;
  perevod(n/16);
  if (a < 10){
    printf("%d", a);
  }else{
    printf("%c", 'A' + (a - 10));
  }
}

int main(){
  perevod(21);
}