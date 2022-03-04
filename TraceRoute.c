#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char* argv[]){
    if(argc!=2){
        printf("Errore, mancano campi dati");
        exit(1);
    }
    int p1;
    int p2;
    int p3;
    int p1p2[2];
    int p2p3[2];
    int p3p0[2];
    if(pipe(p1p2)<0){
        printf("Errore creazione pipe");
        exit(1);
    }
    p1 = fork();
    if (p1 <0){
        printf("Errore creazione pipe");
        exit(1);
    }
    if (p1==0){
        close(1);
        dup(p1p2[1]);
        close(p1p2[1]);
        close(p1p2[0]);
        
        execl("/user/bin/traceroute", "traceroute", argv[1], NULL);
        return -1;
    }

    if (pipe(p2p3)<0){
        printf("Errore creazione pipe");
        exit(1);        
    }

    p2 = fork();
    if (p2 < 0){
        printf("Errore creazione pipe");
        exit(1);        
    }

    if (p2==0){
        close(0);
        dup(p1p2[0]);
        close(p1p2[0]);
        close(p1p2[1]);
        
        close(1);
        dup(p2p3[1]);
        close(p2p3[1]);
        close(p2p3[0]);

        execl("/user/bin/awk", "awk", "{print $4}", NULL);
        return -1;
    }

    close(p1p2[0]);
    close(p1p2[1]);
    
    if(pipe(p3p0)<0){
        printf("Errore creazione pipe");
        exit(1);          
    }

    p3 = fork();
    if(p3 < 0){
        printf("Errore creazione pipe");
        exit(1);    
    }
    if (p3 == 0){
        close(0);
        dup(p2p3[0]);
        close(p2p3[0]);
        close(p2p3[1]);

        close(1);
        dup(p3p0[1]);
        close(p3p0[1]);
        close(p3p0[1]);

        execl("/bin/tail", "tail", "-n", "+2", NULL);
        return -1;
    }

    close(p2p3[0]);
    close(p2p3[1]);
    close(p3p0[1]);

    return 0;

}