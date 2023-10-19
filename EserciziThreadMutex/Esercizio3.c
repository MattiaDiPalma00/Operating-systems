/*
Si realizzi un programma in C e Posix sotto Linux che, 
utilizzando la libreria Pthread.
Lanciamo M thread per calcolre il prodotto di due matrici di dimensione MxN e NxP
dopo effettuato il prodotto, un M+1 esimo thread che era in attesa ne stamperà il risultato 
usare mutex e variabili di condizione.
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>


pthread_mutex_t mutex;
pthread_cond_t cond;
struct Esercizio3
{
    int righeA;
    int colonneA;
    int colonneB;
    int** matriceA;
    int** matriceB;
    int** matriceC; 

}typedef Dati;


void* CalcoloProdotto(void* inf){

Dati* info = (Dati*) inf;
int valore ; 

for(int i = 0 ; i < info->righeA; i++){
    for(int j = 0 ; j < info->colonneB ;j++){
        valore = 0;
        for (int k = 0 ; k<info->colonneA; k++){
            valore += info->matriceA[i][k] * info->matriceB[k][j];
        }
        pthread_mutex_lock(&mutex);
        info->matriceC[i][j] = valore;
        pthread_mutex_unlock(&mutex);
    }
}
pthread_cond_signal(&cond);



}

void* StampaMatrixProdotto(void* inf){

Dati* info = (Dati*) inf;

printf("\n RISULTATO  \n");

pthread_mutex_lock(&mutex);
for(int i = 0 ; i < info->righeA; i++){
    printf("[");
    for(int j = 0 ; j <info->colonneB ;j++){
        while(info->matriceC[i][j] == 0 ){
            pthread_cond_wait(&cond,&mutex);
        }
        printf(" %d, ",info->matriceC[i][j]);
    }
    printf("]\n");
}
pthread_mutex_unlock(&mutex);
}


void main(int argc ,char* argv[]){  // char* arr[] è uguale ad un array di puntatori a caratteri 

pthread_mutex_init(&mutex,NULL);
pthread_cond_init(&cond,NULL);

//in questo caso sono uguali perchè è l'ipotesi di un quadrato
int righeA = 3;//atoi(argv[1]);
int colonneA = 2;//atoi(argv[1]);

int righeB= 2;
int colonneB = 2;
pthread_t th[righeA+1];
Dati info;

//ALLOCAZIONE MATRICE A
int **matrixA = (int **)malloc(righeA * sizeof(int *));
for (int i = 0; i < righeA; i++) {
    matrixA[i] = (int *)malloc(colonneA * sizeof(int));
}

//ALLOCAZIONE MATRICE B
int **matrixB = (int **)malloc(righeB * sizeof(int *));
for (int i = 0; i < righeB; i++) {
    matrixB[i] = (int *)malloc(colonneB * sizeof(int));
}
srand(time(NULL));

int righeC = righeA;
int colonneC = colonneB;

//ALLOCAZIONE MATRICE C (risultato) sarà C[righeA][colonneB]
int **matrixC = (int **)malloc(righeC* sizeof(int *));
for (int i = 0; i < righeC; i++) {
    matrixC[i] = (int *)malloc(colonneC * sizeof(int));
}


//INSERIMENTO MATRICE A

for(int i = 0 ; i < righeA; i++){
    for(int j = 0 ; j <colonneA ;j++){
        matrixA[i][j] = rand()%255;
    }
}

//INSERIMENTO MATRICE B
for(int i = 0 ; i < righeB; i++){
    for(int j = 0 ; j <colonneB ;j++){
        matrixB[i][j] = rand()%255;
    }
}

info.colonneA = colonneA;
info.colonneB = colonneB;
info.righeA = righeA;
info.matriceA = matrixA;
info.matriceB = matrixB;
info.matriceC = matrixC;



printf("\n STAMPA MATRICE A\n");
//STAMPA MATRICE A
for(int i = 0 ; i < righeA; i++){
    printf("[");
    for(int j = 0 ; j <colonneA ;j++){
        printf(" %d, ",matrixA[i][j]);
    }
    printf("]\n");
}

printf("\n STAMPA MATRICE B\n");

//STAMPA MATRICE B
for(int i = 0 ; i < righeB; i++){
    printf("[");
    for(int j = 0 ; j <colonneB ;j++){
        printf(" %d, ",matrixB[i][j]);
    }
    printf("]\n");
}

printf("\n CALCOLO MATRICE C RISUTLATO \n");

for (int i = 0 ; i < righeC+1; i++){
    if(i == righeC){
        pthread_create(&th[i],NULL,StampaMatrixProdotto,(void*)&info);

    }
    else{
    pthread_create(&th[i],NULL,CalcoloProdotto,&info);
    }
    
}

for (int i = 0 ; i < righeC+1; i++){
    pthread_join(th[i],NULL);
    
}


for (int i = 0; i < righeC; i++) {
    free(matrixC[i]);
    free(matrixA[i]);
}

for (int i = 0; i < righeB; i++) {
    free(matrixB[i]);
}

free(matrixC);
free(matrixA);
free(matrixB);

pthread_cond_destroy(&cond);
pthread_mutex_destroy(&mutex);
}