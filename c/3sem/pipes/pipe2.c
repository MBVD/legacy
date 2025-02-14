#include "apue.h"
#include <sys/wait.h>
#include <string.h> // Для функции strlen

/* программа постраничного просмотра по умолчанию */
#define DEF_PAGER "/bin/more"

int main(int argc, char *argv[])
{
    int n;
    int fd[2];
    pid_t pid;
    char *pager, *argv0;
    char line[MAXLINE];
    FILE *fp;

    if (argc != 2)
        err_quit("Использование: a.out <pathname>");

    if ((fp = fopen(argv[1], "r")) == NULL)
        err_sys("невозможно открыть %s", argv[1]);

    if (pipe(fd) < 0)
        err_sys("ошибка вызова функции pipe");

    if ((pid = fork()) < 0) {
        err_sys("ошибка вызова функции fork");
    } else if (pid > 0) {  /* родительский процесс */
        close(fd[0]);      /* закрыть дескриптор для чтения */
        /* родительский процесс копирует argv[1] в канал */
        while (fgets(line, MAXLINE, fp) != NULL) {
            n = strlen(line);
            if (write(fd[1], line, n) != n)
                err_sys("ошибка записи в канал");
        }
        if (ferror(fp))
            err_sys("ошибка вызова функции fgets");
        close(fd[1]);      /* закрыть дескриптор для записи */
        if (waitpid(pid, NULL, 0) < 0)
            err_sys("ошибка вызова функции waitpid");
        exit(0);
    } else {               /* дочерний процесс */
        close(fd[1]);      /* закрыть дескриптор для записи */
        if (fd[0] != STDIN_FILENO) {
            if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
                err_sys("ошибка переназначения канала на stdin");
            close(fd[0]);  /* уже не нужен после вызова dup2 */
        }

        /* определить аргументы для execl() */
        if ((pager = getenv("PAGER")) == NULL)
            pager = DEF_PAGER;
        if ((argv0 = strrchr(pager, '/')) != NULL)
            argv0++;       /* перейти за последний слеш */
        else
            argv0 = pager; /* если нет слеша, использовать все значение как имя */

        /* Выполнить постраничный просмотр */
        if (execl(pager, argv0, (char *)0) < 0)
            err_sys("ошибка вызова функции execl");
    }
    exit(0);
}
