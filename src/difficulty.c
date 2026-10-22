#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/difficulty.h"

#define MAX 100

// Exibição e Leitura da Dificuldade
difficulty showDifficulty() {
    char data[MAX];

    // Limpar o terminal (implementaremos futuramente)

    // Exibindo as dificuldades
    printf("\n========== DIFFICULTY ==========\n");
    printf("\n[1] Easy\n");
    printf("\n[2] Normal\n");
    printf("\n[3] Hard\n");
    printf("\n[0] Back\n");
    printf("\n===============================\n");

    printf("\nAlert: Esta secao aceita numeros e caracteres.\n");
    printf("Selecione a dificuldade desejada: ");

    // Captura a entrada do jogador
    if (fgets(data, sizeof(data), stdin) == NULL) {
        printf("ERRO: Falha ao ler a entrada.\n");
        return INVALID;
    }

    // Envia a entrada para validacao e retorna o resultado
    return difficultyLevel(data);
}

difficulty difficultyLevel(char data[]){

    //Remoção da quebra de linha criada pelo Enter
    data[strcspn(data, "\n")] = '\0';

    //Converção dos caracteres digitados em Minusculos 
    for (int i = 0; data[i] != '\0'; i++){
        data[i] = (char)tolower((unsigned char)data[i]);
    }
    
    //Identificação da dificuldade utilizando ternarios
    difficulty level =

        (strcmp(data, "easy") == 0 || strcmp(data, "1") == 0) ? EASY :
        (strcmp(data, "normal") == 0 || strcmp(data, "2") == 0) ? NORMAL :
        (strcmp(data, "hard") == 0 || strcmp(data, "3") == 0) ? HARD :
        (strcmp(data, "back") == 0 || strcmp(data, "0") == 0) ? BACK : INVALID;

     return level;
}

//Implementação das configurações de mapa de acordo com a dificuldade:

gameConfig getGameConf(difficulty level){

    gameConfig config = {0};
    
    switch (level) {

        case EASY:
            config.rows = 6;
            config.columns = 6;
            config.treasures = 3;
            config.attempts = 30;
            config.compasses = 3;
            break;

        case NORMAL:
            config.rows = 8;
            config.columns = 8;
            config.treasures = 5;
            config.attempts = 45;
            config.compasses = 2;
            break;

        case HARD:
            config.rows = 10;
            config.columns = 10;
            config.treasures = 7;
            config.attempts = 60;
            config.compasses = 1;
            break;

        default:
            printf("ERRO! ERRO!");
            config.rows = 0;
            config.columns = 0;
            config.treasures = 0;
            config.attempts = 0;
            config.compasses = 0;
            break;
    }
    return config;
}
