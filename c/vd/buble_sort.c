#include <stdio.h>

int count(int* a, int n){
  int cnt = 1, el = 0, mx = 0;
  for(int i = 0; i<n-1; i++){
    if (a[i] == a[i + 1]){
      cnt++;
    }   else {
      if (cnt > mx){
        el = a[i];
        mx = cnt;
      }
      cnt = 1;
    }
  }
  if (cnt > mx){
    el = a[n-1];
  }
  printf("%d", el); 
}

int number(char* str){
  int number = 0;
  for (int i = 0; i< strlen(str); i++){
    number = number * 16;
    if (str[i] >= '0' && str[i] <= '9'){
      number += str[i]-'0';
    }
    if (str[i] >= 'A' && str[i] <= 'F'){
      number += str[i]-'A' + 10;
    }
  }
  return number;
}

int main() {
  int a[5] = {1, 2, 3, 4, 4};
  printf("%d", *(a+1));
  printf("%d", a[5] == *(a + 5));
  // count(a, 5);
}