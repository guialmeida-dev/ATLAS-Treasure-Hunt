#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/board_view.h"
int main(void) {
    srand(42);
    for (int level = EASY; level <= HARD; level++) {
        Player player = {"Guilherme", 0, (difficulty)level};
        TreasureGame *game = startTreasureGame(getGameConf(player.level), &player);
        GameStatus s;
        assert(game && getGameStatus(game, &s));
        assert(s.rows == 4 + 2 * level && s.columns == s.rows);
        showBoard(game);
        showExpeditionStatus(game);
        freeTreasureGame(game);
    }
    printf("PASSOU: board_view nas tres dificuldades.\n");
    return 0;
}
