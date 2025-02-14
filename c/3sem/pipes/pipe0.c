#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

static void charatatime(char *); 
int 
main(void) 
{
    pid_t   pid;
    if ((pid = fork()) < 0) { 
        perror("ошибка вызова функции fork"); 
    } else if (pid == 0) { 
        charatatime("от дочернего процесса\n"); 
    } else {
        charatatime("от родительского процесса\n"); 
    } 
    exit(0); 
} 
static void 
charatatime(char *str) 
{
    char    *ptr; 
    int     c;
    setbuf(stdout, NULL); /* установить небуферизованный режим */ 
    for (ptr = str; (c = *ptr++) != 0; ) 
        putc(c, stdout); 
} 