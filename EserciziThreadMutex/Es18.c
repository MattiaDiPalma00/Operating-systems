/*
Scrivere un programma in C e posix che prende N interi compresi tra 5 e 20, il valore di N è costante ed è indicato nel sorgente 
il programma avvia N thread che :

1) ogni thread va in sospensione pari al valore del parametro 
2) alla fine del periodo si sospensione stampa un messaggio thread# _ svegliato!.
3) tutti i thread si sincronizzano con samfori su memoria
4) dopo aver atteso il termine di tutti i thread, ciascun thread scrive sullo schermo thread#: terminato;
*/

#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>

#define N 5 

int* array;
sem_t semaforo;
sem_t semaforo2;


void* routine(void*arg){

int riga = *(int*) arg;

long int* risultato = malloc(sizeof(long int));
*risultato = pthread_self();
//(long int*)malloc(N*sizeof

sem_wait(&semaforo);
sleep(array[riga]);
printf("thread %ld svegliato\n",pthread_self());
sem_post(&semaforo);


//sem_wait(&semaforo2);
//printf("thread %ld terminato\n",pthread_self());
//strcpy(risultato,
//sem_post(&semaforo2);

return risultato;


}


int main(){

pthread_t th[N];
array = (int*)malloc(N*sizeof(int));
sem_init(&semaforo,0,1);
sem_init(&semaforo2,0,N);
int* res;

printf("Inserisci %d numeri compresi tra 5 e 20.",N);

for (int i = 0 ; i < N ; i++){
	printf("\n-> ");
	scanf("%d",&array[i]);
	if(array[i] >20 || array[i]<5){
		printf("Errore, hai inserito un numero non compreso tra 5 e 20.");	
		return -1;
	}
}

for(int i = 0 ; i < N ; i++){
	int* riga = malloc(sizeof(int));
	*riga = i;
	pthread_create(&th[i],NULL,routine,riga);
}


for(int i = 0 ; i < N ; i++){
	pthread_join(th[i],(void**)&res);
	printf("thread %d terminato\n",*res);
}

sem_destroy(&semaforo);
sem_destroy(&semaforo2);



}
