// / @file Es3Libro.c
// / @brief file dell'esercizio 3 del libro pdf
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
* @brief funzione principale che crea tre processi figli, i quali stamperanno "Ciao, io sono Qui", "Ciao, io sono Quo", "Ciao, io sono Qua"
*/

int main(){
    int pid;
    int pid1;
    int pid2;
    int status;
    pid = fork();
    if (pid == 0){
        printf("Io sono qui\n");
        exit(0);
    }
    else{
        pid1= fork();
        if(pid1 == 0){
            sleep(1);
            printf("Io sono quo\n");
            exit(1);
        }
        else{
            pid2 = fork();
            if (pid2 == 0){
            sleep(2);
            printf("Io sono qua\n");
            exit(2);
            }
            else{
                waitpid(pid2, &status, 0);
                return 0;
            }
        }
    }
}
