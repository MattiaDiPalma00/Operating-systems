/*
Scrivere un programma che con l'uso della libreria pthread calcoli in modo concorrente l'espressione:
4x^4 + 2y^3 + z^2 -sqrt(b). il valore complessivo della funzione va calcolato dal thread, principale,
i valori delle variabili , inseriti da console, ciascun thread finito di calcolare il proprio termine
dell'espressione inserisce il proprio tid in un array di dimensioni pari al numero di thread, ad indicare l'ordine
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<math.h>

#define N 4

struct informazioni{
	int x;
	int y;
	int z;
	int b;
	int*array;
	int indice;
	pthread_mutex_t mutex;

}typedef informazioni;

int*riga;

void* Calcolo(void* arg){

informazioni* info = (informazioni*) arg;

sleep(1);

switch(info->indice){
	case 0:
		pthread_mutex_lock(&info->mutex);
		info->array[0] = 4*(pow(info->x,4));
		info->indice++;
		pthread_mutex_unlock(&info->mutex);
		
		
	case 1:
		pthread_mutex_lock(&info->mutex);
		info->array[1] = 2*(pow(info->y,3));
		info->indice++;
		pthread_mutex_unlock(&info->mutex);
		
	
	case 2: 
		pthread_mutex_lock(&info->mutex);
		info->array[2] = pow(info->z,2);
		info->indice++;
		pthread_mutex_unlock(&info->mutex);
		
	
	case 3: 
		pthread_mutex_lock(&info->mutex);
		info->array[3] = sqrt(info->b);
		info->indice++;
		pthread_mutex_unlock(&info->mutex);
		
		
		
}

}


int main(int argv, char** argc){

informazioni info;
pthread_t th[N];

if(argv != 5){
	printf("\nINSERIRE LE VARIABILI IN QUESTO ORDINE : X,Y,Z e B\n");
	return -1;

}

//allocazione mutex
pthread_mutex_init(&info.mutex,NULL);

//inizializzazione variabili 
info.x = atoi(argc[1]);
info.y = atoi(argc[2]);
info.z = atoi(argc[3]);
info.b = atoi(argc[4]);


//allocazione degli arrray 
info.array = (int*)malloc(N*sizeof(int));
riga = (int*)malloc(N*sizeof(int));
info.indice = 0 ;

for(int i = 0 ; i <N ; i++){
	//int *tmp = malloc(sizeof(int));
	//*tmp = i ;
	riga[i] = i;
	pthread_create(&th[i],NULL,Calcolo,&info);
}

for(int i = 0 ; i <N ; i++){

	pthread_join(th[i],NULL);
}

printf("valori di x y z b : ");
for(int i = 0 ; i <N ; i++){
	printf("%d , ",info.array[i]);
}

int sommaF=0;
for(int i = 0 ; i <N ; i++){
	sommaF+=info.array[i];
	
}

printf("\nSomma finale : %d \n",sommaF);




pthread_mutex_destroy(&info.mutex);

}
