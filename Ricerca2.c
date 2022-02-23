#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define READ 0
#define WRITE 1

int main(int argc, char*argv[]){
    if(argc != 2){
        printf("Errore, manca un campo dato");
        exit(1);
    }
    
    int pid1;
    int pid2;
    char str[50];
    int p1p2[2];
    int p2p0[2];
    char ris[32];
    int fd;
    int nread;

    do{
        if(pipe(p1p2)<0){
            printf("Errore creazione pipe");
            exit(1);
        }
        printf("Inserisci la parola da cercare\n");
        scanf("%s", str);
        pid1 = fork();
        if (pid1<0){
            printf("Errore nella creazione del figlio\n");
            exit(1);
        }
        if(pid1 == 0){
            close(1);
            dup(p1p2[WRITE]);
            close(p1p2[WRITE]);
            close(p1p2[READ]);
            execl("/bin/cat", "cat", argv[1], NULL);
            return -1;
        }

        if(pipe(p2p0)<0){
            printf("Errore creazione pipe");
            exit(1);
        }

        pid2 = fork();
        if (pid2 < 0){
            printf("Errore di creazione del figlio\n");
            exit(1);
        }
        if (pid2 == 0){
            close(0);
            dup(p1p2[READ]);
            close(p1p2[READ]);
            close(1);
            dup(p2p0[WRITE]);
            close(p2p0[WRITE]);
            close(p1p2[WRITE]);
            close(p2p0[READ]);
            execl("/bin/grep", "grep", "-c", str, NULL);
            return -1;
       }

        close(p1p2[READ]);
        close(p1p2[WRITE]);
        close(p2p0[WRITE]);
        wait(&pid2);
        close(1);
        fd = open("output.txt", O_RDWR | O_CREAT, 0777);
        if (fd<0){
            printf("Errore apertura file");
            exit(1);
        }
        while ((nread = read(p2p0[READ], ris, 32))>0){
            write(1, ris, nread);
        }
        close(p2p0[READ]);
        close(fd);
        
    }while(strcmp(str, "fine") != 0);

    return 0;
}