/*
Si realizzi un programma in C e Posix sotto Linux che, 
utilizzando la libreria Pthread.
Lanciamo M thread per calcolre il prodotto di due matrici di dimensione MxN e NxP
dopo effettuato il prodotto, un M+1 esimo thread che era in attesa ne stamperà il risultato 
usare semafori posix basati su nome
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<time.h>
#include <fcntl.h>  

#define righeA 2
#define colonneA 3
#define righeB 3
#define colonneB 2
#define nomesem "/semaforo"
#define nomesem2 "/mux"

int** matriceA;
int** matriceB;
int** matriceC;
sem_t* semaforo;
sem_t* mux;

void* CalcoloProdotto(void* x){
    int riga = *(int*) x ;
    int k, sommP;

    for(int i = 0 ; i < colonneB; i++){
        sommP = 0 ;
        for (k = 0 ; k< righeB; k++){
            
            sommP+= matriceA[riga][k]*matriceB[k][i];
           
        }
        sem_wait(mux);
        matriceC[riga][i] = sommP;
        sem_post(mux);

    }
    sem_post(semaforo);
    
        
}


void* StampaRisultato(){

 sem_wait(semaforo);
    for (int i = 0; i < righeA; i++)
    {
        printf(" [ ");
        for(int j = 0 ; j< colonneB; j++){
            if (matriceC[i][j]==0){
                printf("%d, ",matriceC[i][j]);
            }
            else{
                printf("%d, ",matriceC[i][j]);
            }
        }
        printf("]\n");
        
    }
     
     
    


}

void main(){

pthread_t th[righeA +1];

sem_unlink(nomesem);
sem_unlink(nomesem2);
semaforo = sem_open(nomesem, O_CREAT | O_EXCL,0644,0);
mux = sem_open(nomesem2, O_CREAT | O_EXCL,0644,1);


//Allocazione dinamica matriceA 
matriceA = (int**)malloc(righeA*sizeof(int*));
for (int i = 0 ; i < righeA ; i++){
    matriceA[i] = (int*)malloc(colonneA*sizeof(int));
}

srand(time(NULL));

//Riempimento matrice A
for (int i = 0; i < righeA; i++)
{
    for(int j = 0 ; j< colonneA; j++){
        matriceA[i][j] = rand()%100 +i ;
    }
    
}

//Stampa matrice A
for (int i = 0; i < righeA; i++)
{
    printf(" [ ");
    for(int j = 0 ; j< colonneA; j++){
        printf("%d, ",matriceA[i][j]);
    }
    printf("]\n");
    
}

//Allocazione matrice B
matriceB = (int**)malloc(righeB*sizeof(int*));
for (int i = 0 ; i < righeB; i++){
    matriceB[i] = (int*)malloc(colonneB*sizeof(int));
}

//Riempimento matrice B
for (int i = 0; i < righeB; i++)
{
    for(int j = 0 ; j< colonneB; j++){
        matriceB[i][j] = rand()%100 +i ;
    }
    
}

printf("\n////////////\n");

//Stampa matrice B
for (int i = 0; i < righeB; i++)
{
    printf(" [ ");
    for(int j = 0 ; j< colonneB; j++){
        printf("%d, ",matriceB[i][j]);
    }
    printf("]\n");
    
}

//Allocazione matrice risultato C
matriceC = (int**)malloc(righeA*sizeof(int*));
for(int i = 0 ; i < righeA; i++){
    matriceC[i] = (int*)malloc(colonneB*sizeof(int));
}

for(int i= 0 ; i<righeA+1; i++){
    if(i == righeA){
        pthread_create(&th[i],NULL,StampaRisultato,NULL);
    }
    else{
        int* riga = malloc(sizeof(int)); 
        *riga = i;
        pthread_create(&th[i],NULL,CalcoloProdotto,riga);
    }
}

printf("\n////////////\n");
for(int i= 0 ; i<righeA+1; i++){
    pthread_join(th[i],NULL);
}


sem_close(semaforo);
sem_unlink(nomesem);
sem_close(mux);
sem_unlink(nomesem2);

}