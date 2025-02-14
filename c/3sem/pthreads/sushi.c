#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define CONVEER_SIZE 12
#define SUSHIST 3
#define USERS 4
#define COUNT 50

typedef struct {
    int sushi_conv[CONVEER_SIZE];
    int count;
    int head;
    int tail;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} sushiconv;

sushiconv conv;
int chef_count[SUSHIST] = {0};
int users_count[USERS] = {0};
int total_sushi_made = 0;
int total_sushi_eaten = 0;

void* chef(void* arg) {
    int chef_id = *((int*)arg);
    while (1) {
        usleep(50000);
        pthread_mutex_lock(&conv.mutex);

        if (total_sushi_made >= COUNT) {
            pthread_mutex_unlock(&conv.mutex);
            break;
        }

        while (conv.count == CONVEER_SIZE) {
            pthread_cond_wait(&conv.not_full, &conv.mutex);
        }

        conv.sushi_conv[conv.tail] = 1;
        conv.tail = (conv.tail + 1)% CONVEER_SIZE;
        conv.count++;
        chef_count[chef_id]++;
        total_sushi_made++;

        printf("chef %d added sushi: %d\n", chef_id, total_sushi_made);
        pthread_cond_signal(&conv.not_empty);

        pthread_mutex_unlock(&conv.mutex);
    }
    return NULL;
}
void* user(void* arg) {
    int user_id = *((int*)arg);
    while (1) {
        usleep(70000);
        pthread_mutex_lock(&conv.mutex);

        if (total_sushi_eaten >= COUNT) {
            pthread_mutex_unlock(&conv.mutex);
            break;
        }

        while (conv.count == 0) {
            pthread_cond_wait(&conv.not_empty, &conv.mutex);
        }

        conv.sushi_conv[conv.head] = 0;
        conv.head = (conv.head + 1)% CONVEER_SIZE;
        conv.count--;
        users_count[user_id]++;
        total_sushi_eaten++;

        printf("user %d ate sushi: %d\n", user_id, total_sushi_eaten);
        pthread_cond_signal(&conv.not_full);
        pthread_mutex_unlock(&conv.mutex);
    }
    return NULL;
}

int main() {
    pthread_t chefs[SUSHIST];
    pthread_t users[USERS];
    int chef_ids[SUSHIST];
    int user_ids[USERS];

    conv.count = 0;
    conv.tail = 0;
    conv.head = 0;

    pthread_mutex_init(&conv.mutex, NULL);
    pthread_cond_init(&conv.not_full, NULL);
    pthread_cond_init(&conv.not_empty, NULL);
    for (int i = 0; i < SUSHIST; i++) {
        chef_ids[i] = i;
        if (pthread_create(&chefs[i], NULL, chef, &chef_ids[i]) != 0) {
            perror("pthread chef error");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < USERS; i++) {
        user_ids[i] = i;
        if (pthread_create(&users[i], NULL, user, &user_ids[i]) != 0) {
            perror("pthread user error");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < SUSHIST; i++) {
        pthread_join(chefs[i], NULL);
    }
    for (int i = 0; i < USERS; i++) {
        pthread_join(users[i], NULL);
    }
    printf("ress:\n");
    for (int i = 0; i < SUSHIST; i++) {
        printf("chef %d made %d sushi\n", i, chef_count[i]);
    }
    for (int i = 0; i < USERS; i++) {
        printf("user %d ate %d sushi\n", i, users_count[i]);
    }
    pthread_mutex_destroy(&conv.mutex);
    pthread_cond_destroy(&conv.not_full);
    pthread_cond_destroy(&conv.not_empty);

    return 0;
}
