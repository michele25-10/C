// / @file Es2Libro.c
// / @brief file dell'esercizio 2 del libro pdf
// / @data 26/11/2021
// / @autore Michele Gabrieli

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

/* *
* @fn int main()
* @brief funzione principale che chiede in input un numero e poi crea un processo figlio, successivamente il processo padre somma 10 al numero e il processo figlio somma 15. Fatte le operazioni vengono stampate le due somme
*/

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("\nErrore");
        exit(1);
    }
    int pid; 
    int status;
    int numero = atoi(argv[1]);
    pid = fork();
    if(pid == 0){
        numero = numero + 15;
        printf("La somma del figlio: %d\n", numero);
        exit(1);
    }
    else{
        pid = wait(&status);
        numero = numero + 10;
        printf("La somma del padre: %d\n", numero);
        return 0;
    }
}
