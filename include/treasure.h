#ifndef TREASURE_H
#define TREASURE_H

#include "difficulty.h"

#define TREASURE_MAX_SIZE 10

// Um tesouro guardado na coleção dinâmica (separada do tabuleiro)
typedef struct {
    int row;
    int column;
    int found;      // 0 = oculto, 1 = encontrado
} Treasure;

// Estado completo de uma partida
typedef struct {
    int rows;
    int columns;
    int maxAttempts;
    int attemptsLeft;
    char board[TREASURE_MAX_SIZE][TREASURE_MAX_SIZE]; // Somente o que o jogador já sabe
    Treasure *treasures;                              // Coleção dinâmica de tesouros
    int treasureCount;
    int treasureCapacity;
    int foundCount;
} TreasureGame;

typedef enum {
    INVESTIGATE_INVALID,    // Coordenada fora do tabuleiro
    INVESTIGATE_REPEATED,   // Já investigada (não consome tentativa)
    INVESTIGATE_EMPTY,      // Nada encontrado
    INVESTIGATE_TREASURE    // Tesouro encontrado
} InvestigateResult;

typedef enum {
    TREASURE_RUNNING,
    TREASURE_WON,
    TREASURE_LOST
} TreasureState;

// Funções do sistema de Caça ao Tesouro

int startTreasureGame(TreasureGame *game, gameConfig config); // Gera o tabuleiro e sorteia os tesouros. Retorna 1 se deu certo
void freeTreasureGame(TreasureGame *game);                    // Libera a memória da coleção de tesouros
InvestigateResult investigateCoordinate(TreasureGame *game, int row, int column); // Investiga uma coordenada (índices a partir de 0)
TreasureState getTreasureState(const TreasureGame *game);     // Informa se a partida está em andamento, vencida ou perdida
void showKnownBoard(const TreasureGame *game);                // Imprime o estado conhecido pelo jogador

int saveTreasureGame(const TreasureGame *game, const char *fileName); // Salva a partida em arquivo
int loadTreasureGame(TreasureGame *game, const char *fileName);       // Carrega e valida uma partida salva

void registerRanking(const char *name, const TreasureGame *game, int won); // Registra um resultado no ranking
void showRanking();                                                        // Imprime o ranking

void runTreasureHunt(gameConfig config); // Joga uma nova partida completa no terminal
void resumeTreasureHunt();               // Retoma a partida salva, se existir

#endif
