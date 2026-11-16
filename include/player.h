#ifndef PLAYER_H
#define PLAYER_H
#include "difficulty.h"
#define MAX_NAME 51

typedef struct Player {
    char name[MAX_NAME];
    int score;
    difficulty level;
} Player;

int infoPlayer(Player *player, difficulty level);

#endif
