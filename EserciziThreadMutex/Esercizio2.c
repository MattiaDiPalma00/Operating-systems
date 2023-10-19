/*
Si realizzi un programma C e Posix in ambiente Linux 
che, impiegando la libreria Pthread, generi tre 
thread
• I primi due thread sommano 1000 numeri generati 
casualmente ed ogni volta incrementano un contatore
• Il terzo thread attende che il contatore incrementato 
dai due thread raggiunga un valore limite fornito da 
riga di comando, notifica l’avvenuta condizione e 
termina
• Utilizzare le variabili condizione
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

#define NUMTH 3
#define limite 10


int count = 0 ;
int somma = 0 ;
pthread_mutex_t mux;
pthread_cond_t cond;

void* conta(void *arg){

    srand(time(NULL));

    for(int i = 0 ; i< 1000; i++){
          pthread_mutex_lock(&mux);
        somma+= rand()%100 +1;
        count++;
         if (count == limite) {
            pthread_cond_signal(&cond);
         }
         pthread_mutex_unlock(&mux);
    }
      pthread_exit(NULL);
   
    

}

void* notifica(void *arg){

    pthread_mutex_lock(&mux);
    while(count < limite){
        printf("aspettiamo che supera il limite...\n");
      
        pthread_cond_wait(&cond,&mux);
    }
    printf("limite raggiunto possiamo uscire...\n");
    pthread_mutex_unlock(&mux);
    pthread_exit(NULL);
}


void main(){
    
    pthread_t th[NUMTH];
    pthread_mutex_init(&mux,NULL);
    pthread_cond_init(&cond,NULL);
    

   if( pthread_create(&th[0],NULL,conta,NULL)!= 0)
        perror("errore creazioene");

    pthread_create(&th[1],NULL,conta,NULL);
    pthread_create(&th[2],NULL,notifica,NULL);
   
    for(int i = 0; i<NUMTH, i++;){

        pthread_join(th[i],NULL);
    }

     //printf("La somma dei numeri casuali è: %d\n", somma);

    pthread_mutex_destroy(&mux);
    pthread_cond_destroy(&cond);
}