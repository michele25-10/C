#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char * argv[]){
    char stringa[50];
    int p;
    int fd;
    int p1p2[2];

    if(pipe(p1p2)<0){
        printf("Errore");
        exit(0);
    }

    p = fork();
    if (p<0){
        printf("Errore pid");
        exit(0);
    }
    
    if(p==0){
        close(p1p2[0]);
        while(1){
            printf("Inserisci una stringa \n");
            scanf("%s", stringa);
            if(strcmp(stringa, "fine") == 0){
                close(p1p2[1]);
                exit(0);
            }
            strcat(stringa, "\n");
            write(p1p2[1], stringa, strlen(stringa));
        }
    }

    close(p1p2[1]);

    p = fork();
    if (p<0){
        printf("Errore nel pid");
        exit(0);
    }

    if (p==0){
        close(0);
        dup(p1p2[0]);
        close(p1p2[0]);

        close(1);
        if((fd = open("es8.txt", O_RDWR|O_CREAT, 0777))<0){
            printf("Errore apertura file");
            exit(0);
        }

        execl("/usr/bin/sort", "sort", (char *)0);
        return -1;
    }

    wait(&p);

    close(p1p2[0]);

    return 0;
}