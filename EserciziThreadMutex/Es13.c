/*

Si realizi un programma in C  sotto linux che lancia N thread per calcolare la somma degli elementi di ciascuna riga di una matrice
NxN allocata dinamicamente di interi generati casualmente in un intervallo [0,255].
Il calcolo della somma degli elementi di ciascuna riga deve essere effettuato concorrenetemente

1) il thread i-esimo con i pari, calcola la somma degli elementi di indice pari della riga i-esima.

2) il thread-iesimo con i dispari, calcola la somma degli elementi di indice dispari della riga i-esima.

calcolate le somme parziali si stamperà il minimo.
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

int** matrice;
int minimo;
pthread_mutex_t mutex;
int N ;
int count;

void* Somma(void* arg){

int riga = *(int*) arg;
int somma = 0 ;

printf("%d, \n",riga);

pthread_mutex_lock(&mutex);
count++;
int numero = count;
pthread_mutex_unlock(&mutex);

for(int j = 0 ; j < N ; j++){
	pthread_mutex_lock(&mutex);
	somma += matrice[riga][j];
	pthread_mutex_unlock(&mutex);
}

if(numero ==1){
	minimo = somma;
}

printf("somma : %d \n",somma);

if(somma < minimo){
	pthread_mutex_lock(&mutex);
	minimo = somma;	
	pthread_mutex_unlock(&mutex);
}

}


int main(int argv, char**argc){

int random;
minimo = 0 ;
count = 0;
int* riga;
int* rigadispari;


if(argv != 2){
	printf("INSERIRE LA DIMENSIONE N");
	return -1;
}

N = atoi(argc[1]);
srand(time(NULL));
pthread_t th[N];

pthread_mutex_init(&mutex,NULL);

//allocazione dinamica matrice 
matrice = (int**)malloc(N*sizeof(int*));
for(int i = 0 ; i < N ; i++){
	matrice[i] = (int*)malloc(N*sizeof(int));
}

//riempimento matrice
for(int i = 0 ; i < N ; i++){
	for(int j = 0 ; j < N ; j++){
		random = rand()%255 ;
		matrice[i][j] = random;
	}
}

//stampa matrice 
printf("\nSTAMPA MATRICE NxN\n");

for(int i = 0 ; i < N ; i++){
	printf("[ ");
	for(int j = 0 ; j < N ; j++){
		printf("%d, ",matrice[i][j]);
	}
	printf("]\n");
}

for(int i = 0 ; i < N ; i++){
	if(i%2 == 0){
		riga = malloc(sizeof(int));
		*riga = i;
		pthread_create(&th[i],NULL, Somma,riga);
	}else{
		rigadispari = malloc(sizeof(int));
		*rigadispari = i;
		pthread_create(&th[i],NULL, Somma,rigadispari);
		
	}
}

for(int i = 0 ; i < N ; i++){
	pthread_join(th[i],NULL);
}

printf("\nQUESTO È IL MINIMO DI TUTTE LE SOMME : %d ",minimo);



pthread_mutex_destroy(&mutex);

}
