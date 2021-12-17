#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define MAX 10

void GeneraNumeri(int numeri[MAX]){
    srand(time(0));
    for(int i = 0; i < MAX; i++){
        numeri[i] = 1 +rand()%100;
        printf("%i\n", numeri[i]);
    }
}

int main(){
    int Canale1[2];
    int Canale2[2];
    int somma = 0;
    int numeri[MAX];
    pipe(Canale1);
    pipe(Canale2);
    int pid = fork();
    if (pid > 0){
        close(Canale1[0]);
        close(Canale2[1]);
        GeneraNumeri(numeri);
        write(Canale1[1], &numeri, sizeof(numeri));
        read(Canale2[0], &somma, sizeof(int));
        printf("La somma e': %d\n", somma);
        return 0;
    }
    else{
        close(Canale1[1]);
        close(Canale2[0]);
        read(Canale1[0], &numeri, sizeof(numeri));
        for (int i = 0; i < MAX; i++)
        {
            somma = somma + numeri[i];
        }
        write(Canale2[1], &somma, sizeof(int));
        exit(0);
    }
}
