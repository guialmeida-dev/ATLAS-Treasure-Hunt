#ifndef DIFFICULTY_H
#define DIFFICULTY_H

//Tipos de Dificuldade
typedef enum {
    INVALID = -1,
    BACK = 0,
    EASY = 1,
    NORMAL = 2,
    HARD = 3
} difficulty;

// Configurações de cada Dificuldade
typedef struct {
    int rows;
    int columns;
    int treasures; // Tesouros 
    int attempts; // Tentativas
    int compasses;
} gameConfig;

// Funções do sistema de Dificuldade

difficulty showDifficulty(); //Imprime o menu de Dificuldades no Terminal
difficulty difficultyLevel(char data[]); //Responsavel por receber e validar a entrada
gameConfig getGameConf(difficulty level); //Recebe a dificuldade e retorna e estrutura contendo as configurações

#endif