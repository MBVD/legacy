    #include <stdio.h>
    #include <pthread.h>
    #include <stdlib.h>
    #include <string.h>

    #define NUM_THREADS 3

    typedef struct {
        int x, y, z;
    } Vec3D;

    typedef struct {
        size_t start;
        size_t finish;
        pthread_t thread;
    } Thread_arg;

    Vec3D arr[100];

    Vec3D sum = {0, 0, 0};

    pthread_mutex_t mut;

    void* summator(void* args);

    Thread_arg threads_args[NUM_THREADS];

    int main() {
        for (int i = 0; i<100; i++) {
            arr[i].x = i;
            arr[i].y = 100 - i;
            arr[i].z = 1; 
        }

        pthread_mutex_init(&mut, NULL);
        
        for (int i = 0; i < NUM_THREADS - 1; i++){
            threads_args[i].start = i * (100/NUM_THREADS);
            threads_args[i].finish = (i + 1)*(100/NUM_THREADS);
            if (pthread_create(&threads_args[i].thread, NULL, summator, &threads_args[i]) == -1){
                fprintf(stderr, "Cant create thread %d: %s\n", i, streerror(errno));
            }
        }

        threads_args[NUM_THREADS-1].start =  (NUM_THREADS - 1) * (100/NUM_THREADS);
        threads_args[NUM_THREADS-1].finish = 100;

        if (pthread_create(&threads_args[NUM_THREADS-1].thread, NULL, summator, &threads_args[NUM_THREADS - 1]) == -1){
            fprintf(stderr, "Cant create thread %d: %s\n", NUM_THREADS - 1, streerror(errno));
        }


        for (int i = 0; i<NUM_THREADS; ++i){
            pthread_join(threads_args[i].thread, NULL);
        }


        pthread_mutex_destroy(&mut);

        printf("x - %d y - %d z - %d \n", sum.x, sum.y, sum.z);

        return 0;
    }

    void * summator(void * argp){
        Thread_arg * arg = (Thread_arg *) argp;
        
        Vec3D local_sum = {0, 0, 0};
        
        for (size_t i = arg->start; i < arg->finish; i++){
            local_sum.x += arr[i].x;
            local_sum.y += arr[i].y;
            local_sum.z += arr[i].z;
        }

        pthread_mutex_lock(&mut); // начало критической секции
        sum.x += local_sum.x;
        sum.y += local_sum.y;
        sum.z += local_sum.z;
        pthread_mutex_unlock(&mut); // конец критической секции
        
        return NULL;
    }