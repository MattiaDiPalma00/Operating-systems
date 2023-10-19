/*
Si scriva un programma Ce posix che prende dariga di comando N interi tra 5 e 20.
Il valore di N è costante ed indicato nel sorgente. Il programma avvia N thread che 
hanno il seguente comportamente :
1)Ogni thread va in sospensione con la sleep per un un numero di secondi corrispondente e stampa indice 
2) alla fine della sleep ogni thread stampa "thread svegliato"
3) i thread  vengono sincronizatti utilizzando semafori basati su memoria
4) dopo aver atteso il termine di tutti gli altri thread ciascun thread scrive sullo schermo 
che ha terminato
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<time.h>
#include<unistd.h>
#define N 4 

int array[4];
sem_t semaforo ;
sem_t semaforofine;

void* routine(void* arg){
    int indice = *(int*) arg;
    int tempo ;
  
    sem_wait(&semaforo);
    tempo = array[indice];
    sleep(tempo);
    printf("THREAD SVEGLIATO TID: %ld E INDICE %d \n",pthread_self(),indice);
    sem_post(&semaforo);

  
    free(arg);

}

void main(){

    printf("Inserire quattro numeri compresi tra 5 e 20.");
    int numero ;
    pthread_t th[N];
    sem_init(&semaforo,0,1);
    sem_init(&semaforofine,0,N);

    for(int i = 0 ; i <N ; i++){
        printf("\narray[%d] = ",i);
        scanf("%d",&numero);
        array[i]= numero;
    }

    for(int i = 0 ; i < N ; i++){
        int *indice = malloc(sizeof(int));
        *indice = i;
        pthread_create(&th[i],NULL,routine,indice);
    }

    for(int i = 0 ; i < N ; i++){
       
        pthread_join(th[i],NULL);
        printf("THREAD TERMINATO TID: %ld \n",th[i]);
    }


    sem_destroy(&semaforo);
    sem_destroy(&semaforofine);



}