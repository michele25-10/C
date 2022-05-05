#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char*argv[]){
    if(argc != 17){
        printf("Errore");
        exit(0);
    }

    int somma = 0;
    int numero = 0;
    int j = 1;

    for(j = 1; j < 17; j++){
        numero = atoi(argv[j]);
        if(j % 2 == 0){
            somma = somma + (numero * 2);
        }
        else{
            somma = somma + numero;
        }
    }

    if(somma%10 == 0){
        printf("Codice PAN valido\n");
        exit(1);
    }
    else{
        printf("Codice PAN NON valido\n");
    }

    return 0;
}