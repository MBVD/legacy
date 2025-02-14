#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


void* init(void* pid){
    puts("01");
    puts("yes");
    printf("thread arg (in thread): %d, %ld \n", (int*)pid, pthread_self());
    pthread_exit(NULL);
}


int main(){
    pthread_t thread1;
    pthread_t thread2[5];
    int arg1 = 1;
    int arg2 = 2;
    pthread_attr_t attr_thread1;
    pthread_attr_t attr_thread2;


    pthread_attr_init(&attr_thread1);

    pthread_attr_setdetachstate(&attr_thread1, PTHREAD_CREATE_DETACHED);    

    if(pthread_create(&thread1, &attr_thread1, init, ((void*)arg1)) != 0){
        perror("error");
        exit(EXIT_FAILURE);
    }

    pthread_attr_init(&attr_thread2);
    size_t stack_s = 2097152;
    pthread_attr_setstacksize(&attr_thread2, stack_s);

    for(int i = 0; i < 5; i++){
        if(pthread_create(&thread2, &attr_thread2, init, ((void*)arg2)) != 0){
            perror("error");
            exit(EXIT_FAILURE);
        }
    }


    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_attr_destroy(&attr_thread1);
    pthread_attr_destroy(&attr_thread2);

   
}