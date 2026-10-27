#include <stdio.h>
#include <stdlib.h>
#include "../include/manual.h"

void menuManual(){
    int n;
    printf("==========ESCOLHA UMA OPCAO==========\n\n");
    printf("1 - Criar Manual\n");
    printf("2 - Abrir Manual\n");
    printf("0 - Voltar\n");
    printf("\n=====================================\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &n);
    #ifdef _WIN32 //comando pra limpar o termial tanto no Windows quanto no Linux
            system("cls");
        #else
            system("clear");
        #endif

    if(n<0 || n>2){
        printf("OPCAO INVALIDA\n\n");
        menuManual();
    }
    switch(n){
        case 1:
        criarManual();
        menuManual();
        break;

        case 2:
        abrirManual();
        menuManual();
        break;
    }
}
void criarManual(){
    FILE *fp = fopen("manual.txt", "r");

    if(fp==NULL){  //Cria o manual apenas caso ele não exista.
        fp = fopen("manual.txt", "w");

        fprintf(fp, "MANUAL DO JOGADOR\n");
        fprintf(fp, "\nCaça ao Tesouro\n");
        fprintf(fp, "\n1. Bem-vindo ao Caça ao Tesouro!\n");
        fprintf(fp, "Prepare-se para testar sua sorte e estratégia! No Caça ao Tesouro, seu objetivo é \n");
        fprintf(fp, "encontrar todos os tesouros escondidos em um tabuleiro antes que suas tentativas acabem.\n\n");
        fprintf(fp, "Explore as posições, evite repetir jogadas e tente encontrar todos os tesouros com o\n");
        fprintf(fp, "menor número possível de tentativas!\n\n");
        fprintf(fp, "2. Como começar a jogar\n\n");
        fprintf(fp, "Ao abrir o jogo, você poderá acessar o menu principal e escolher uma das opções disponíveis:\n\n");
        fprintf(fp, "Nova partida: começa um novo jogo, com um tabuleiro e tesouros escondidos.\n");
        fprintf(fp, "Continuar partida: permite retomar uma partida que foi salva anteriormente.\n");
        fprintf(fp, "Ranking: mostra os resultados registrados dos jogadores.\n");
        fprintf(fp, "Sair: encerra o jogo.\n\n");
        fprintf(fp, "Escolha a opção desejada e siga as instruções apresentadas na tela.\n\n");
        fprintf(fp, "3. Conhecendo o tabuleiro\n\n");
        fprintf(fp, "O tabuleiro é formado por linhas identificadas por letras e colunas identificadas por\n");
        fprintf(fp, "números. Para escolher uma posição, você deverá combinar uma letra com um número.\n\n");
        fprintf(fp, "Veja um exemplo de tabuleiro:\n\n");
        fprintf(fp, "    1 2 3 4 5 6\n");
        fprintf(fp, "A   ? ? ? ? ? ?\n");
        fprintf(fp, "B   ? X ? ? ? ?\n");
        fprintf(fp, "C   ? ? T ? ? ?\n");
        fprintf(fp, "D   ? ? ? ? ? ?\n");
        fprintf(fp, "E   ? ? ? X ? ?\n");
        fprintf(fp, "F   ? ? ? ? ? ?\n\n");
        fprintf(fp, "Cada símbolo representa uma situação:\n\n");
        fprintf(fp, "? — Você ainda não explorou essa posição. Pode haver um tesouro escondido nela!\n");
        fprintf(fp, "X — Você já explorou essa posição, mas não encontrou nenhum tesouro.\n");
        fprintf(fp, "T — Você encontrou um tesouro nessa posição.\n\n");
        fprintf(fp, "Os tesouros permanecem escondidos até que você os encontre. Portanto, será\n");
        fprintf(fp, "necessário escolher suas coordenadas com atenção.\n\n");
        fprintf(fp, "4. Como encontrar os tesouros\n\n");
        fprintf(fp, "Durante sua vez, informe as coordenadas da posição que deseja explorar.\n\n");
        fprintf(fp, "Por exemplo, se quiser explorar a quarta coluna da linha D, digite 'D4'\n\n");
        fprintf(fp, "O jogo verificará a posição e informará o resultado da sua escolha.\n\n");
        fprintf(fp, "Se encontrar um tesouro: a posição será identificada com o símbolo T, e o total de\n");
        fprintf(fp, "tesouros encontrados será atualizado.\n\n");
        fprintf(fp, "Se não encontrar um tesouro: a posição será identificada com o símbolo X,\n");
        fprintf(fp, "e você perderá uma tentativa.\n\n");
        fprintf(fp, "Continue escolhendo novas coordenadas até encontrar todos os tesouros ou acabar suas tentativas.\n\n");
        fprintf(fp, "5. Atenção às posições repetidas!\n\n");
        fprintf(fp, "Você não perde tentativas ao selecionar uma posição que já explorou.\n\n");
        fprintf(fp, "Se escolher novamente uma coordenada marcada com X ou T, o jogo informará que\n");
        fprintf(fp, "aquela posição já foi explorada. O tabuleiro continuará mostrando o resultado\n");
        fprintf(fp, "anterior, e nenhuma tentativa será descontada.\n\n");
        fprintf(fp, "Por isso, observe sempre as posições que já foram verificadas antes de realizar uma nova jogada.\n\n");
        fprintf(fp, "6. Tentativas restantes\n\n");
        fprintf(fp, "Você possui uma quantidade limitada de tentativas para encontrar todos os tesouros.\n\n");
        fprintf(fp, "A cada jogada válida, o número de tentativas restantes será atualizado. Por isso,\n");
        fprintf(fp, "pense bem antes de escolher uma posição e evite desperdiçar oportunidades.\n\n");
        fprintf(fp, "Na tela do jogo, você poderá acompanhar:\n\n");
        fprintf(fp, "Tesouros encontrados: quantos tesouros você já localizou.\n");
        fprintf(fp, "Total de tesouros: quantos tesouros existem na partida.\n");
        fprintf(fp, "Tentativas restantes: quantas jogadas válidas você ainda pode realizar.\n");
        fprintf(fp, "7. Como vencer ou perder\n\n");
        fprintf(fp, "Existem dois resultados possíveis:\n\n");
        fprintf(fp, "Vitória!\n\n");
        fprintf(fp, "Você vence quando encontra todos os tesouros antes que suas tentativas acabem. \n");
        fprintf(fp, "Ao vencer, seu resultado poderá ser registrado no ranking.\n\n");
        fprintf(fp, "Derrota!\n\n");
        fprintf(fp, "Você perde quando suas tentativas acabam e ainda existem tesouros escondidos no tabuleiro.\n\n");
        fprintf(fp, "Não desanime! Inicie uma nova partida e tente melhorar seu resultado.\n\n");
        fprintf(fp, "8. Salvando e continuando sua partida\n\n");
        fprintf(fp, "Se precisar parar de jogar, utilize a opção de salvar partida, caso esteja disponível no menu.\n\n");
        fprintf(fp, "Ao salvar, seu progresso será armazenado para que você possa continuar depois.\n\n");
        fprintf(fp, "Quando voltar ao jogo, selecione a opção de continuar partida. O tabuleiro, \n");
        fprintf(fp, "as posições exploradas, os tesouros encontrados e as tentativas restantes serão recuperados.\n\n");
        fprintf(fp, "Assim, você poderá retomar a aventura sem precisar começar novamente.\n\n");
        fprintf(fp, "9. Consultando o ranking\n\n");
        fprintf(fp, "Quer saber como foi seu desempenho?\n\n");
        fprintf(fp, "Acesse a opção Ranking no menu principal para consultar os resultados registrados.\n\n");
        fprintf(fp, "O ranking permite acompanhar o desempenho dos jogadores e comparar os \n");
        fprintf(fp, "resultados das partidas. A classificação seguirá os critérios definidos pelo jogo.\n\n");
        fprintf(fp, "Tente vencer utilizando suas tentativas com inteligência para alcançar uma boa posição!\n\n");
        fprintf(fp, "10. Dicas para jogar melhor\n\n");
        fprintf(fp, "Observe o tabuleiro antes de escolher uma coordenada.\n");
        fprintf(fp, "Não repita posições que já foram exploradas.\n");
        fprintf(fp, "Acompanhe sempre o número de tentativas restantes.\n");
        fprintf(fp, "Confira as coordenadas antes de confirmá-las.\n");
        fprintf(fp, "Salve seu progresso quando quiser continuar em outro momento.\n");
        fprintf(fp, "Busque melhorar seu resultado a cada nova partida.\n\n");
        fprintf(fp, "Boa sorte!\n\n");
        fprintf(fp, "Agora que você conhece as regras, está pronto para começar sua aventura.\n\n");
        fprintf(fp, "Escolha suas coordenadas, encontre os tesouros escondidos e tente conquistar o \n");
        fprintf(fp, "melhor resultado no Caça ao Tesouro!");
        fclose(fp);

        printf("MANUAL CRIADO!\n\n");
    } else {
        fclose(fp);
        printf("O MANUAL JA EXISTE!\n\n");
    }
}
void abrirManual(){
    FILE *fp = fopen("manual.txt", "r");

    if(fp==NULL){
        printf("ERRO!! O MANUAL NAO EXISTE!\n\n");
        return;
    }
    fclose(fp);

    #ifdef _WIN32 //Comandos para abrir o editor de texto tanto no Windows quanto no Linux.
        system("notepad manual.txt");
    #else
        system("xdg-open manual.txt"); 
    #endif
}