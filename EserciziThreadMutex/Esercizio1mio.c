/*
Si realizzi un programma in C e Posix sotto Linux che, 
utilizzando la libreria Pthread
• lancia n thread per cercare un elemento in una matrice nxn di 
caratteri
• Ognuno dei thread cerca l'elemento in una delle righe della 
matrice
• Non appena un thread ha trovato l'elemento cercato, rende note 
agli altri thread le coordinate dell'elemento e tutti i thread
terminano (sono cancellati)
*/

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

#define dimensione 3

void* cerca_carattere(void*x);


struct Eserc
{
    int cordinataI; 
    int cordinataJ;
    char target;
    int f;

}typedef Esercizio1;

Esercizio1 eser;
 int f= 0 ; // falso

char  matrice[3][3] = {
    {'a','b','p'},
    {'c','d','5'},
    {'c','d','l'}
};
pthread_mutex_t mux;

void main(){
   
    pthread_t th[2];

    eser.target = '5';
    pthread_mutex_init(&mux,NULL);
    int i ;
    int riga ;
   


    for(i = 0 ; i <dimensione ; i++){
        riga = i;
        pthread_create(&th[i],NULL,cerca_carattere,(void*)&riga);
    }
    for(int i = 0 ; i <dimensione ; i++){
        pthread_join(th[i],NULL);
    }

    if (f)
        printf("\nLe cordinate sono riga : %d colonna : %d\n",eser.cordinataI,eser.cordinataJ);
    else 
        printf("carattere non trovato");

        pthread_mutex_destroy(&mux);
       

}

void* cerca_carattere(void* x){
    int riga = *(int*) x; // cosi accedi all'interno dell'indirizzo di quello che hai passato come parametro 

    for(int j =0 ; j<dimensione; j++){
        pthread_mutex_lock(&mux);
            if (f){
                pthread_mutex_unlock(&mux);
                break;
            }
            if (eser.target == matrice[riga][j]){
                f = 1 ;
                eser.cordinataI = riga;
                eser.cordinataJ = j ;
            
            }
        pthread_mutex_unlock(&mux);
    }
    pthread_exit(NULL);   

}