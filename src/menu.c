#include <stdio.h> 
#include "../include/menu.h"
#include "../include/difficulty.h"

void runMainMenu(){
    int option, result;
    difficulty selectedLevel;
    gameConfig config;

    do{
        printf("===========================================\n");
        printf("                 A T L A S\n");
        printf("       A MALDICAO DO TESOURO PERDIDO\n");
        printf("===========================================\n");
        printf("\n");
        printf("         1 - Nova Expedicao\n");
        printf("         2 - Continuar Expedicao\n");
        printf("         3 - Manual do Explorador\n");
        printf("         4 - Ranking dos Exploradores\n");
        printf("         0 - Sair\n");
        printf("\n");
        printf("===========================================\n");
        printf("Escolha uma opcao: ");
     result = scanf("%d", &option);

if (result != 1) {
    int ch;

    printf("ERRO: Apenas numeros sao aceitos. Digite um numero valido.\n");

    // Limpa os caracteres invalidos do buffer
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }

    // Encerra se a entrada chegou ao fim
    if (ch == EOF) {
        break;
    }

    option = -1;
    continue;
}

// Limpa o restante da linha apos uma leitura numerica
int ch;

while ((ch = getchar()) != '\n' && ch != EOF) {
}
          switch (option){
            case 1:
            printf("Iniciando nova expedicao. . .\n");
            selectedLevel = showDifficulty();
            
            if(selectedLevel == BACK){
                printf("Retornando ao Menu Principal...\n");
            }
            else if(selectedLevel == INVALID){
                printf("ERRO: Dificuldade Invalida\n");
            }
            else {
                config = getGameConf(selectedLevel);

                printf("\n========== DEBUG GAME CONFIG ==========\n");
                printf("Linhas: %d\n", config.rows);
                printf("Colunas: %d\n", config.columns);
                printf("Tesouros: %d\n", config.treasures);
                printf("Tentativas: %d\n", config.attempts);
                printf("Bussolas: %d\n", config.compasses);
                printf("=======================================\n");
}
            break;

            case 2: 
            printf("Inciacao de jogo Salvo em desenvolvimento\n");
            break;

            case 3:
            printf("Execucao de Manual do Jogo em Desenvolvimento\n");
            break;

            case 4:
            printf("Sistema de Ranking em desenvolvimento\n");
            break;

            case 0:
            printf("Encerrando o Programa...\n");
            break;

            default:
            printf("ERRO: Opcao invalida.\n");
            break;
        }
    } while (option != 0);

}