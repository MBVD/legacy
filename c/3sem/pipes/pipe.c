#include "apue.h"
#include <unistd.h>  // Для функций pipe, fork, read, write

#define MAXLINE 4096  // Определяем максимальный размер строки

int main(void) {
    int n;
    int fd[2];
    pid_t pid;
    char line[MAXLINE];

    if (pipe(fd) < 0) {
        err_sys("ошибка вызова функции pipe");
    }

    if ((pid = fork()) < 0) {
        err_sys("ошибка вызова функции fork");
    } else if (pid > 0) { /* родительский процесс */
        close(fd[0]);  // Закрываем конец для чтения
        write(fd[1], "pipe3.привет\n", 13);  // Пишем все 12 байт
        close(fd[1]);  // Закрываем конец для записи, чтобы отправить EOF
    } else { /* дочерний процесс */
        close(fd[1]);  // Закрываем конец для записи
        n = read(fd[0], line, MAXLINE);  // Читаем из канала
        if (n > 0) {
            write(STDOUT_FILENO, line, n);  // Печатаем прочитанное
        }
        close(fd[0]);  // Закрываем конец для чтения
    }

    exit(0);
}
