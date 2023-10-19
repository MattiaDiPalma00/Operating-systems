
/*
PRIMI ESEMPI SU RACE CONDITION E MUTEX PER SINCRONIZZARE I THREAD 
*/


#include<stdio.h>
#include<pthread.h>

int global = 0 ; // variabile globale che serve per l'esempio 
 pthread_mutex_t mux; // mutex globale 

void* aggiungi();

void main(){
    pthread_t th1 ;
    
    pthread_mutex_init(&mux,NULL);


    for (int i = 0 ; i <2; i++){
        pthread_create(&th1,NULL,aggiungi,NULL);
    }

    for (int i = 0 ; i <2; i++){
        pthread_join(th1,NULL);
    }

    printf("la var globale : %d",global);
    pthread_mutex_destroy(&mux);


}

void* aggiungi(){

    for (int i = 0 ; i < 1000 ; i++){
        pthread_mutex_lock(&mux);
        global++;
        pthread_mutex_unlock(&mux);
    }
}
