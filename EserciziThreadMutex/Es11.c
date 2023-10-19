/*
scrivere un programma in C sotto linux che calcoli in modo concorrente l'espressione 4x^4+2y^3+z^2-sqrt(b).
I valori delle variabili vanno inserite da riga di comando, il valore complessivo dell'espressione deve essere calcolato
dal thread principale. ciascun thread finito di calcolare il proprio termine dell'espressione inserisce il proprio TID 
in un array pari al numero di thread, ad indicare il proprio ordine di conclusione.
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>

#define N 4

int x,y,z,b;
double* array;
long int* ordine;
pthread_mutex_t mux;

void* Routine(void* arg){

int id = *(int*) arg;
double result;
long int tmp = pthread_self();

pthread_mutex_lock(&mux);
ordine[id] = tmp;
pthread_mutex_unlock(&mux);

switch(id){
	case 0 :
		result = 4*pow(x,4);
		pthread_mutex_lock(&mux);
		array[0] = result;
		pthread_mutex_unlock(&mux);
		break;
	case 1 :
		result = 2*pow(y,3);
		pthread_mutex_lock(&mux);
		array[1] = result;
		pthread_mutex_unlock(&mux);
		break;
	case 2 :
		result = pow(z,2);
		pthread_mutex_lock(&mux);
		array[2] = result;
		pthread_mutex_unlock(&mux);
		break;
	case 3 :
		result = -sqrt(b);
		pthread_mutex_lock(&mux);
		array[3] = result;
		pthread_mutex_unlock(&mux);
		break;
}

}

int main(int argv, char**argc){
pthread_t th[N];


if(argv != 5){
	printf("\nINSERISCI I VALORI DI X,Y,Z,B\n");
	return -1;
}

pthread_mutex_init(&mux,NULL);

x = atoi(argc[1]);
y = atoi(argc[2]);
z = atoi(argc[3]);
b = atoi(argc[4]);

array = (double*)malloc(N*sizeof(double));
ordine= (long int*)malloc(N*sizeof(long int));


for(int i = 0 ; i < N ; i++){
	int* riga = malloc(sizeof(int));
	*riga = i;
	pthread_create(&th[i],NULL,Routine,riga);
}

for(int i = 0 ; i < N ; i++){
	pthread_join(th[i],NULL);
}


double sommaF = 0 ;
for(int i = 0 ; i < N ; i++){
	sommaF+=array[i];
}

for(int i = 0 ; i < N ; i++){
	printf("%5.2f, ",array[i]);
}

printf("\nLa somma risulta : %5.2f",sommaF);

printf("\nL'ordine di calcolo risulta :\n");
for (int i = 0 ; i < N ; i++){
	printf("posizione : %d -> %ld \n",i,ordine[i]);
}




pthread_mutex_destroy(&mux);






}
