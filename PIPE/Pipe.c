/// @file       Pipe.c
/// @brief      programma che permette, istanziando un processo ed una pipe, di sommare dei numeri randomici e sommarli
/// @date       15/12/2021
/// @author     Michele Gabrieli    

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

/// @param MAX: numeri da generare
#define MAX 10

/**
* @fn void GeneraNumeri()
* @brief Funzione che permette di generare i numeri
*/
void GeneraNumeri(int numeri[]){
    srand(time(0)); /*Inizializziamo il generatore sull'ora attuale dell'elaboratore
                    time 0, in maniera tale da avere valori diversi*/
    for (int i = 0; i < MAX; i++){
        numeri[i] = 1 + rand()%10; //random da 1 a 100
    }
}

/**
* @fn           int main()
* @brief        funzione principale che crea una pipe, successivamente istanzia un processo figlio. Il processo padre genera 10 numeri casuali e li passa al figlio utilizzando la pipe. Successivamente il processo figlio li somma e passa la somma al padre, il quale stampera' il risultato
*/ 
int main(){
    int Canale1[2];
    int Canale2[2];
    int numeri[MAX];
    int somma = 0;
    int valore;
    pipe(Canale1);
    pipe(Canale2);
    int pid;
    pid = fork();
    if(pid > 0){        //padre
        GeneraNumeri(numeri);
        close(Canale1[0]);   //chiudo read
        close(Canale2[1]);   //chiudo write
        for(int i = 0; i < MAX; i++){
            write(Canale1[1], &numeri[i], MAX);
        }
        read(Canale2[0], &somma, sizeof(int));
        printf("La somma è: %d\n", somma);
        return 0;
    }
    else{               //figlio
        close(Canale1[1]);  //chiudo write
        close(Canale2[0]);  //chiudo read
        for(int i = 0; i < MAX; i++){
            read(Canale1[0], &numeri[i], MAX);
            somma = somma + numeri[i];
        }
        write(Canale2[1], &somma, sizeof(int));
        exit(0);
    }

}