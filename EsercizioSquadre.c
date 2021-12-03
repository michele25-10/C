#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>

typedef struct{
    char nome[10];
    char cognome[10];
    char ruolo[14];
}calciatore;

int ContaRighe(FILE *fp, char *nomefile){
    fp = fopen(nomefile, "r");
    int righe = 1;
    char c;
    while (!feof(fp))
    {
        c = fgetc(fp);
        if(c == '\n'){
            righe++;
        }
    }
    fclose(fp);
    return righe;
}

void GiocatoriSquadra(calciatore calciatori[], FILE *fp, int indice, char *nomefile){
    fp = fopen(nomefile, "r");
    int e = 0; 
    while (fscanf(fp, "%s %s %s", calciatori[e].nome, calciatori[e].cognome, calciatori[e].ruolo) != EOF)
    {
        e++;
    }
    fclose(fp);
}

void Output(calciatore calciatori[], int indice){
    for(int i = 0; i < indice; i++){
        if(strcmp(calciatori[i].ruolo, "Attaccante") == 0){
            printf("%s %s %s\n", calciatori[i].nome, calciatori[i].cognome, calciatori[i].ruolo);
        }
    }
}

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("Non ci sono abbastanza campi dati\n");
        exit(0);
    }
    FILE *stream;
    if (fopen(argv[1], "r") == NULL){
        printf("Errore\n");
        exit(1);
    }
    int pid;
    int status;
    int indice = ContaRighe(stream, argv[1]);
    calciatore calciatori[indice];
    GiocatoriSquadra(calciatori, stream, indice, argv[1]);
    pid = fork();
    if (pid == 0){
        Output(calciatori, indice);
        exit(6);
    }
    else{
        wait(&status);
        return 0;
    }
}