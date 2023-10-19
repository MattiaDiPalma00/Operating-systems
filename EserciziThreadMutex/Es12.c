/*
si realizzi un programma in C e posix sotto linux che con l'ausilio di pthread, lancia M thread per calcolare il prodotto di due matrici 
di dimensione MxN e NxP, un M+1-esimo thread che resta in attesa provvederà si stampare la matrice risultato
usare semafori posix basati su nome.
*/

#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
#include<time.h>
#include<fcntl.h>

#define M 3
#define N 2
#define P 4
#define nomesem "/semaforo1"
#define nomesem2 "/semaforo2"

struct informazioni
{
	int** matriceA;
	int** matriceB;
	int** matriceC;
}typedef informazioni;

sem_t* semaforo;
sem_t* semaforoSt;

void* Prodotto(void* arg){

informazioni* info = (informazioni*)arg;
int somma ;

for(int i = 0 ; i < M ; i++){
	for (int j = 0 ; j< P ; j++){
		somma = 0 ;
		for(int k = 0 ; k<N ; k++){
			sem_wait(semaforo);
			somma+= info->matriceA[i][k]*info->matriceB[k][j];
			sem_post(semaforo);
		}
		sem_wait(semaforo);
		info->matriceC[i][j] = somma;
		sem_post(semaforo);
	}	
}


}

void* Stampa(){

}

void main(){

pthread_t th[M+1];
int** matA;
int** matB;
int** matC;
int random;
informazioni info[M+1];

//apertura semafori 
sem_unlink(nomesem);
sem_unlink(nomesem2);
semaforo = sem_open(nomesem,O_CREAT | O_EXCL , 0644, 1);
semaforoSt = sem_open(nomesem2,O_CREAT | O_EXCL , 0644, 1);

srand(time(NULL));
//allocazione matriceA
matA = (int**)malloc(M*sizeof(int*));
for (int i = 0; i < M ; i++){
	matA[i] =(int*)malloc(N*sizeof(int));
}

//allocazione matriceB
matB = (int**)malloc(N*sizeof(int*));
for (int i = 0; i < N ; i++){
	matB[i] =(int*)malloc(P*sizeof(int));
}

//allocazione matriceC
matC = (int**)malloc(M*sizeof(int*));
for (int i = 0; i < M ; i++){
	matC[i] =(int*)malloc(P*sizeof(int));
}

//riempimento random matriceA
for (int i = 0 ; i < M ; i++){
	for(int j = 0; j < N ; j++){
		random = rand()%10+1;
		matA[i][j] = random;
	}		
}

//riempimento random matriceB
for (int i = 0 ; i < N ; i++){
	for(int j = 0; j < P ; j++){
		random = rand()%10+1;
		matB[i][j] = random;
	}		
}

//riempimento matriceC
for (int i = 0 ; i < M ; i++){
	for(int j = 0; j < P ; j++){
		matC[i][j] = 0;
	}		
}


//stampa matrice A
printf("\nStampa matriceA\n");
for (int i = 0 ; i < M ; i++){
	printf("[ "); 
	for(int j = 0; j < N ; j++){
		printf("%d, ",matA[i][j]);
	}
	printf("]\n"); 		
}

//stampa matrice B
printf("\nStampa matriceB\n");
for (int i = 0 ; i < N ; i++){
	printf("[ "); 
	for(int j = 0; j < P ; j++){
		printf("%d, ",matB[i][j]);
	}
	printf("]\n"); 		
}

for(int i = 0 ;i < M+1; i++){
	if(i==M){
		pthread_create(&th[i],NULL,Stampa,NULL);
	}else{
		info[i].matriceA = matA;
		info[i].matriceB = matB;
		info[i].matriceC = matC;
		pthread_create(&th[i],NULL,Prodotto,&info[i]);
	}
}

//stampa matrice B
printf("\nStampa matriceC\n");
for (int i = 0 ; i < M ; i++){
	printf("[ "); 
	for(int j = 0; j < P ; j++){
		printf("%d, ",matC[i][j]);
	}
	printf("]\n"); 		
}






sem_close(semaforo);
sem_unlink(nomesem);
sem_close(semaforoSt);
sem_unlink(nomesem2);

}


