#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(){
    pid_t ls_pid = -1, grep_pid = -1, wc_pid = -1;
    int f_ls_grep[2], f_grep_wc[2];
    if(pipe(f_ls_grep) == -1){
        perror("f_ls_grep");
        return 1;
    }

    ls_pid = fork();

    if(ls_pid == -1){
        perror("ls_pid");
        return 2;
    }

    if(ls_pid == 0){
        ls_pid = getpid();
        dup2(f_ls_grep[1], 1);
        close(f_ls_grep[0]);
        close(f_ls_grep[1]);
        
        //вызывваем правильного чужого
        execlp("ls", "ls", NULL);
        perror("ls");
        return 3;
    }
    close(f_ls_grep[1]);

    if(pipe(f_grep_wc) == -1){
        perror("f_grep_wc");
        return 1;
    }

    grep_pid = fork();

    if(grep_pid == -1){
        perror("grep_pid");
        return 2;
    }

    if(grep_pid == 0){
        grep_pid = getpid();
        dup2(f_ls_grep[0], 0);
        dup2(f_grep_wc[1], 1);
        close(f_ls_grep[0]);
        close(f_grep_wc[1]);
        close(f_grep_wc[0]);
        
        execlp("grep","grep", ".c", NULL);
        perror("grep");
        return 3;
    }
    close(f_grep_wc[1]); //закрыли на запись
    close(f_ls_grep[0]); //закрыли на чтение 
    //1. pipe полностью закрыт

    wc_pid = fork();

    if(wc_pid == -1){
        perror("wc_pid");
        return 2;
    }

    if(wc_pid == 0){
        wc_pid = getpid();
        dup2(f_grep_wc[0], 0);
        close(f_grep_wc[0]);

        
        execlp("wc", "wc", "-l", NULL);
        perror("wc");
        return 3;
    }
    close(f_grep_wc[0]);

    int status, ret_val = 0;

    waitpid(ls_pid, &status, WUNTRACED);
    if( !(WEXITSTATUS(status) == 0 && WIFEXITED(status)) ){
        ret_val |= 1;
    }

    waitpid(grep_pid, &status, WUNTRACED);
    if( !(WEXITSTATUS(status) == 0 && WIFEXITED(status)) ){
        ret_val |= 2;
    }

    waitpid(wc_pid, &status, WUNTRACED);
    if( !(WEXITSTATUS(status) == 0 && WIFEXITED(status)) ){
        ret_val |= 4;
    }

    return ret_val;
}