/*

solito eser del prodotto di matrici per con mutex var cond e struct
matrice MxN e matrice NxP
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<fcntl.h>

#define M 3
#define N 2
#define P 2

struct informazioni{
	int** matriceA;
	int** matriceB;
	int** matriceC;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int sblocco;

}typedef informazioni;


void* Prodotto(void* arg){

informazioni* info = (informazioni*)arg;
int somma ;



for(int i =  0; i < M; i++){
	for (int j = 0 ; j < P ; j++){
		somma = 0 ;
		for(int k = 0 ; k < N ; k++){
			pthread_mutex_lock(&info->mutex);
			somma += info->matriceA[i][k]*info->matriceB[k][j];
			pthread_mutex_unlock(&info->mutex);
		}
		pthread_mutex_lock(&info->mutex);
		info->matriceC[i][j] = somma;
		pthread_mutex_unlock(&info->mutex);
		
	}
}
pthread_mutex_lock(&info->mutex);
info->sblocco = 1;
pthread_cond_signal(&info->cond);
pthread_mutex_unlock(&info->mutex);

}


void* Stampa(void*arg){

informazioni* info = (informazioni*)arg;

//stampa matriceC
printf("\nStampa matriceC\n");
for(int i = 0 ; i < M ; i++){
	printf("[ ");
	for(int j = 0 ; j < N ; j++){
		pthread_mutex_lock(&info->mutex);
		while(!info->sblocco){
			pthread_cond_wait(&info->cond,&info->mutex);
		}
		printf("%d, ",info->matriceC[i][j]);
		pthread_mutex_unlock(&info->mutex);
	}
	printf("]\n");
}




}


void main(){
	
pthread_t th[M+1];
informazioni info;
int random;

srand(time(NULL));
info.sblocco = 0;

//inizializzazione mutex e var cond
pthread_mutex_init(&info.mutex,NULL);
pthread_cond_init(&info.cond,NULL);

//allocazione matriceA
info.matriceA = (int**)malloc(M*sizeof(int*));
for(int i = 0 ; i < M ; i++){
	info.matriceA[i]= (int*)malloc(N*sizeof(int));
}

//allocazione matriceB
info.matriceB = (int**)malloc(N*sizeof(int*));
for(int i = 0 ; i < N ; i++){
	info.matriceB[i]= (int*)malloc(P*sizeof(int));
}

//Riempimento matrice A e matrice B
for(int i = 0 ; i < M ; i++){
	for(int j = 0 ; j < N ; j++){
		random = rand()%10+1;
		info.matriceA[i][j] = random;
	}
}

for(int i = 0 ; i < N ; i++){
	for(int j = 0 ; j < P ; j++){
		random = rand()%10+1;
		info.matriceB[i][j] = random;
	}
}

//allocazione matriceC
info.matriceC = (int**)malloc(M*sizeof(int*));
for(int i = 0 ; i < M ; i++){
	info.matriceC[i]= (int*)malloc(P*sizeof(int));
}

//stampa matriceA 
printf("\nStampa matriceA\n");
for(int i = 0 ; i < M ; i++){
	printf("[ ");
	for(int j = 0 ; j < N ; j++){
		printf("%d, ",info.matriceA[i][j]);
	}
	printf("]\n");
}

//stampa matriceB
printf("\nStampa matriceB\n");
for(int i = 0 ; i < N ; i++){
	printf("[ ");
	for(int j = 0 ; j < P ; j++){
		printf("%d, ",info.matriceB[i][j]);
	}
	printf("]\n");
}

for(int i = 0 ; i < M+1; i++){
	if(i == M){
		pthread_create(&th[i],NULL,Stampa,&info);
	}
	else{
		pthread_create(&th[i],NULL,Prodotto,&info);
	}
}

for(int i = 0 ; i < M+1; i++){
	pthread_join(th[i],NULL);
}


//stampa matriceC
printf("\nStampa matriceC\n");
for(int i = 0 ; i < M ; i++){
	printf("[ ");
	for(int j = 0 ; j < N ; j++){
		printf("%d, ",info.matriceC[i][j]);
	}
	printf("]\n");
}


pthread_mutex_destroy(&info.mutex);
pthread_cond_destroy(&info.cond);
}



