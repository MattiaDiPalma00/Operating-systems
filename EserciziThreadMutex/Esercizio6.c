/*
Data una matrice NxN con N pari di interi generati casualmente, allocata dinamicamente con N argomento 
da riga di comando, creare N thread che prelevano casualmente un elemento dalla riga di compentenza 
e lo inseriscano in un vettore di N elementi. Un thread n+1-esimo attende il riempimento del vettore
per stampare il contenuto e il numero di elementi. Usare semafori posix basati su nome
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<semaphore.h>
#include<pthread.h>
#include<fcntl.h>  

#define nome "/semaforo"
#define nome2 "/semaforostampa"

struct informazioni {
    int* riga;
    int index;
    int* count;
}typedef informazioni;

sem_t* semaforo;
sem_t* semaforostampa;
int** matrice;
int* array;
int N;
informazioni info; 


void* Routine(void* arg){

    informazioni* info = (informazioni*)arg;
    int random = rand()%N;

    sem_wait(semaforo);
    array[info->index] = info->riga[random];
    sem_post(semaforo);
    
    if(info->index == N)
        sem_post(semaforostampa);
}

void* StampaArray(void* arg){
    informazioni* info = (informazioni*) arg;
    sem_wait(semaforostampa);

    printf("vettore risultato");
    printf("\n[ ");
    for(int i = 0 ; i < N ;i ++){
        *(info->count)++ ;
        printf("%d, ",array[i]);
    }
      printf(" ] e il numero di elementi è : %d ",*(info->count));
    


}


void main(){
    //int N;
    int*riga;
    int random;
    pthread_t th[N+1];
    informazioni* info;

    sem_unlink(nome);
    sem_unlink(nome2);
    semaforo = sem_open(nome, O_CREAT | O_EXCL,0644,1);
    semaforostampa = sem_open(nome2, O_CREAT | O_EXCL,0644,1);

    srand(time(NULL));
    printf("Inserire la dimensione della matrice.\n");
    scanf("%d",&N);
    array =(int*) malloc(N*sizeof(int));
    info = (informazioni*)malloc(N*sizeof(informazioni));

    //ALLOCAZIONE MATRICE 
    matrice = (int**)malloc(N*sizeof(int*));
    for(int i = 0 ; i < N ;i++){
        matrice[i] = (int*)malloc(N*sizeof(int));
    }
   

    //RIEMPIMENTO CASUALE MATRICE
    for(int i = 0 ; i< N ;i++){
        for(int j = 0 ; j<N ;j++){
            random = rand()%100 + 1;
            matrice[i][j] = random;
        }
    }

    printf("\nStampa della matrice.\n");
    //STAMPA MATRICE 
     for(int i = 0 ; i< N ;i++){
        printf(" [ ");
        for(int j = 0 ; j<N ;j++){
            printf("%d, ",matrice[i][j]);
        }
        printf("]\n");

    }

    for(int i = 0 ; i < N+1 ;i++){
        if(i == N){
            int count = 0 ;
            info[i].count = &count;
            pthread_create(&th[i],NULL,StampaArray,&info[i]);
        }
        else {
            info[i].index = i;
            info[i].riga = matrice[i];
            pthread_create(&th[i],NULL,Routine,(void*)&info[i]);
        }
    }

    for(int i = 0 ; i < N+1 ;i++){
       pthread_join(th[i],NULL);
    }
    
    
    sem_close(semaforo);
    sem_unlink(nome);
    sem_close(semaforostampa);
    sem_unlink(nome2);

}