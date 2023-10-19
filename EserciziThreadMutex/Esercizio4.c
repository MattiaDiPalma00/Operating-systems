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
Utilizzare mutex e variabili di condizione
*/


#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<semaphore.h>

#define N 5


int** matrice;
int* somme;
int x = 0 ;
pthread_mutex_t mutex;
pthread_cond_t cond;


void* CalcoloSomma(void* arg){
    int riga = *(int*) arg;
    
    for (int j = 0 ; j <N ;j++){
        if((riga%2)==0){
            if ((j%2)==0){
                pthread_mutex_lock(&mutex);
                somme[riga]+= matrice[riga][j];
                pthread_mutex_unlock(&mutex);

            }
        }
        else{
              if ((j%2)!=0){
                pthread_mutex_lock(&mutex);
                somme[riga]+= matrice[riga][j];
                pthread_mutex_unlock(&mutex);

            }
        }
        
    }

    pthread_cond_signal(&cond);

}

void* CalcoloMin(){

    int minimo ;

    pthread_mutex_lock(&mutex);
    while (somme[0] == 0)
    {
         pthread_cond_wait(&cond,&mutex);
    }
    pthread_mutex_unlock(&mutex);

    minimo = somme[0];
    for (int i = 0 ; i < N ; i++){
        pthread_mutex_lock(&mutex);
            while (somme[i] == 0)
            {
                 pthread_cond_wait(&cond,&mutex);
            }
            if (somme[i] < minimo )
                minimo = somme[i];

    pthread_mutex_unlock(&mutex);
        
    }
    printf("\nIl minimo è : %d\n ", minimo) ; 

}



void main(){

    pthread_t th[N+1];
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
   

    //ALLOCAZIONE MATRICE NxN
    matrice = malloc(N*sizeof(int*));
    for (int i = 0; i <N ; i++){
         matrice[i] = malloc(N*sizeof(int));
    }

    somme = malloc(N*sizeof(int));

    //ARRAY DI SOMME PARZIALI INIZIALIZZATO A 0 
    for (int i = 0; i <N ; i++){
        somme[i] = 0 ;
    }

    srand(time(NULL));
    int random;

    //RIEMPIMENTO MATRICE 
      for (int i = 0; i <N ; i++){
        for (int j = 0; j <N ; j++){
            random = rand()%255;
            matrice[i][j] = random;
        }
    }
   

    printf("\nSTAMPA DELLA MATRICE\n");

    //STAMPA MATRICE 
      for (int i = 0; i <N ; i++){
        printf("[ ");
        for (int j = 0; j <N ; j++){
            printf("%d, ",matrice[i][j]);
        }
        printf(" ]\n");
    }
    
    
   
    int i ;
    int riga;
    for (i = 0 ; i < N; i++){
        int *riga = malloc(sizeof(int));
        *riga = i ;
        pthread_create(&th[i],NULL,&CalcoloSomma,riga);
    }

     pthread_create(&th[N],NULL,&CalcoloMin,NULL);
    

      for (int i = 0 ; i < N+1; i++){
            pthread_join(th[i],NULL);
    }

    printf("\nSOMME PARZIALI\n ");
    printf("[ ");
      for (int i = 0; i <N ; i++){
        printf(" %d, ",somme[i]);
    }
     printf(" ]\n");
 

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

}

