#include <stdio.h> 
#include "../include/menu.h"

void runMainMenu(){
    int option, result;

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
        
        if (result != 1){
            int ch;
            printf("ERRO: Apenas Numeros sao Aceitos. Digite um numero valido. \n");
            
           while ((ch = getchar()) != '\n' && ch != EOF) {
}

            if (ch == EOF){
                break;
            }

            option = -1; 
            continue;
        }

          switch (option){
            case 1:
            printf("Dificuldade em desenvolvimento\n");
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