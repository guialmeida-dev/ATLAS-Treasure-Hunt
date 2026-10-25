#ifndef PLAYER_H
#define PLAYER_H
#include "difficulty.h"

//Limite do nome do jogador, definindo um limite maximo que o vetor pode suportar (50)
#define MAX_NAME 50

/*Estrutura que representa um game guardado após save. Informações necessárias:
Nome;
Pontos;
Tesouros encontrados;
Locais ja Explorados;
Dificuldade;
*/

typedef struct Expedition Expedition;

//Cadrasto do Jogado:

typedef struct Player{
    char [MAX_NAME]; // Nome do jogador
    int score; // Quantidade de pontos feitos no jogo
    difficulty level; //Nivel de dificuldade selecionada pelo jogador
    Expedition *expedition;
    struct Palyer *next;
} Player;

// Aplicando função que pede o nome do jogador:

int infoPlayer(Player *player, difficulty level);

#endif
