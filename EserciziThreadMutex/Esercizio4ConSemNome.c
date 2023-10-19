/*
Si realizzi un programma in C e Posix sotto Linux che, 
utilizzando la libreria Pthread.
Lancia N thread per calcolare la somma degli elementi di ciascuna riga di una matrice NxN
di interi generati casualmente in un intervallo [0,255] alloca dinamicamente,
il calcolo della somma degli elementi di ciascuna riga deve essere effettuato concorrentemente su
tutte le righe secondo queste regole:
il thread i-esimo pari , calcola la somma degli elementi di indice pari della riga i-esima 
il thread i-esimo dispari , calcola la somma degli elementi di indice dispari della riga i-esima
le somme parziali verrano inserite in un array ,e un N+1-esimo thread
provvederà a ricercare il minimo e stamparlo a video. 
Utilizzare semafori posix basati su nome 
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>

#define nomesem "/semaforo1"
#define nomesem2 "/semaforo2"

struct informazioni
{
    int* vettore;
    int indice;
}typedef informazioni;

int** matrice ;
int* sommeP;
int N;
sem_t* semaforo;
sem_t* semaforoStampa;

void* CalcoloSomma(void* arg){
    informazioni* info = (informazioni*) arg;
    int somma = 0;

   
    if(info->indice%2 == 0 ){
        for(int i = 0 ; i<N; i++){
            if(i%2 == 0 ){
                sem_wait(semaforo);
                somma+= info->vettore[i];
                sommeP[info->indice]=somma;
                sem_post(semaforo);
            }
        }
    }
    else{
        for(int i = 0 ; i<N; i++){
            if(i%2 != 0 ){
                sem_wait(semaforo);
                somma+= info->vettore[i];
                sommeP[info->indice]=somma;
                sem_post(semaforo);
                
            }
        }
    }
    sem_post(semaforoStampa);
    
}

void* Stampa(){

    printf("Stampa risultato\n");
    printf(" [ ");
    for(int i = 0 ; i<N; i++){
        sem_wait(semaforoStampa);
        printf("%d, ",sommeP[i]);
    }
    printf(" ]\n ");

}

void main(){
    pthread_t th[N+1];
    informazioni* info;
    int random;

    sem_unlink(nomesem);
    sem_unlink(nomesem2);
    semaforo = sem_open(nomesem,O_CREAT|O_EXCL,0644,1);
    semaforoStampa = sem_open(nomesem2,O_CREAT|O_EXCL,0644,0);

    printf("Inserisci dimensione : \n");
    scanf("%d",&N);

    //allocazione dinamica matrice 
    matrice = (int**)malloc(N*sizeof(int*));
    for(int i = 0 ; i<N; i++){
        matrice[i] = (int*)malloc(N*sizeof(int));
    }

    //allocazione dinamica struttura 
    info = (informazioni*)malloc((N+1)*sizeof(informazioni));

    //allocazione somme parziali
    sommeP = (int*)malloc(N*sizeof(int));
    for(int i = 0 ;i<N ;i++){
        sommeP[i]= 0;
    }

    srand(time(NULL));
    for(int i = 0 ; i<N; i++){
        for(int j = 0; j<N ;j++){
            random = rand()%255;
            matrice[i][j] = random;
        }
    }

    printf("Stampa matrice\n");
      for(int i = 0 ; i<N; i++){
        printf(" [ ");
        for(int j = 0; j<N ;j++){
             printf("%d, ",matrice[i][j]);
        }
         printf("]\n");
    }

    for(int i = 0 ;i <N+1; i++){
        if(i==N){
            pthread_create(&th[i],NULL,Stampa,NULL);
        }
        else{
            info[i].indice=i;
            info[i].vettore = matrice[i];
            pthread_create(&th[i],NULL,CalcoloSomma,(void*)&info[i]);
        }
    }

      for(int i = 0 ;i <N+1; i++){
        pthread_join(th[i],NULL);
      }




    sem_unlink(nomesem);
    sem_close(semaforo);
    sem_unlink(nomesem2);
    sem_close(semaforoStampa);
}
