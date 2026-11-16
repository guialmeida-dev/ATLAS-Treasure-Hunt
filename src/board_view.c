// Interface original de Guilherme de Almeida, ligada ao estado da partida.
#include <stdio.h>
#include "../include/board_view.h"

// Mostra somente as casas que o jogador já conhece.
void showBoard(const TreasureGame *game) {
    GameStatus status;
    if (!getGameStatus(game, &status)) {
        printf("Tabuleiro indisponivel.\n");
        return;
    }

    printf("\n     ");
    for (int column = 0; column < status.columns; column++) {
        printf(" %c ", 'A' + column);
    }
    printf("\n    +");
    for (int column = 0; column < status.columns; column++) {
        printf("---");
    }
    printf("+\n");

    for (int row = 0; row < status.rows; row++) {
        printf("%2d  |", row + 1);
        for (int column = 0; column < status.columns; column++) {
            char cell = getKnownCell(game, row, column);
            if (cell == '.') {
                cell = '?';
            } else if (cell == 'o') {
                cell = 'X';
            }
            printf(" %c ", cell);
        }
        printf("|\n");
    }
    printf("    +");
    for (int column = 0; column < status.columns; column++) {
        printf("---");
    }
    printf("+\n? oculto | X vazio | $ tesouro | ! enigma pendente\n");
}

void showExpeditionStatus(const TreasureGame *game) {
    GameStatus status;
    if (!getGameStatus(game, &status)) {
        return;
    }
    printf("EXPLORADOR: %s | %s | Mapa: %dx%d\n", status.player.name,
           difficultyName(status.player.level), status.rows, status.columns);
    printf("TESOUROS: %d/%d | TENTATIVAS: %d/%d | PONTOS: %d\n",
           status.foundCount, status.treasureCount, status.attemptsLeft,
           status.maxAttempts, status.player.score);
    printf("RADAR: %d/%d | BUSSOLA: %d\n",
           RADAR_LIMIT - status.radarUsed, RADAR_LIMIT, status.compassesLeft);
}