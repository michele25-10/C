#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define READ 0
#define WRITE 1

int main(int argc, char * argv[]){
    if(argc!=2){
        printf("Errore campi dati");
        exit(0);
    }
    int p0p1[2];
    int p1p2[2];
    char cod[10];
    char n[10];
    char ris[10];
    int pid1;
    int pid2;
    pipe(p0p1);
    do{ 
        printf("Inserisci il codice:\n");
        scanf("%s", cod);
        pid1 = fork();
        if (pid1 < 0){
            printf("Errore di creazione figlio\n");
            exit(1);
        }
        if(pid1 == 0){
            close(1);
            dup(p0p1[WRITE]);
            close(p0p1[WRITE]);
            close(p0p1[READ]);
            execl("/bin/grep", "grep", "-w", "-c", cod, argv[1], NULL);
        }
        if (pid1 > 0){
            wait(&pid1);
            read(p0p1[READ], n, sizeof(n));
            if (atoi(n) != 0){
                pipe(p1p2);
                pid2 = fork();
                if (pid2 < 0){
                    printf("Errore di creazione figlio\n");
                    exit(1);
                }
                if (pid2 == 0){
                    close(1);
                    dup(p1p2[WRITE]);
                    close(p1p2[WRITE]);
                    close(p1p2[READ]);
                    close(p0p1[READ]);
                    close(p0p1[WRITE]);
                    execl("/bin/grep", "grep", "-w", "-c", strcat(cod, " insoluto"), argv[1], NULL);
                }
                if (pid2>0){
                    wait(&pid2);
                    read(p1p2[READ], ris, sizeof(ris));
                    printf("sono state trovate %i fatture non pagate dal cliente: %s\n", atoi(ris), cod);
                }
            }
        }
    }while(strcmp(cod, "fine") != 0);
    close(p0p1[READ]);
    close(p0p1[WRITE]);
    close(p1p2[READ]);
    close(p1p2[WRITE]);
    return 0;
}