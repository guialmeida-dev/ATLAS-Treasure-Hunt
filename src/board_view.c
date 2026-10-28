#include <stdio.h>
#include "../include/board_view.h"

void showBoard(const TreasureGame *game) {

    if (game == NULL) {
        printf("ERRO: Tabuleiro indisponivel.\n");
        return;
    }
    // Cabecalho das colunas
  printf("\n     ");

    for (int col = 0; col < game->columns; col++) {
    printf("  %c", 'A' + col);
    }
    printf("\n");
    // Borda superior
    printf("    +");

    for (int col = 0; col < game->columns; col++) {
    printf("---");
    }
    printf("--+\n");
    // Linhas e colunas
   for (int row = 0; row < game->rows; row++) {
        printf("%2d  |", row + 1);

    for (int col = 0; col < game->columns; col++) {

    char cell = game->board[row][col];

    if (cell == '.') {
        cell = '?';
    }
    else if (cell == 'o') {
        cell = 'X';
    }
    printf(" %c ", cell);
}
printf("|\n");
}
    // Borda inferior
    printf("    +");

    for (int col = 0; col < game->columns; col++) {
        printf("---");
    }
    printf("--+\n");
}

void showExpeditionStatus(const TreasureGame *game, const Player *player){
        if (game == NULL || player == NULL){
            printf("ERRO: Informacoes da expedicao indisponiveis\n");
            return;
        }

        printf("\n----------------------------------------\n");

        printf(" EXPLORADOR: %s\n", player->name);

        const char *levelName = "DESCONHECIDA";

        switch (player -> level){
            case EASY:
            levelName = "EASY";
            break;

            case NORMAL:
            levelName = "NORMAL";
            break;

            case HARD:
            levelName = "HARD";
            break;
            
            default:
            break;
        }

        printf(" DIFICULDADE: %s\n", levelName);

        printf(" TESOUROS: %d/%d\n", game->foundCount, game->treasureCount);

        printf(" TENTATIVAS: %d/%d\n", game->attemptsLeft, game->maxAttempts);

        printf("\n----------------------------------------\n");
}

/*Falta a integração entre o board_view.c e o treasure.c, para substituir 
a exibição antiga pelo novo tabuleiro durante uma partida real.*/ 