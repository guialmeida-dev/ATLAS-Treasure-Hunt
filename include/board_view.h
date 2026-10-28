#ifndef BOARD_VIEW_H
#define BOARD_VIEW_H
#include "treasure.h"
#include "player.h"

void showBoard(const TreasureGame *game);
void showExpeditionStatus(const TreasureGame *game, const Player *player);

#endif