/*
Data una matrice NxN con N pari di interi generati casualmente, allocata dinamicamente con N argomento 
da riga di comando, creare N thread che prelevano casualmente un elemento dalla riga di compentenza 
e lo inseriscano in un vettore di N elementi. Un thread n+1-esimo attende il riempimento del vettore
per stampare il contenuto e il numero di elementi. Usare mutex e var cond
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

struct Esercizio6ConMuxCond
{
    int*array;
    int* risultato;
    int riga;

}typedef informazioni;

int N;
int** matrice ;
pthread_mutex_t mux;
pthread_cond_t condizione;

void* Routine(void*arg){
    informazioni* info = (informazioni*)arg;
    int random = rand()%N;
    int count = 0;

    pthread_mutex_lock(&mux);
    info->risultato[info->riga] = info->array[random];
    pthread_mutex_unlock(&mux);

    pthread_cond_signal(&condizione);

}

void* Stampa(void*arg){
    informazioni* info = (informazioni*)arg;
    
    pthread_mutex_lock(&mux);
    printf("\n[ ");
    for(int i = 0 ; i<N ; i++){
        while (info->risultato[i]==0)
        {
            pthread_cond_wait(&condizione,&mux);
        }
        
        printf(" %d, ",info->risultato[i]);
     }
      printf(" ]\n ");
    pthread_mutex_unlock(&mux);

}

void main(){
    pthread_t th[N+1];
    informazioni* info;
    int* vettore;
    int random ;

    srand(time(NULL));
    pthread_mutex_init(&mux,NULL);
    pthread_cond_init(&condizione,NULL);

    printf("Inserisci la dimensione N : ");
    scanf("%d",&N);
    printf("\n");

    //Allocazione dinamica matrice 
    matrice = (int**)malloc(N*sizeof(int*));
    for(int i = 0; i<N ;i++){
        matrice[i] = (int*)malloc(N*sizeof(int));
    }

    //allocazione dinamica della stuct 
    info = (informazioni*)malloc((N+1)*sizeof(informazioni));
    vettore = (int*)malloc(N*sizeof(int));
    for(int i = 0; i<N ;i++){
        vettore[i] =0;
    }


    //Riempimento matrice 
    for(int i = 0 ; i<N ; i++){
        for(int j=0 ; j<N ; j++){
            random = rand()%100+1;
            matrice[i][j] = random;
        }
        
    }
    //printf("STAMPA MATRICE\n");

    for(int i = 0 ; i<N ; i++){
        printf(" [ ");
        for(int j=0 ; j<N ; j++){
            printf("%d ," ,matrice[i][j]);
        }
        printf(" ]\n");
    }

    for(int i = 0 ; i<N+1 ; i++){
        if(i==N){
            info[i].risultato= vettore;
            pthread_create(&th[i],NULL,Stampa,(void*)&info[i]);
        }
        else{
            info[i].riga = i;
            info[i].array = matrice[i];
            info[i].risultato = vettore;
            pthread_create(&th[i],NULL,Routine,(void*)&info[i]);
        }
    }

    for(int i = 0 ; i<N+1 ; i++){
        pthread_join(th[i],NULL);
    }

   

    pthread_mutex_destroy(&mux);
    pthread_cond_destroy(&condizione);

}
