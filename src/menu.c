#include <stdio.h>
#include "../include/menu.h"
#include "../include/difficulty.h"
#include "../include/player.h"
#include "../include/manual.h"
#include "../include/treasure.h"
#include "../include/input.h"
#include "../include/screen.h"

// Encaminha cada opção para o módulo correspondente.
void runMainMenu(void) {
    char text[128];
    int option, result;
    while (1) {
        clearScreen();
        printf("===========================================\n                 A T L A S\n       A MALDICAO DO TESOURO PERDIDO\n===========================================\n");
        printf("1 - Nova Expedicao\n2 - Continuar Expedicao\n3 - Manual do Explorador\n4 - Ranking dos Exploradores\n0 - Sair\n");
        result = readInput("Escolha uma opcao: ", text, sizeof text);
        if (result == 0 || result == -2) {
            break;
        }
        if (result != 1 || !parseInteger(text, 0, 4, &option)) {
            printf("ERRO: opcao invalida. Use um numero de 0 a 4.\n");
            continue;
        }
        if (option == 0) {
            break;
        }
        switch (option) {
            case 1: {
                Player player = {0};
                difficulty level;
                if (!infoPlayer(&player, INVALID)) {
                    break;
                }
                do {
                    level = showDifficulty();
                    if (level == INVALID) {
                        printf("ERRO: dificuldade invalida.\n");
                    }
                } while (level == INVALID);
                if (level == BACK) {
                    break;
                }
                player.level = level;
                runTreasureHunt(getGameConf(level), &player);
                break;
            }
            case 2:
                resumeTreasureHunt();
                break;
            case 3:
                menuManual();
                break;
            case 4:
                showRanking();
                break;
        }
    }
    printf("Encerrando o programa. Ate a proxima expedicao!\n");
}