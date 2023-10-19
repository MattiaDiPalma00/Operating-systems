/*
Si realizzi un programma in C e posix sotto linux che lancia M thread per calcolare il prodotto 
righe-colonne di due matrici MxN e NxP. L'i-esimo thread, dopo aver calcolato l'i-esima riga della matrice
risultato ne calcola la somma ed inserisce il risultato in un array m-dimensionale, in modo concorrente, 
nella prima locazione libera disponibile.
Non appena l'array sarà riempito, un m+1 -esimo thread che era in attesa ne stamperà il contenuto.
Le matrici devono essere allocate dinamicamente. Usare mutex e variabili di condizione
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#define M 2
#define N 2
#define P 3

struct informazioni
{
    int** matriceA;
    int** matriceB;
    int** risultato;
    int* somme;
}typedef informazioni;

int** matriceA;
int** matriceB;
int** risultato;
int* somme;
pthread_mutex_t mutex;
pthread_mutex_t mutexstampa;
pthread_cond_t cond;
pthread_cond_t condstampa;

void* CalcoloProdotto(void* arg){
    int riga = *(int*)arg;
    int somma;

    for(int i = 0 ; i < P ; i++){
        somma = 0 ;
        for(int j = 0 ; j < N; j++){
            pthread_mutex_lock(&mutex);
            somma += matriceA[riga][j]*matriceB[j][i];
            pthread_mutex_unlock(&mutex);
        }
        pthread_mutex_lock(&mutex);
        risultato[riga][i] = somma;
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    
    pthread_cond_signal(&condstampa);
    
 
}
void* FaiSomma(void* arg){
    int riga = *(int*)arg;
    int somma = 0 ;

    pthread_mutex_lock(&mutex);
    for(int i = 0 ; i < P ; i++ ){
        while(risultato[riga][i]==0){
            pthread_cond_wait(&cond,&mutex);
        }
        somma += risultato[riga][i];
    }
    somme[riga] = somma;
    pthread_mutex_unlock(&mutex);
       
}

void* Stampa(void* arg){

    int riga = *(int*)arg;
    printf("\n array delle somme -> [ ");
    pthread_mutex_lock(&mutexstampa);
    for(int i = 0 ; i < M ;i++){
        while(somme[i]==0){
            pthread_cond_wait(&condstampa,&mutexstampa);
        }
        printf("%d, ",somme[i]);
    }
    printf("]\n");
    pthread_mutex_unlock(&mutexstampa);

}

void main(){
    pthread_t th[M+1];
    int random;
    int* riga;

    pthread_mutex_init(&mutex,NULL);
    pthread_mutex_init(&mutexstampa,NULL);
    pthread_cond_init(&cond,NULL);
    pthread_cond_init(&condstampa,NULL);

    //Allocazione dinamica matricaA
    matriceA = (int**)malloc(M*sizeof(int*));
    for (int i = 0 ; i < M ; i++){
        matriceA[i] = (int*)malloc(N*sizeof(int));
    }

    //Allocazione dinamica matriceB
    matriceB = (int**)malloc(N*sizeof(int*));
    for (int i = 0 ; i < N ; i++){
        matriceB[i] = (int*)malloc(P*sizeof(int));
    }

    //Allocazione dinamica matrice risultato
    risultato = (int**)malloc(M*sizeof(int*));
    for (int i = 0 ; i < M ; i++){
        risultato[i] = (int*)malloc(P*sizeof(int));
    }

    //Allocazione array somme
    somme = (int*)malloc(M*sizeof(int));

    srand(time(NULL));
    //riempimento matrici
    for(int i = 0 ; i<M ; i++){
        for(int j = 0 ; j <N ;j++){
            random = rand()%3+1;
            matriceA[i][j] = random;
        }

    }
    for(int i = 0 ; i<N ; i++){
        for(int j = 0 ; j <P ;j++){
            random = rand()%5+1;
            matriceB[i][j] = random;
        }

    }
     for(int i = 0 ; i<M ; i++){
        for(int j = 0 ; j <P ;j++){
           risultato[i][j] = 0;
        }

    }
    for(int i = 0 ; i<M ; i++){
            somme[i] = 0;
    }

    //Stampa matriceA
     for(int i = 0 ; i<M ; i++){
        printf(" [ ");
        for(int j = 0 ; j <N ;j++){
            printf("%d, ",matriceA[i][j]);
        }
         printf("]\n");

    }

    printf("\n");
     //Stampa matriceB
     for(int i = 0 ; i<N ; i++){
        printf(" [ ");
        for(int j = 0 ; j <P ;j++){
            printf("%d, ",matriceB[i][j]);
        }
         printf("]\n");

    }

    for(int i = 0 ; i<M+1; i++){
        if(i == M){
            riga = malloc(sizeof(int));
            *riga = i;
            pthread_create(&th[i],NULL,Stampa,(void*)riga);
        }
        else{
            riga = malloc(sizeof(int));
            *riga = i;
            pthread_create(&th[i],NULL,CalcoloProdotto,(void*)riga);
            pthread_create(&th[i],NULL,FaiSomma,(void*)riga);

        }
    }

    for(int i = 0 ; i<M+1; i++){
        pthread_join(th[i],NULL);
    }

     printf("\nstampa matrice risultato\n");
     //Stampa matricerisultato
     for(int i = 0 ; i<M ; i++){
        printf(" [ ");
        for(int j = 0 ; j <P ;j++){
            printf("%d, ",risultato[i][j]);
        }
         printf("]\n");

    }
    
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutexstampa);
    pthread_cond_destroy(&cond);
    pthread_cond_destroy(&condstampa);


}