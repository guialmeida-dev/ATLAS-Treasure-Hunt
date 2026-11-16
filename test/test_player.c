#include <stdio.h>
#include "../include/player.h"

int main(void) {
    Player player = {0};
    if (!infoPlayer(&player, EASY)) return 1;
    printf("CADASTRO: %s | pontos %d | dificuldade %d\n", player.name, player.score, player.level);
    return 0;
}