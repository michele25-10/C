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
    char stringa[50];
    int p1p2[2];
    int p2p3[2];
    int p3p0[2];
    pipe(p1p2);
    int pid1;
    int pid2;
    int pid3;
    int tot = 0;
    int ris = 0;
    do
    {
        printf("Inserisci la stringa che vuoi cercare\n");
        scanf("%s", stringa);
        pid1 = fork(); // istanziato P1
        if (pid1 == 0) // se e' il figlio
        {
            close(1); 
            dup(p1p2[1]); 
            close(p1p2[1]);
            close(p1p2[0]);
            execl("/bin/cat", "cat", argv[1], NULL);
        }
        else{
            pipe(p2p3);
            pid2 = fork();
            if (pid2 == 0){
                close(0);
                dup(p1p2[0]);
                close(p1p2[0]);
                close(p1p2[1]);
                close(1);
                dup(p2p3[1]);
                close(p2p3[1]);
                close(p2p3[1]);
                execl("/bin/grep", "grep", "-o", stringa, argv[1], NULL);
            }
            else{
                pipe(p3p0);
                pid3 = fork();
                if (pid3==0){
                    close(0);
                    dup(p2p3[0]);
                    close(p2p3[0]);
                    close(p2p3[1]);
                    close(1);
                    dup(p3p0[1]);
                    close(p3p0[1]);
                    close(p3p0[0]);
                    execl("/bin/wc", "wc", "-l", NULL);
                }
            }
        }
        read(p3p0[0], &ris, sizeof(ris)); // legge il risultato della chiamata grep
        printf("La stringa %s e' comparsa %d volte\n\n", stringa, ris);
        tot += ris;
    } while (strcasecmp(stringa, "Fine") != 0);

    return 0;
}