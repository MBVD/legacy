#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUF_MAX 256

char* readline(){
  int n = BUF_MAX, i = 0, c;
  char* buf = (char*)malloc(n*sizeof(char));
  while((c=getchar())!='\n'){
    if (i == n-1){
        n*=2;
        buf = (char*)realloc(buf, n*sizeof(char));
    }
    buf[i++] = c;
  }
  buf[i] = '\0';
  return buf;
}

int main() {
  FILE *file = fopen("1.txt", "r+");
  char* word = readline();
  int n = BUF_MAX, m = BUF_MAX;
  char** sentence = (char**)malloc(n*sizeof(char*));
  for (int i = 0; i<n; i++){
    sentence[i] = malloc(m*sizeof(char));
  }
  if (file == NULL) {
    return 1;
  }
  int ch, i = 0, j = 0;
  ch = fgetc(file);
  while(ch != EOF){
    while (ch != EOF && ch != "\n" && ch != " ") {
      sentence[i][j++] = fgetc(file);
      if (j == m-1){
        m*=2;
        sentence[i] = realloc(sentence[i], m*sizeof(char));
      }
      ch = fgetc(file);
    }
    i++;
    if (i == n-1){
      n*=2;
      sentence = realloc(sentence, n*sizeof(char));
    }
  }
  for (int i = 0; i<n; i++){
    if (strcmp(sentence[i], word) != 0){
      fprintf(file, "%s ", sentence[i]);
    }
  }
  fclose(file);
}