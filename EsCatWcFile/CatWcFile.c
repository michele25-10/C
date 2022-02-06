#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int main(){
    int p1p2[2];
    int p2p0[2];
    int pid1;
    int pid2;
    pipe(p1p2);
    pipe(p2p0);
    pid1 = fork();
    if (pid1 == 0){
        close(1);
        dup(p1p2[1]);
        close(p1p2[1]);
        close(p1p2[0]);
        execlp("/bin/cat", "cat", "File.txt", NULL);
    }else{
        pid2 = fork();
        if (pid2 == 0){
            close(0);
            dup(p1p2[0]);
            close(1);
            dup(p2p0[1]);
            close(p1p2[0]);
            close(p1p2[1]);
            close(p2p0[0]);
            close(p2p0[1]);

            execlp("/bin/wc", "wc", NULL);
        }
        else{
            int stream;
            stream = open("Risultato.txt", O_WRONLY);       //apriamo in scrittura
            close(0);
            dup(p2p0[0]);
            close(p2p0[0]);
            close(1);
            dup(stream);      //duplico il file descriptor

            close(p2p0[1]);

            close(p1p2[1]);
            close(p1p2[0]);

            execlp("/bin/tee", "tee", NULL);        //prende l'input e lo riscrive


            return 0;
        }
    }
}