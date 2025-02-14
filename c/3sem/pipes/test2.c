#include <stdio.h>
#include <signal.h>
#include <string.h>

void signal_handler(int signal) {
    printf("Received signal %d\n", signal);
}

int main() {
    struct sigaction sa;
    
    // Очищаем структуру
    memset(&sa, 0, sizeof(sa));

    // Указываем обработчик сигнала
    sa.sa_handler = signal_handler;

    // Устанавливаем действие на сигнал SIGINT (например, Ctrl+C)
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    // Ждем сигнала
    while (1) {
        pause(); // Ждем сигнала
    }

    return 0;
}
