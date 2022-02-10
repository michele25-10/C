#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]){    
    if(argc != 2){
        printf("Errore");
        exit(0);
    }
    char stringa;
    int piped[2];
    pipe(piped);
    int pid;
    int status;
    int tot = 0;
    int risultato = 0;
    pid = fork();
    if (pid > 0){
        do{
            printf("stringa:\n");
            scanf("%s", &stringa);
            write(piped[1], &stringa, sizeof(stringa));
            wait(&status);                  //neccessario altrimenti non termina il ciclo for
            read(piped[0], &risultato, 1024);
            printf("%d\n", risultato);
            tot = tot + risultato;
        }while(strcmp(&stringa, "fine") != 0);
        printf("%d\n", tot);
        return 0;
    }
    else{
        close(0);
        dup(piped[0]);
        close(piped[0]);
        close(1);
        dup(piped[1]);
        close(piped[1]);
        printf("%s", &stringa);
        execl("bin/grep", "-c", &stringa, NULL);
        exit(0);
    }
}