/*
Data una matrice NxN(Npari)di interi generati casualmente, allocata dinamicamente, con N argomento da riga di comando
creare N thread che prelevano casualmente un elemento dalla riga di competenza e lo inseriscano concorrentemente
in un vettore di (n+1)/2 elementi. un thread n+1 esiemo attende il riempimento del vettore per stampare il contenuto
usare mutex e var di condizione.
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

pthread_cond_t cond;
pthread_mutex_t mux;
int** matrice;
int* vettore;
int count;
int boleano;
int N ;
int dim;

void* Routine(void* arg){

int riga = *(int*) arg;
int random;

random = rand()%N;
pthread_mutex_lock(&mux);
if(riga < dim){
	vettore[riga] = matrice[riga][random];
}
	count++;
pthread_mutex_unlock(&mux);

if(count == dim){
	pthread_mutex_lock(&mux);
	boleano = 1;
	pthread_mutex_unlock(&mux);
	pthread_cond_signal(&cond);
	pthread_exit(NULL);
}
	

}

void* Stampa(){

pthread_mutex_lock(&mux);
printf("\n[ ");
for(int i = 0 ; i < dim ; i++){
	while(boleano == 0){
		pthread_cond_wait(&cond,&mux);
	}
	
	printf("%d, ",vettore[i]);
}
printf("]");
	
pthread_mutex_unlock(&mux);
pthread_exit(NULL);

}

int main(int argv, char**argc){

int* riga;
int random;
boleano = 0 ;


if(argv !=2){
	printf("\nERRORE, INSERIRE LA DIMENSIONE DELLA MATRICE N\n");
	return -1;
}

N = atoi(argc[1]);
dim = (N+1)/2;
count = 0;
pthread_t th[N+1];

if (N%2 !=0){
	printf("\nERRORE, INSERIRE N PARI\n");
	return -1;
}

srand(time(NULL));
pthread_mutex_init(&mux,NULL);
pthread_cond_init(&cond,NULL);

//ALLOCAZIONE MATRICE
matrice= (int**)malloc(N*sizeof(int*));
for(int i = 0 ; i < N ; i++){
	matrice[i] = (int*)malloc(N*sizeof(int));
}

//riempimento matrice
for(int i = 0 ; i < N ; i++){
	for(int j = 0 ; j< N ; j++){
		random = rand()%100 + 1 ;
		matrice[i][j] = random;
	}
}

//STAMPA MATRICE 
printf("\nSTAMPA DELLA MATRICE\n");
for(int i = 0 ; i < N ; i++){
	printf("[ ");
	for(int j = 0 ; j< N ; j++){
		printf("%d, ",matrice[i][j]);
	}
	printf("]\n");
}

//ALLOCAZIONE VETTORE
vettore = (int*)malloc(dim*sizeof(int));



for(int i = 0 ; i <N+1 ; i++){
	if(i == N){
		pthread_create(&th[i],NULL,Stampa,NULL);
	}else{
		riga = malloc(sizeof(int));
		*riga = i;
		pthread_create(&th[i],NULL,Routine,riga);
	}
}

for(int i = 0 ; i <N+1 ; i++){
	pthread_join(th[i],NULL);
}

pthread_mutex_destroy(&mux);
pthread_cond_destroy(&cond);


}

