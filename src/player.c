#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/player.h"

int infoPlayer(Player *player, difficulty level){
    //condicional criada para impedir o acesso a uma estrutura inexistente
    if(player == NULL){
        printf("ERRO: Falha ao Ler nome\n");
        return 0;
    }

    while(1){ // Irá executar o laço até que o codigo retorne o valor 1 (Verdadeiro)
        printf("\n Nome do Explorado: ");

        if(fgets(player->name, MAX_NAME, stdin) == NULL){ // Condicinal caso nao tenha nada inserido
            printf("ERRO: Falha ao ler nome inserido\n");
            return 0;
        }

        char *newline = strchr(player->name, '\n');

        if(newline != NULL){
            *newline = '\0';
        }
        else{
            /*Verifica se o nome ultrapassou o limite de caracteres definidos na biblioteca
            Caso ultrapasse, irá descartar o restante e manter o que estiver no limite*/ 
            int ch = getchar();

            if(ch != '\n' && ch != EOF){
                while(ch != '\n' && ch != EOF){
                    ch = getchar();
                }
                printf("ERRO: O nome deve ter no maximo 50 caracteres");
                continue; // Irá continuar, pois deleta o restante e mantem apenas os 50 inseridos
            }
        }
        int hasCharacter = 0; // Criado para verificar se existe nome que contem apenas espaços

        for(int i = 0; player->name[i] != '\0'; i++){ // Percorre toda a string
            if(!isspace((unsigned char)player->name[i])){
                hasCharacter = 1;
                break; // Se sim, para o codigo e retorna ao inicio
            }
        }
        if (!hasCharacter){
            printf("ERRO: Nenhum caracter encontrado, o nome do jogador nao pode estar vazio\n");
            continue;
        }
        player->score = 0;
        player->level = level;
        player->expedition = NULL;
        player->next = NULL;

        return 1;
    }
}