#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<ctype.h>

#define MAX 15
#define INDICE 4

typedef struct{
    char nome[MAX];
    char cognome[MAX];
}Persona_T;


void Maiuscolo(char *str){
    for (int i = 0; i < strlen(str); i++){
        str[i] = toupper(str[i]);
    }
    printf("%s\n", str);
}

void LetturaFile(char *NomeFile, Persona_T identita[]){
    FILE *stream;
    if((stream = fopen(NomeFile, "r")) == NULL){
        exit(1);
    }

    int i = 0;
    while(!feof(stream)){
        fscanf(stream, "%s %s", identita[i].nome, identita[i].cognome);
        i++;
    }
    fclose(stream);
    
    for(int j = 0; j < INDICE; j++){
        Maiuscolo(identita[j].nome);
        Maiuscolo(identita[j].cognome);
    }
}

void Scrittura(Persona_T identita[]){
    FILE *fp;
    fp = fopen("Scrivo.txt", "w");
    for (int i = 0; i < INDICE; i++){
        fprintf(fp, "%s %s\n", identita[i].nome, identita[i].cognome);
    }
    fclose(fp);
}

int main(int argc, char *argv[]){
    if (argc != 3){
        exit(1);
    }
    int pid;
    int status;
    Persona_T identita[INDICE];
    pid = fork();
    if(pid == 0){
        Maiuscolo(argv[1]);
        LetturaFile(argv[2], identita);
        Scrittura(identita);
        exit(0);
    }
    else{
        wait(&status);
        return 0;
    }
}