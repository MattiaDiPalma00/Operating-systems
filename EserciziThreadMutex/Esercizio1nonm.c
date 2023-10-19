#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

#define NUM_THREADS 3
#define NUM_RANDOM_NUMBERS 1000
#define LIMIT 10

int count = 0;
int sum = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *thread_function(void *arg) {
    srand(time(NULL));
    int i, num;
    for (i = 0; i < NUM_RANDOM_NUMBERS; i++) {
        num = rand() % 100 + 1;
        pthread_mutex_lock(&mutex);
        sum += num;
        count++;
        if (count == LIMIT) {
            pthread_cond_broadcast(&cond);
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

void *notify_function(void *arg) {
    pthread_mutex_lock(&mutex);
    while (count < LIMIT) {
        printf("Aspettiamo che il contatore superi il limite...\n");
        pthread_cond_wait(&cond, &mutex);
    }
    printf("Limite raggiunto. Possiamo uscire...\n");
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int i;
    
    pthread_create(&threads[0], NULL, thread_function, NULL);
    pthread_create(&threads[1], NULL, thread_function, NULL);
    pthread_create(&threads[2], NULL, notify_function, NULL);

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("La somma dei numeri casuali è: %d\n", sum);
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    
    return 0;
}