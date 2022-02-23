#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char * argv[]){
    if(argc != 2){
        printf("Mancano dei dati");
        exit(0);
    }

    int pid1;
    int pid2;
    char ris[24];
    int p1p2[2];
    int p2p0[2];
    int nread;
    int fd;

    
    if (pipe(p1p2) < 0){
        printf("Errore creazione della pipe");
        exit(1);
    }

    pid1 = fork();
    if (pid1<0){
        printf("Errore nella creazione del figlio");
        exit(1);
    }
    if (pid1 == 0){
        close(p1p2[0]);
        close(1);
        dup(p1p2[1]);
        close(p1p2[1]);
        execl("/bin/cat", "cat", argv[1], NULL);
        return -1;
    }

    if(pipe(p2p0) < 0){
        printf("Errore nella creazione della pipe");
        exit(1);
    }
    wait(&pid1);
    pid2=fork();
    if (pid2 < 0){
        printf("Errore nella creazione del figlio");
        exit(1);
    }
    if(pid2 == 0){
        close(0);
        dup(p1p2[0]);
        close(p1p2[0]);
        close(p1p2[1]);
        close(1);
        dup(p2p0[1]);
        close(p2p0[1]);
        close(p2p0[0]);
        execl("/bin/wc", "wc", NULL);
        return -1;
    }

    close(p1p2[0]);
    close(p1p2[1]);
    close(p2p0[1]);

    wait(&pid2);
    close(1);
    fd = open("output.txt", O_RDWR | O_CREAT, 0777);
    if(fd<0){
        printf("Errore nell'apertura del file");
        exit(1);
    }
    
    while((nread = read(p2p0[0], ris, 24)) > 0)
    {
        write(1, ris, nread);
    }

    close(p2p0[0]);

    return 0;
}