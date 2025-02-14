#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>


int main(int smth, char * arr[]){
    char * buf;
    pid_t pid;
    int fd;
    //fd = open(arr[smth - 1], O_WRONLY | O_CREAT, 0644);

    if(fd < 0){
        perror("dfsfd");
        exit(1);
    } else {
        printf("file sozdan %d\n",fd);
        fflush(NULL);
    }
    // for(i; i < smth; i++){
    //     if(arr[i][0] == '>'){
    //         continue;
    //     }
    // }
    
    pid = fork();
    
    if(pid == -1){
        perror("Pid error");
        exit(1);
    }
    else{
        puts("pid razdvoen ");
        if(pid == 0){ //child
        //close(fd);
        //dup2(fd , STDOUT_FILENO);
        //close(fd);
        printf("file sozdan %d\n",fd);
        puts("ya rodilsya child ");

        //execlp(arr[1], arr[1], NULL);
        //perror("Bad copy");
        exit(1);   
    }else{ //parent
        puts("ya umer parent ");
        
        waitpid(pid, NULL, 0);
    }


        
    }
    

    exit(0);
}
