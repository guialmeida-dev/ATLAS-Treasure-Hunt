// Manual original de Guilherme Arthur, com repetição do menu e leitura no terminal.
#include <stdio.h>
#include "../include/manual.h"
#include "../include/input.h"
#include "../include/storage.h"
static const char manualText[] =
"ATLAS - A MALDICAO DO TESOURO PERDIDO\n"
"J4 - Caca ao Tesouro | FACAPE | Programacao Avancada 2026.2\n\n"
"1. Nova Expedicao: informe nome, escolha Easy, Normal ou Hard.\n"
"Easy: 6x6, 3 tesouros, 30 tentativas, 3 bussolas.\n"
"Normal: 8x8, 5 tesouros, 45 tentativas, 2 bussolas.\n"
"Hard: 10x10, 7 tesouros, 60 tentativas, 1 bussola.\n\n"
"2. Coordenadas: letras nas COLUNAS, numeros nas LINHAS. B3 = coluna B, linha 3.\n"
"? oculto | X explorado vazio | $ tesouro encontrado.\n"
"Cada coordenada nova valida gasta uma tentativa, com ou sem tesouro.\n"
"Coordenadas invalidas ou repetidas nao gastam tentativas.\n"
"Encontre todos os tesouros ate a ultima tentativa para vencer.\n\n"
"3. Comandos durante a expedicao:\n"
"B3          investigar uma coordenada (tambem aceita b3).\n"
"radar B3    consultar as 8 casas vizinhas de uma casa vazia ja investigada.\n"
"            Conta TODOS os tesouros, incluindo os ja encontrados.\n"
"            Limite: 3 casas diferentes por partida; repetir nao gasta carga.\n"
"            A leitura e atualizada se um tesouro for realocado no Hard.\n"
"bussola B3  indicar direcao do tesouro oculto mais proximo de B3.\n"
"            Distancia Manhattan; empate por linha e depois coluna.\n"
"            Pode usar qualquer coordenada valida; consome uma bussola.\n"
"            Cargas por dificuldade: 3 / 2 / 1.\n"
"salvar      gravar em partida.sav; outro save exige confirmacao.\n"
"sair        retornar ao menu com opcao de salvar.\n"
"ajuda       exibir comandos.\n\n"
"4. Hard: encontrar um tesouro apresenta um enigma aritmetico.\n"
"Resposta correta conquista o tesouro. Resposta incorreta marca a casa vazia\n"
"e realoca o tesouro para uma casa oculta livre, sem conceder pontos.\n"
"A tentativa e gasta uma unica vez. EOF durante o enigma conta como erro.\n\n"
"5. Pontos: 100 por tesouro; vitoria soma 10 por tentativa restante.\n"
"Radar e bussola nao descontam pontos. Ranking separado por dificuldade,\n"
"mantendo o melhor resultado por nome exato (diferencia maiusculas).\n"
"Empate: vitoria, menos tentativas, nome; mesma partida nao e registrada duas vezes.\n\n"
"6. Continuar Expedicao restaura nome, mapa, recursos e progresso.\n"
"Ha um slot de save. Encerrar outra partida nao apaga esse slot.\n"
"O formato antigo numerico nao e carregado; preserve-o antes de substitui-lo.\n"
"EOF sai sem salvar automaticamente. Salve antes de encerrar a entrada.\n"
"Em falha de arquivo, a mensagem informa o erro e o arquivo anterior e preservado.\n"
"Arquivos .tmp/.bak apos interrupcao exigem recuperacao descrita no README.\n\n"
"Equipe: Guilherme de Almeida, Guilherme Arthur, Marcio e Enzo.\n";
void criarManual(void) {
    char temporary[128];
    FILE *file = beginStoredWrite("manual.txt", temporary, sizeof temporary);
    if (!file) {
        printf("ERRO: nao foi possivel criar/atualizar manual.txt.\n");
        return;
    }
    fputs(manualText, file);
    if (finishStoredWrite(file, "manual.txt", temporary)) {
        printf("Manual atualizado em manual.txt.\n");
    } else {
        printf("ERRO ao gravar manual.txt.\n");
    }
}
void abrirManual(void) {
    printf("%s\n", manualText);
}
void menuManual(void) {
    char text[64];
    int option, result;
    while (1) {
        printf("\nMANUAL DO EXPLORADOR\n1 - Criar/atualizar manual.txt\n2 - Ler manual no terminal\n0 - Voltar\n");
        result = readInput("Opcao do manual: ", text, sizeof text);
        if (result == 0 || result == -2) {
            return;
        }
        if (result != 1 || !parseInteger(text, 0, 2, &option)) {
            printf("ERRO: opcao invalida.\n");
            continue;
        }
        if (option == 0) {
            return;
        }
        if (option == 1) {
            criarManual();
        }
        else {
            abrirManual();
        }
    }
}