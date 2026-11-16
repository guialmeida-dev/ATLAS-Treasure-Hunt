// Cadastro original da equipe, com validação do nome do explorador.
#include <stdio.h>
#include <string.h>
#include "../include/player.h"
#include "../include/input.h"

int infoPlayer(Player *player, difficulty level) {
    char text[256];
    int result;

    if (!player) {
        return 0;
    }
    while (1) {
        result = readInput("Nome do explorador (1 a 50 bytes): ", text, sizeof text);
        if (result == 0 || result == -2) {
            return 0;
        }
        if (result != 1) {
            printf("ERRO: nome muito longo ou invalido.\n");
            continue;
        }
        trimText(text);
        if (!validPlayerName(text)) {
            printf("ERRO: use um nome nao vazio com ate 50 bytes.\n");
            continue;
        }
        // O nome já foi validado e cabe no vetor do jogador.
        strcpy(player->name, text);
        player->score = 0;
        player->level = level;
        return 1;
    }
}