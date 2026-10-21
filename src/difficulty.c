#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/difficulty.h"
#define MAX 100

void showDifficulty(){
    printf("\n========== DIFFICULTY ==========\n");
    printf("\n[1] Easy\n");
    printf("\n[2] Normal\n");
    printf("\n[3] Hard\n");
    printf("\n[0] Back\n");
    printf("\n===============================\n");
}

difficulty selectDiff(){
    char data[MAX];

    showDifficulty();

    printf("Alert: Esta Secao aceita numero e caracteres.\n");
    printf("Selecione a dificuldade desejada: ");
    
    if (fgets(data, sizeof(data), stdin) == NULL){
        printf("ERRO: Falha ao Ler a entrada.\n");
        return;
    }
    printf ("Voce digitou: %s", data);
    data[strcspn(data, "\n")] =  '\0'; //Remove a quebra de linha criada pelo enter

    for (int i = 0; data[i] != '\0'; i++){
        data[i] = (char)tolower((unsigned char)data[i]); //Padronizar qualquer coisa que usuario digitar em minusculo.
    }
    
    difficulty level = 
        (strcmp(data, "easy") == 0 || strcmp(data, "1") == 0) ? EASY :
        (strcmp(data, "normal") == 0 || strcmp(data, "2") == 0) ? NORMAL :
        (strcmp(data, "hard") == 0 || strcmp(data, "3") == 0) ? HARD :
        (strcmp(data, "back") == 0 || strcmp(data, "0") == 0) ? BACK : INVALID;

        return level;
}
