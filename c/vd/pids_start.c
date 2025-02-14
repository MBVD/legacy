#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define BUF_MAX 256

int main() {
  char buf[BUF_MAX];
  int status;
  while (fgets(buf, BUF_MAX, stdin) != NULL){
    if (buf[strlen(buf) - 1] == '\n'){
      buf[strlen(buf) - 1] = '\0';
    }
    pid_t pid;
    if ((pid = fork()) < 0){
      perror("error creation fork");
      exit(0);
    } else if (pid == 0) { // Child process
      execlp(buf, buf, NULL);
      perror("error with execution");
      exit(1);
    } else { // Parent process
      if (waitpid(pid, &status, 0) < 0) {
        perror("error waiting");
        return 1;
      }
    }
  }
  exit(0);
}