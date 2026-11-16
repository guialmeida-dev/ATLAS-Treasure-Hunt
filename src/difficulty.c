#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../include/difficulty.h"
#include "../include/input.h"

// Mostra as dificuldades e lê uma escolha completa.
difficulty showDifficulty(void) {
    char data[100];
    int result;

    printf("\n========== DIFFICULTY ==========\n");
    printf("[1] Easy\n[2] Normal\n[3] Hard\n[0] Back\n");
    printf("===============================\n");
    result = readInput("Selecione a dificuldade desejada: ", data, sizeof data);

    if (result == 0 || result == -2) {
        return BACK;
    }
    if (result != 1) {
        return INVALID;
    }
    trimText(data);
    return difficultyLevel(data);
}

// Aceita o número ou o nome da dificuldade, sem diferenciar maiúsculas.
difficulty difficultyLevel(char data[]) {
    data[strcspn(data, "\n")] = '\0';
    for (int i = 0; data[i] != '\0'; i++) {
        data[i] = (char)tolower((unsigned char)data[i]);
    }

    if (strcmp(data, "easy") == 0 || strcmp(data, "1") == 0) {
        return EASY;
    } else if (strcmp(data, "normal") == 0 || strcmp(data, "2") == 0) {
        return NORMAL;
    } else if (strcmp(data, "hard") == 0 || strcmp(data, "3") == 0) {
        return HARD;
    } else if (strcmp(data, "back") == 0 || strcmp(data, "0") == 0) {
        return BACK;
    }
    return INVALID;
}

// Mantém os parâmetros escolhidos pelo grupo para cada dificuldade.
gameConfig getGameConf(difficulty level) {
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
            printf("ERRO: dificuldade invalida.\n");
            break;
    }
    return config;
}

const char *difficultyName(difficulty level) {
    switch (level) {
        case EASY:
            return "Easy";
        case NORMAL:
            return "Normal";
        case HARD:
            return "Hard";
        default:
            return "Desconhecida";
    }
}