/*
data una matrice NxN con N pari di interi generati casualmente allo.din.
creare N thread che prelevano casulamente elementi dalla riga di competenza e lo inseriscano in un vettore 2N;
un thread N+1-iesimo stampa il contenuto del vettore e il numero di elemnti inserito da ognuno. usare i semafori con nome
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<time.h>
#include<fcntl.h>

#define nomesem "/semaforo1"
#define nomesem2 "/semaforo2"


struct informazioni{
	int** matrice;
	int* vettore;
	int N;
	sem_t* semaforo1;
	sem_t* semaforo2;
	long int *riga;
	int finito ;
	int*array;
	int*myInt;
	
}typedef informazioni;

struct informazionitest{
	int* riga;
	
}typedef informazionitest;


void* test(void*arg){

informazionitest* info = (informazionitest*) arg;

int riga = *(info->riga);

printf("%d, ", riga);

}

void* Inserimento(void* arg){

informazioni* info = (informazioni*) arg;
int rigaprova2 = info->myInt[3];
int random ;
int rigatmp;
int count = 0 ;
int var = 0;

sem_wait(info->semaforo1);
sem_post(info->semaforo1);


sem_wait(info->semaforo1);
for(int i = 0 ; i < info->N ; i++){
	
	if(info->riga[i] == pthread_self()){
		rigatmp = i;
	}
	
}
sem_post(info->semaforo1);



random = rand()%info->N;
	
if(info->finito + 1 < info->N  ){
	sem_wait(info->semaforo1);
	info->vettore[rigatmp] = info->matrice[rigatmp][random];
	info->finito++;
	var++;
	info->array[rigatmp] = var;
	sem_post(info->semaforo1);
}
else{
	while(info->finito < 2*info->N){
		random = rand()%info->N;
		int rigarandom = rand()%info->N;
		sem_wait(info->semaforo1);
		info->vettore[info->finito] = info->matrice[rigarandom][random];
		info->finito++;
		var++;
		info->array[rigatmp] = var;
		sem_post(info->semaforo1);
		}
}

sem_post(info->semaforo2);
}

void* Stampa(void* arg){

informazioni* info = (informazioni*) arg;

printf("\n[ ");
for(int i = 0 ; i < 2*info->N ; i++){
	sem_wait(info->semaforo2);
	printf("%d ,",info->vettore[i]);
	sem_post(info->semaforo2);
}
printf("]\n");


}

int main(int argv, char** argc){

informazioni info;
int random;


if(argv != 2){
	printf("\nINSERIRE LA DIMENSIONE N\n");
	return -1;
}


info.N = atoi(argc[1]);
pthread_t th[info.N+1];
info.finito = 0 ;
informazionitest infotest[info.N];
pthread_t thtest[info.N];
srand(time(NULL));

sem_unlink(nomesem);
sem_unlink(nomesem2);
info.semaforo1 = sem_open(nomesem,O_CREAT | O_EXCL,0644,1);
info.semaforo2 = sem_open(nomesem2,O_CREAT | O_EXCL,0644,0);


//allocazione matrice 
info.matrice = (int**)malloc(info.N*sizeof(int*));
for(int i = 0 ; i < info.N ; i++){
	info.matrice[i] = (int*)malloc(info.N*sizeof(int));
}

for(int i = 0 ; i < info.N ; i++){
	for(int j = 0 ; j < info.N ; j++){
		random = rand()%100;
		info.matrice[i][j] = random;
	}
}


//allocazione vettore 
info.vettore = (int*)malloc((2*info.N)*sizeof(int));
info.riga= (long int*)malloc(info.N*sizeof(long int));
info.array = (int*)malloc(info.N*sizeof(int));
info.myInt =(int*) malloc(info.N*sizeof(int));


//stampa matrice 
printf("\nStampa matrice\n"); 
for(int i = 0 ; i < info.N ; i++){
	printf("[ ");
	for(int j = 0 ; j < info.N ; j++){
		printf("%d, ",info.matrice[i][j]);
	}
	printf("]\n");
}


for(int i = 0 ; i < info.N+1 ; i++){
	if( i == info.N){
		pthread_create(&th[i],NULL,Stampa,&info);
	}
	else{
            	
		pthread_create(&th[i],NULL,Inserimento,&info);
		info.riga[i] = th[i];
	}
	
}

for(int i = 0 ; i < info.N+1 ; i++){
	pthread_join(th[i],NULL);
}

for(int i = 0 ; i < info.N ; i++){
	infotest[i].riga = malloc(sizeof(int));
	*(infotest[i].riga) = i;
	pthread_create(&thtest[i],NULL,test,&infotest[i]);
}

for(int i = 0 ; i < info.N ; i++){
	pthread_join(thtest[i],NULL);
}


for(int i = 0 ; i < info.N ; i++){
	printf("sono il thread %d, e ho contato %d \n",i,info.array[i]);
}
printf("]");



sem_close(info.semaforo1);
sem_unlink(nomesem);
sem_close(info.semaforo2);
sem_unlink(nomesem2);



}








