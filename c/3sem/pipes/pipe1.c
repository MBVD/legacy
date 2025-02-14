#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Каналы для синхронизации
static int pfd1[2], pfd2[2];

// Функции для синхронизации
void TELL_WAIT() {
    if (pipe(pfd1) < 0 || pipe(pfd2) < 0) {
        perror("pipe error");
        exit(1);
    }
}

void TELL_PARENT(pid_t ppid) {
    if (write(pfd2[1], "c", 1) != 1) {  // Отправка символа родителю
        perror("write error");
        exit(1);
    }
}

void WAIT_PARENT() {
    char c;
    if (read(pfd1[0], &c, 1) != 1) {  // Ожидание символа от родителя
        perror("read error");
        exit(1);
    }
    if (c != 'p') {
        fprintf(stderr, "WAIT_PARENT: неверный символ '%c'\n", c);
        exit(1);
    }
}

void TELL_CHILD(pid_t pid) {
    if (write(pfd1[1], "p", 1) != 1) {  // Отправка символа потомку
        perror("write error");
        exit(1);
    }
}

void WAIT_CHILD() {
    char c;
    if (read(pfd2[0], &c, 1) != 1) {  // Ожидание символа от потомка
        perror("read error");
        exit(1);
    }
    if (c != 'c') {
        fprintf(stderr, "WAIT_CHILD: неверный символ '%c'\n", c);
        exit(1);
    }
}

// Функция для поочередной печати символов строки
void charatatime(char *str) {
    char *ptr;
    int c;
    setbuf(stdout, NULL);  // Устанавливаем небуферизованный вывод
    for (ptr = str; (c = *ptr++) != 0; ) {
        putc(c, stdout);   // Печатаем символ за символом
    }
}

int main(void) {
    pid_t pid; 
    
    TELL_WAIT();  // Подготовка каналов для синхронизации
    
    if ((pid = fork()) < 0) {
        perror("ошибка вызова функции fork");
        exit(1);
    } else if (pid == 0) {  // Дочерний процесс
        WAIT_PARENT();  // Ждем сигнала от родителя
        charatatime("От дочернего процесса\n");
        TELL_PARENT(getppid());  // Сообщаем родителю, что закончили вывод
        exit(0);
    } else {  // Родительский процесс
        charatatime("От родительского процесса\n");
        TELL_CHILD(pid);  // Сообщаем дочернему процессу, что можно начинать
        WAIT_CHILD();  // Ждем, пока дочерний процесс закончит
    }

    if (waitpid(pid, NULL, 0) != pid) {  // Ожидание завершения дочернего процесса
        perror("ошибка вызова функции waitpid");
    }

    exit(0);
}
