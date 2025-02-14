#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    int pipefd[2];
    pid_t pid1, pid2, pid3;
    int status;
    int file_fd;

    // Создаем пайп для передачи данных между pr1 и pr2
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Запускаем pr1 arg1 arg2
    pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // В дочернем процессе перенаправляем stdout в pipe
        close(pipefd[0]); // Закрываем ненужный конец для чтения
        dup2(pipefd[1], STDOUT_FILENO); // Перенаправляем stdout в pipe
        close(pipefd[1]);

        // Выполняем pr1 с аргументами arg1 и arg2
        execlp("pr1", "pr1", "arg1", "arg2", (char *)NULL);
        perror("execlp pr1");
        exit(EXIT_FAILURE);
    }

    // Запускаем pr2, который получает данные из pr1
    pid2 = fork();
    if (pid2 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        // В дочернем процессе перенаправляем stdin на чтение из pipe
        close(pipefd[1]); // Закрываем ненужный конец для записи
        dup2(pipefd[0], STDIN_FILENO); // Перенаправляем stdin из pipe
        close(pipefd[0]);

        // Выполняем pr2
        execlp("pr2", "pr2", (char *)NULL);
        perror("execlp pr2");
        exit(EXIT_FAILURE);
    }

    // Закрываем pipe в родительском процессе
    close(pipefd[0]);
    close(pipefd[1]);

    // Ожидаем завершения pr1 и pr2
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

    // Запускаем pr3, перенаправляя его вывод в файл f1.dat
    pid3 = fork();
    if (pid3 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid3 == 0) {
        // Открываем файл f1.dat для дозаписи
        file_fd = open("f1.dat", O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (file_fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Перенаправляем stdout в файл
        dup2(file_fd, STDOUT_FILENO);
        close(file_fd);

        // Выполняем pr3
        execlp("pr3", "pr3", (char *)NULL);
        perror("execlp pr3");
        exit(EXIT_FAILURE);
    }

    // Ожидаем завершения pr3
    waitpid(pid3, &status, 0);

    return 0;
}
