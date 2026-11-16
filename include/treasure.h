#ifndef TREASURE_H
#define TREASURE_H
#include <stddef.h>
#include "difficulty.h"
#include "player.h"
#define TREASURE_MAX_SIZE 10
#define RADAR_LIMIT 3
// Os dados internos ficam em treasure.c; aqui estão as operações da partida.
typedef struct TreasureGame TreasureGame;

typedef struct {
    int rows, columns, maxAttempts, attemptsLeft, treasureCount, foundCount;
    int compassesLeft, radarUsed;
    Player player;
} GameStatus;

typedef enum {
    INVESTIGATE_INVALID, INVESTIGATE_REPEATED, INVESTIGATE_EMPTY,
    INVESTIGATE_TREASURE, INVESTIGATE_CHALLENGE, INVESTIGATE_ENDED
} InvestigateResult;

typedef enum { TREASURE_RUNNING, TREASURE_WON, TREASURE_LOST } TreasureState;
TreasureGame *startTreasureGame(gameConfig config, const Player *player);
void freeTreasureGame(TreasureGame *game);
int getGameStatus(const TreasureGame *game, GameStatus *status);
char getKnownCell(const TreasureGame *game, int row, int column);
InvestigateResult investigateCoordinate(TreasureGame *game, int row, int column);
TreasureState getTreasureState(const TreasureGame *game);
int parseCoordinate(const char *text, int *row, int *column);
const char *getEnigmaQuestion(const TreasureGame *game);
int resolveEnigma(TreasureGame *game, const char *answer);
// Conta os tesouros vizinhos, incluindo encontrados, sem cobrar pela repetição.
int useRadar(TreasureGame *game, int row, int column, int *count);
int useCompass(TreasureGame *game, int row, int column, char *direction, size_t size);
int saveTreasureGame(const TreasureGame *game, const char *fileName);
TreasureGame *loadTreasureGame(const char *fileName);
int registerRanking(const TreasureGame *game);
void showRanking(void);
void runTreasureHunt(gameConfig config, const Player *player);
void resumeTreasureHunt(void);

#endif