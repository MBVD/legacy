#include <unistd.h>
#define BUF_MAX 256

void err_sys(const char* x) { 
  perror(x); 
  exit(1); 
}

int main() {
  int n;
  char buf[BUF_MAX];
  while ((n = read(STDIN_FILENO, buf, BUF_MAX) != 0)){
    if (write(STDOUT_FILENO, buf, n) != n){
      err_sys("write error");
    }
  }
}