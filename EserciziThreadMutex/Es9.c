/*
Si realizzi un programma in C sottolinux che con la libreria Pthread lancia M thread per calcolare il prodotto 
di due matrici di dimensione MxN e NxP, un M+1 esimo thread che era in attesa, ne stamperà il risultato.
Le matrici devono essere allocate dinamicamente, usare i semafori posix basati su nome.
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<semaphore.h>
#include <fcntl.h>  

#define M 3
#define N 2
#define P 3 
#define nomesem "/semaforo"
#define nomesem2 "/semaforo2"

int**matriceA;
int**matriceB;
int**matriceC;
sem_t* semaforo;
sem_t* mux;

void*CalcoloP(void* arg){

int riga = *(int*)arg;
int somma;


for(int i = 0; i < P; i++){
	somma = 0;
	for (int j = 0 ; j < N; j++){
	somma+=matriceA[riga][j]*matriceB[j][i];
	}
	sem_wait(semaforo);
	matriceC[riga][i] = somma;
	sem_post(semaforo);
}

}

void*Stampa(){

sem_wait(mux);
printf("\nStampa matrice risultato\n");
for(int i = 0; i < M; i++){
	printf("[ ");
	for(int j = 0 ;j<P;j++){
		printf("%d, ",matriceC[i][j]);
	}
	printf("]\n");
}
}


void main(){

pthread_t th[M+1];
int random;
int* riga;

srand(time(NULL));

sem_unlink(nomesem);
sem_unlink(nomesem2);
semaforo = sem_open(nomesem,O_CREAT | O_EXCL,0644,1);
mux = sem_open(nomesem2,O_CREAT | O_EXCL,0644,0);

//Allocazione matriceA
matriceA = (int**)malloc(M*sizeof(int*));
for(int i =0 ; i < M ;i++){
	matriceA[i] = (int*)malloc(N*sizeof(int));
}
//Riempimento matriceA
for(int i = 0; i < M; i++){
	for(int j = 0; j<N;j++){
		random = rand()%10+1;
		matriceA[i][j] = random;
	}
}

printf("\nStampa matrice A\n");
for(int i = 0; i < M; i++){
	printf("[ ");
	for(int j = 0 ;j<N;j++){
		printf("%d, ",matriceA[i][j]);
	}
	printf("]\n");
}

//Allocazione matriceB
matriceB = (int**)malloc(N*sizeof(int*));
for(int i =0 ; i < N ;i++){
	matriceB[i] = (int*)malloc(P*sizeof(int));
}
//Riempimento matriceB
for(int i = 0; i < N; i++){
	for(int j = 0; j<P;j++){
		random = rand()%10+1;
		matriceB[i][j] = random;
	}
}

printf("\nStampa matrice B\n");
for(int i = 0; i < N; i++){
	printf("[ ");
	for(int j = 0 ;j<P;j++){
		printf("%d, ",matriceB[i][j]);
	}
	printf("]\n");
}

//Allocazione matriceC(risultato)
matriceC = (int**)malloc(M*sizeof(int*));
for(int i =0 ; i < M ;i++){
	matriceC[i] = (int*)malloc(P*sizeof(int));
}

for(int i = 0 ; i < M+1 ; i++){
	if(i == M){
		pthread_create(&th[i],NULL,Stampa,NULL);
	}else{
		riga = malloc(sizeof(int));
		*riga = i ;
		pthread_create(&th[i],NULL,CalcoloP,(void*)riga);
	}
}
for(int i = 0 ; i < M+1 ; i++){
	pthread_join(th[i],NULL);
}



sem_close(mux);
sem_unlink(nomesem2);
sem_close(semaforo);
sem_unlink(nomesem);

}
