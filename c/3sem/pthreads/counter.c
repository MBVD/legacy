#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 10
#define VALUE 10000000


int counter = 0;
pthread_mutex_t mutex;

void increment_counter(){
    while (1)
    {
        pthread_mutex_lock(&mutex);
        if(counter >= VALUE){
            pthread_mutex_unlock(&mutex);
            break;
        }

        counter++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
    
}

int main(){
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < NUM_THREADS; i++){
        if(pthread_create(&threads[i], NULL, increment_counter, NULL) != 0){
            perror("error creating");
            exit(EXIT_FAILURE);
            
        }
    }

    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("final counter %d\n", counter);
    return 0;
}