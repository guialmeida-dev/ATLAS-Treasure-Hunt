// Executa os mesmos grupos principais da revisão anterior usando apenas C e GCC.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif
#include "../include/difficulty.h"

typedef struct {
    char id[64];
    char name[51];
    int values[10];
    int treasures[10][3];
    char board[10][11];
    char radar[10][11];
} SavedGame;

static char root[2048];
static char work[8192];
static char currentCase[8192];
static char coreFolder[8192];
static char executable[4096];
static char output[524288];
static FILE *report;
static int tests;
static int passed;
static int failed;
static int executions;
static const char *solutions[] = {"8", "12", "5", "9", "7", "6", "10"};

// As diferenças de sistema ficam apenas nestas funções dos testes.
static void changeDirectory(const char *path) {
#ifdef _WIN32
    assert(_chdir(path) == 0);
#else
    assert(chdir(path) == 0);
#endif
}
static void createDirectory(const char *path) {
    int result;
#ifdef _WIN32
    result = _mkdir(path);
#else
    result = mkdir(path, 0700);
#endif
    assert(result == 0 || errno == EEXIST);
}

static void joinPath(char *result, size_t size, const char *folder, const char *name) {
    assert(strlen(folder) + strlen(name) + 2 <= size);
    strcpy(result, folder);
    strcat(result, "/");
    strcat(result, name);
}

static int exists(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        return 0;
    }
    fclose(file);
    return 1;
}

static void copyFile(const char *source, const char *destination) {
    FILE *from = fopen(source, "rb");
    FILE *to = fopen(destination, "wb");
    char buffer[4096];
    size_t count;
    assert(from && to);
    while ((count = fread(buffer, 1, sizeof buffer, from)) > 0) {
        assert(fwrite(buffer, 1, count, to) == count);
    }
    assert(!ferror(from));
    assert(fclose(from) == 0);
    assert(fclose(to) == 0);
}

static int filesEqual(const char *first, const char *second) {
    FILE *a = fopen(first, "rb");
    FILE *b = fopen(second, "rb");
    int same = 1;
    int ch;
    assert(a && b);
    while ((ch = fgetc(a)) != EOF) {
        if (fgetc(b) != ch) {
            same = 0;
            break;
        }
    }
    if (same && fgetc(b) != EOF) {
        same = 0;
    }
    if (ferror(a) || ferror(b)) {
        same = 0;
    }
    fclose(a);
    fclose(b);
    return same;
}

static void writeText(const char *path, const char *text) {
    FILE *file = fopen(path, "wb");
    size_t length = strlen(text);
    assert(file && fwrite(text, 1, length, file) == length);
    assert(fclose(file) == 0);
}

static void readText(const char *path, char *text, size_t size) {
    FILE *file = fopen(path, "rb");
    size_t count;
    assert(file);
    count = fread(text, 1, size - 1, file);
    assert(!ferror(file) && fgetc(file) == EOF);
    text[count] = '\0';
    fclose(file);
}

static int occurrences(const char *text, const char *word) {
    int count = 0;
    size_t length = strlen(word);
    while ((text = strstr(text, word)) != NULL) {
        count++;
        text += length;
    }
    return count;
}

static void check(int condition, const char *message) {
    if (!condition) {
        failed = 1;
        printf("  FALHOU: %s\n", message);
        fprintf(report, "  FALHOU: %s\n", message);
    }
}

static void beginTest(void) {
    char name[32];
    tests++;
    failed = 0;
    executions = 0;
    snprintf(name, sizeof name, "%02d", tests);
    joinPath(currentCase, sizeof currentCase, work, name);
    createDirectory(currentCase);
    changeDirectory(currentCase);
}

static void endTest(const char *name) {
    const char *result = "FALHOU";
    if (!failed) {
        result = "PASSOU";
        passed++;
    }
    printf("%02d %s: %s\n", tests, result, name);
    fprintf(report, "%02d %s: %s\n", tests, result, name);
    fflush(report);
}

// Guarda a entrada, a saída e os erros de cada execução real do programa.
static void runProgram(const char *program, const char *input, size_t length) {
    char command[16384];
    char inputName[64];
    char outputName[64];
    char errorName[64];
    char errorText[8192];
    FILE *file;
    int result;

    executions++;
    snprintf(inputName, sizeof inputName, "input-%d.txt", executions);
    snprintf(outputName, sizeof outputName, "output-%d.txt", executions);
    snprintf(errorName, sizeof errorName, "error-%d.txt", executions);
    file = fopen(inputName, "wb");
    assert(file && fwrite(input, 1, length, file) == length);
    assert(fclose(file) == 0);
    snprintf(command, sizeof command, "\"%s\" < %s > %s 2> %s", program, inputName, outputName, errorName);
    result = system(command);
    readText(outputName, output, sizeof output);
    readText(errorName, errorText, sizeof errorText);
    check(result == 0, "O executavel retornou erro.");
    check(errorText[0] == '\0', "O executavel escreveu erro inesperado.");
}

static void play(const char *input) {
    runProgram(executable, input, strlen(input));
}

static void coreFile(char *path, size_t size, const char *name) {
    joinPath(path, size, coreFolder, name);
}

// Os testes leem seus próprios saves para preparar situações sem depender da sorte.
static void readSave(const char *path, SavedGame *game) {
    char line[512];
    FILE *file = fopen(path, "r");
    assert(file && fgets(line, sizeof line, file));
    assert(fgets(game->id, sizeof game->id, file));
    assert(fgets(game->name, sizeof game->name, file));
    game->id[strcspn(game->id, "\r\n")] = '\0';
    game->name[strcspn(game->name, "\r\n")] = '\0';
    for (int i = 0; i < 10; i++) {
        assert(fscanf(file, "%d", &game->values[i]) == 1);
    }
    for (int i = 0; i < game->values[5]; i++) {
        assert(fscanf(file, "%d%d%d", &game->treasures[i][0], &game->treasures[i][1], &game->treasures[i][2]) == 3);
    }
    for (int row = 0; row < game->values[1]; row++) {
        assert(fscanf(file, "%10s", game->board[row]) == 1);
    }
    for (int row = 0; row < game->values[1]; row++) {
        assert(fscanf(file, "%10s", game->radar[row]) == 1);
    }
    fclose(file);
}

static void writeSave(const char *path, const SavedGame *game) {
    FILE *file = fopen(path, "w");
    assert(file);
    fprintf(file, "ATLAS_SAVE 2\n%s\n%s\n", game->id, game->name);
    for (int i = 0; i < 10; i++) {
        fprintf(file, "%d%c", game->values[i], i == 9 ? '\n' : ' ');
    }
    for (int i = 0; i < game->values[5]; i++) {
        fprintf(file, "%d %d %d\n", game->treasures[i][0], game->treasures[i][1], game->treasures[i][2]);
    }
    for (int row = 0; row < game->values[1]; row++) {
        fprintf(file, "%s\n", game->board[row]);
    }
    for (int row = 0; row < game->values[1]; row++) {
        fprintf(file, "%s\n", game->radar[row]);
    }
    fprintf(file, "END\n");
    assert(fclose(file) == 0);
}

static void initialSave(int level, SavedGame *game) {
    char name[64];
    char path[8192];
    snprintf(name, sizeof name, "fixture-%d.sav", level);
    coreFile(path, sizeof path, name);
    copyFile(path, "partida.sav");
    readSave("partida.sav", game);
}

static int hasTreasure(const SavedGame *game, int row, int column) {
    for (int i = 0; i < game->values[5]; i++) {
        if (game->treasures[i][0] == row && game->treasures[i][1] == column) {
            return 1;
        }
    }
    return 0;
}

static void coordinate(char *text, int row, int column) {
    snprintf(text, 16, "%c%d", 'A' + column, row + 1);
}

static void nearEnd(int level, int win, char *target) {
    SavedGame game;
    int found;
    int explored;
    initialSave(level, &game);
    found = win ? game.values[5] - 1 : 0;
    explored = found;
    for (int row = 0; row < game.values[1]; row++) {
        for (int column = 0; column < game.values[2]; column++) {
            game.board[row][column] = '.';
            game.radar[row][column] = '0';
        }
    }
    for (int i = 0; i < game.values[5]; i++) {
        game.treasures[i][2] = i < found;
        if (i < found) {
            game.board[game.treasures[i][0]][game.treasures[i][1]] = '$';
        }
    }
    target[0] = '\0';
    for (int row = 0; row < game.values[1]; row++) {
        for (int column = 0; column < game.values[2]; column++) {
            if (hasTreasure(&game, row, column)) {
                continue;
            }
            if (explored < game.values[3] - 1) {
                game.board[row][column] = 'o';
                explored++;
            } else if (!*target) {
                coordinate(target, row, column);
            }
        }
    }
    if (win) {
        int last = game.values[5] - 1;
        coordinate(target, game.treasures[last][0], game.treasures[last][1]);
    }
    game.values[4] = 1;
    game.values[6] = found;
    game.values[8] = 0;
    game.values[9] = found * 100;
    strcpy(game.name, "Ultima tentativa");
    writeSave("partida.sav", &game);
}

static int compileAll(void) {
    const char *common = "src/menu.c src/difficulty.c src/player.c src/manual.c src/treasure.c src/board_view.c src/screen.c src/input.c src/storage.c";
    const char *names[] = {"core", "alloc", "board", "player"};
    char command[4096];
    char logName[128];
    char log[32768];
    int ok = 1;
    changeDirectory(root);
    if (system("gcc -std=c11 -Wall -Wextra -Wpedantic src/*.c -o atlas.exe > build/c-compile-game.log 2>&1") != 0) {
        ok = 0;
    }
    readText("build/c-compile-game.log", log, sizeof log);
    if (*log) {
        printf("%s", log);
        ok = 0;
    }
    for (int i = 0; i < 4; i++) {
        const char *extra = "";
        if (i == 1) {
            extra = "-include test/alloc_hooks.h";
        }
        snprintf(command, sizeof command,
                 "gcc -std=c11 -Wall -Wextra -Wpedantic %s test/test_%s.c %s -o build/test_%s.exe > build/c-compile-%s.log 2>&1",
                 extra, names[i], common, names[i], names[i]);
        if (system(command) != 0) {
            ok = 0;
        }
        snprintf(logName, sizeof logName, "build/c-compile-%s.log", names[i]);
        readText(logName, log, sizeof log);
        if (*log) {
            printf("%s", log);
            ok = 0;
        }
    }
    changeDirectory(currentCase);
    return ok;
}

static void testMenu(void) {
    char input[8192];
    beginTest();
    strcpy(input, "1abc\n9999999999999999999999999999\nabc\n");
    size_t length = strlen(input);
    memset(input + length, 'x', 5000);
    // Completa a linha longa sem depender do conteúdo anterior do vetor.
    input[length + 5000] = '\0';
    strcat(input, "\n2\n4\n0\n");
    play(input);
    check(occurrences(output, "opcao invalida") == 4, "Menu deve rejeitar as quatro entradas invalidas.");
    check(strstr(output, "Nome do explorador") == NULL, "1abc nao pode iniciar cadastro.");
    check(strstr(output, "Nenhuma partida salva valida") != NULL, "Save ausente deve ser informado.");
    check(strstr(output, "Nenhum resultado") != NULL, "Ranking ausente deve ser informado.");
    endTest("Menu, entradas invalidas e primeira execucao");
}
static void testEof(void) {
    const char *inputs[] = {"", "1\n", "1\nNome\n", "3\n", "1\nNome\n3\n"};
    beginTest();
    for (int i = 0; i < 5; i++) {
        play(inputs[i]);
        check(strstr(output, "Encerrando") != NULL, "EOF precisa encerrar sem travar.");
    }
    endTest("EOF em menu, cadastro, dificuldade, manual e partida");
}
static void testNames(void) {
    char input[8192];
    char name[51];
    SavedGame game;
    size_t length;
    beginTest();
    memset(name, 'A', 50);
    name[50] = '\0';
    strcpy(input, "1\n\n   \n");
    length = strlen(input);
    memset(input + length, 'N', 5000);
    input[length + 5000] = '\0';
    strcat(input, "\n");
    strcat(input, name);
    strcat(input, "\nxxx\n1\nsalvar\nsair\nn\n0\n");
    play(input);
    check(occurrences(output, "ERRO:") >= 3, "Nomes vazios ou longos precisam ser rejeitados.");
    check(strstr(output, "dificuldade invalida") != NULL, "Dificuldade invalida precisa ser rejeitada.");
    readSave("partida.sav", &game);
    check(strcmp(game.name, name) == 0, "Nome de 50 bytes precisa ser preservado.");
    play("2\nsair\nn\n0\n");
    check(strstr(output, "Partida retomada!") != NULL && strstr(output, name) != NULL, "Nome limite precisa ser carregado.");
    endTest("Cadastro, nomes invalidos e persistencia do limite de 50 bytes");
}
static void testManual(void) {
    char input[8192];
    size_t length;
    beginTest();
    strcpy(input, "3\nabc\n999999999999999999999999\n");
    length = strlen(input);
    memset(input + length, 'q', 5000);
    input[length + 5000] = '\0';
    strcat(input, "\n2\n1\n1\n0\n0\n");
    play(input);
    check(occurrences(output, "ERRO: opcao invalida") == 3, "Submenu precisa rejeitar entradas invalidas.");
    check(strstr(output, "COLUNAS") != NULL, "Manual deve aparecer no terminal.");
    readText("manual.txt", input, sizeof input);
    check(strstr(input, "radar B3") != NULL && strstr(input, "bussola B3") != NULL, "Manual deve explicar os recursos.");
    check(!exists("manual.txt.tmp") && !exists("manual.txt.bak"), "Atualizacao normal nao deve deixar auxiliares.");
    endTest("Manual no terminal, criacao, atualizacao e retorno");
}
static void testComplete(int level) {
    SavedGame game;
    char input[4096];
    char line[64];
    char name[64];
    beginTest();
    snprintf(input, sizeof input, "1\nExplorador %d\n%d\nsalvar\nsair\nn\n0\n", level, level);
    play(input);
    readSave("partida.sav", &game);
    snprintf(line, sizeof line, "Mapa: %dx%d", game.values[1], game.values[2]);
    check(strstr(output, line) != NULL, "Mapa precisa usar o tamanho escolhido.");
    strcpy(input, "2\n");
    for (int i = 0; i < game.values[5]; i++) {
        snprintf(line, sizeof line, "%c%d\n", 'A' + game.treasures[i][1], game.treasures[i][0] + 1);
        strcat(input, line);
        if (level == HARD) {
            strcat(input, solutions[i]);
            strcat(input, "\n");
        }
    }
    strcat(input, "4\n0\n");
    play(input);
    check(strstr(output, "*** VITORIA ***") != NULL, "Partida completa precisa vencer.");
    check(strstr(output, "Resultado registrado") != NULL, "Resultado precisa ser registrado.");
    check(!exists("partida.sav"), "Save proprio concluido precisa ser removido.");
    play("4\n0\n");
    snprintf(line, sizeof line, "Explorador %d", level);
    check(strstr(output, line) != NULL, "Ranking precisa continuar depois de reiniciar.");
    snprintf(name, sizeof name, "Partida completa pelo menu, dificuldade %d", level);
    endTest(name);
}
static void testLastAttempt(int level, int win) {
    char target[16];
    char input[128];
    char name[128];
    beginTest();
    nearEnd(level, win, target);
    snprintf(input, sizeof input, "2\n%s\n", target);
    if (win && level == HARD) {
        strcat(input, solutions[6]);
        strcat(input, "\n");
    }
    strcat(input, "4\n0\n");
    play(input);
    check(strstr(output, win ? "*** VITORIA ***" : "*** DERROTA ***") != NULL,
          "Resultado incorreto na ultima tentativa.");
    snprintf(name, sizeof name, "%s na ultima tentativa, dificuldade %d", win ? "Vitoria" : "Derrota", level);
    endTest(name);
}
static void testClues(void) {
    SavedGame game;
    char input[16384];
    char target[16];
    char line[64];
    int empty[4][2];
    int count = 0;
    size_t length;
    beginTest();
    initialSave(EASY, &game);
    for (int row = 0; row < 6 && count < 4; row++) {
        for (int column = 0; column < 6 && count < 4; column++) {
            if (!hasTreasure(&game, row, column)) {
                empty[count][0] = row;
                empty[count][1] = column;
                count++;
            }
        }
    }
    coordinate(target, empty[0][0], empty[0][1]);
    strcpy(input, "2\nZ99\nB3xyz\nB9999999999999999999999999\n");
    length = strlen(input);
    memset(input + length, 'b', 5000);
    input[length + 5000] = '\0';
    strcat(input, "\n");
    strcat(input, target);
    strcat(input, "\n");
    strcat(input, target);
    strcat(input, "\n");
    for (int i = 0; i < 4; i++) {
        if (i > 0) {
            snprintf(line, sizeof line, "%c%d\n", 'A' + empty[i][1], empty[i][0] + 1);
            strcat(input, line);
        }
        snprintf(line, sizeof line, "radar %c%d\n", 'A' + empty[i][1], empty[i][0] + 1);
        strcat(input, line);
    }
    snprintf(line, sizeof line, "radar %s\nbussola %s\n", target, target);
    strcat(input, line);
    strcat(input, "salvar\nsair\nn\n0\n");
    play(input);
    check(strstr(output, "fora do tabuleiro") != NULL && strstr(output, "Entrada invalida") != NULL, "Coordenadas invalidas precisam ser rejeitadas.");
    check(strstr(output, "ja investigada") != NULL, "Repeticao precisa ser reconhecida.");
    check(occurrences(output, "RADAR: ") >= 4 && strstr(output, "Radar indisponivel") != NULL, "Radar precisa limitar casas distintas.");
    check(strstr(output, "BUSSOLA:") != NULL, "Bussola precisa indicar direcao.");
    readSave("partida.sav", &game);
    check(game.values[4] == 26 && game.values[7] == 2 && game.values[8] == 3, "Contadores de tentativas e recursos incorretos.");
    copyFile("partida.sav", "before.sav");
    snprintf(input, sizeof input, "2\nradar %s\nsalvar\nsair\nn\n0\n", target);
    play(input);
    check(filesEqual("partida.sav", "before.sav"), "Reconsulta e roundtrip devem preservar o estado salvo.");
    endTest("Investigacoes, radar, bussola e restauracao dos recursos");
}
static void testHard(int eof) {
    SavedGame before;
    SavedGame after;
    char input[256];
    beginTest();
    initialSave(HARD, &before);
    copyFile("partida.sav", "before.sav");
    snprintf(input, sizeof input, "2\n%c%d\n", 'A' + before.treasures[0][1], before.treasures[0][0] + 1);
    if (!eof) {
        strcat(input, "errada\nsalvar\nsair\nn\n0\n");
    }
    play(input);
    check(strstr(output, "Resposta incorreta") != NULL, "Resposta incorreta precisa ser tratada.");
    if (eof) {
        check(strstr(output, "Entrada encerrada") != NULL && filesEqual("partida.sav", "before.sav"), "EOF nao pode alterar o save explicito.");
        endTest("EOF durante o enigma Hard");
        return;
    }
    readSave("partida.sav", &after);
    check(after.values[6] == 0 && after.values[4] == 59, "Erro nao pode conceder tesouro nem gastar duas tentativas.");
    check(after.treasures[0][0] != before.treasures[0][0] || after.treasures[0][1] != before.treasures[0][1], "Tesouro precisa mudar de posicao.");
    check(after.board[before.treasures[0][0]][before.treasures[0][1]] == 'o', "Casa antiga deve ficar vazia.");
    check(after.board[after.treasures[0][0]][after.treasures[0][1]] == '.', "Nova casa deve continuar oculta.");
    for (int i = 1; i < 7; i++) {
        check(after.treasures[0][0] != after.treasures[i][0] || after.treasures[0][1] != after.treasures[i][1], "Realocacao nao pode sobrepor tesouros.");
    }
    endTest("Enigma incorreto e realocacao no Hard");
}
static void testSaveProtection(void) {
    SavedGame game;
    beginTest();
    initialSave(EASY, &game);
    copyFile("partida.sav", "before.sav");
    play("1\nOutra pessoa\n2\nsalvar\nn\nsair\nn\n0\n");
    check(filesEqual("partida.sav", "before.sav"), "Recusar substituicao deve preservar o save.");
    play("1\nOutra pessoa\n2\nsalvar\ns\nsair\nn\n0\n");
    readSave("partida.sav", &game);
    check(strcmp(game.name, "Outra pessoa") == 0, "Confirmar deve permitir substituir o save.");
    endTest("Confirmacao para substituir outro save");
}
static void testUnrelatedSave(void) {
    SavedGame game;
    char input[2048];
    char line[32];
    beginTest();
    initialSave(NORMAL, &game);
    copyFile("partida.sav", "before.sav");
    strcpy(input, "1\nOutra expedicao\n1\n");
    // Trinta casas novas encerram o Easy, com vitória ou derrota.
    for (int row = 0; row < 6; row++) {
        for (int column = 0; column < 6; column++) {
            snprintf(line, sizeof line, "%c%d\n", 'A' + column, row + 1);
            strcat(input, line);
        }
    }
    strcat(input, "0\n");
    play(input);
    check(strstr(output, "*** VITORIA ***") != NULL || strstr(output, "*** DERROTA ***") != NULL, "Outra expedicao precisa terminar.");
    check(filesEqual("partida.sav", "before.sav"), "Terminar outra partida nao pode apagar o save anterior.");
    endTest("Conclusao preserva o save de outra partida");
}
static void testDuplicate(void) {
    char path[8192];
    beginTest();
    coreFile(path, sizeof path, "won-1.sav");
    copyFile(path, "partida.sav");
    copyFile("partida.sav", "before.sav");
    play("2\n0\n");
    copyFile("ranking.txt", "before-ranking.txt");
    copyFile("before.sav", "partida.sav");
    play("2\n0\n");
    check(filesEqual("ranking.txt", "before-ranking.txt"), "Mesmo ID nao pode criar outro resultado.");
    endTest("Mesma partida nao duplica resultado entre execucoes");
}
static void testBestRanking(void) {
    char path[8192];
    beginTest();
    coreFile(path, sizeof path, "ranking.txt");
    copyFile(path, "ranking.txt");
    play("4\n0\n");
    check(occurrences(output, "Explorador de teste") == 3, "Deve existir apenas um melhor resultado por dificuldade.");
    check(occurrences(output, "| Vitoria |") == 3, "Derrotas inferiores nao podem substituir vitorias.");
    endTest("Melhor resultado por jogador e dificuldade");
}
static void testRankingOrder(void) {
    const char *names[] = {"Zulu", "Alfa", "Baixo", "Zulu"};
    const int attempts[] = {3, 3, 10, 8};
    char path[8192];
    const char *first;
    const char *second;
    const char *third;
    beginTest();
    coreFile(path, sizeof path, "won-1.sav");
    for (int i = 0; i < 4; i++) {
        SavedGame game;
        int explored = 3;
        readSave(path, &game);
        snprintf(game.id, sizeof game.id, "order-%d", i);
        strcpy(game.name, names[i]);
        game.values[4] = 30 - attempts[i];
        game.values[8] = 0;
        game.values[9] = 300 + game.values[4] * 10;
        for (int row = 0; row < 6; row++) {
            for (int column = 0; column < 6; column++) {
                game.radar[row][column] = '0';
                if (game.board[row][column] != '$') {
                    game.board[row][column] = '.';
                    if (explored < attempts[i]) {
                        game.board[row][column] = 'o';
                        explored++;
                    }
                }
            }
        }
        writeSave("partida.sav", &game);
        play("2\n0\n");
    }
    play("4\n0\n");
    first = strstr(output, "Alfa");
    second = strstr(output, "Zulu");
    third = strstr(output, "Baixo");
    check(first && second && third, "Nomes do ranking precisam aparecer.");
    if (first && second && third) {
        check(first < second && second < third, "Pontuacao e desempate devem definir a ordem.");
    }
    check(occurrences(output, "Zulu") == 1, "Resultado inferior nao pode criar outra linha para o mesmo nome.");
    endTest("Ordenacao, desempate e melhor pontuacao");
}

static void corruptSave(int kind) {
    SavedGame game;
    char lines[40][512];
    FILE *file;
    int count = 0;
    int total;
    char name[128];
    const char *kinds[] = {"incompleto", "encontrados", "tentativas", "recursos", "pontuacao", "dimensoes", "duplicacao", "descoberta", "mapa", "radar", "sufixo", "overflow", "sobras", "antigo", "versao"};
    beginTest();
    initialSave(EASY, &game);
    file = fopen("partida.sav", "r");
    assert(file);
    while (count < 40 && fgets(lines[count], sizeof lines[count], file)) {
        lines[count][strcspn(lines[count], "\r\n")] = '\0';
        count++;
    }
    fclose(file);
    total = count;
    switch (kind) {
        case 0: total = 5;
        break;
        case 1: game.values[6] = 3;
        game.values[9] = 600;
        break;
        case 2: game.values[4] = 29;
        break;
        case 3: game.values[7] = 999;
        break;
        case 4: game.values[9] = 999;
        break;
        case 5: game.values[1] = 10;
        break;
        case 6: strcpy(lines[5], lines[4]);
        break;
        case 7: snprintf(lines[4], sizeof lines[4], "%d %d 7", game.treasures[0][0], game.treasures[0][1]);
        break;
        case 8: lines[4 + game.values[5]][0] = '$';
        break;
        case 9: lines[4 + game.values[5] + game.values[1]][0] = '1';
        break;
        case 10: strcat(lines[3], " xyz");
        break;
        case 11: strcpy(lines[3], "999999999999999999999999999999999999999999999999");
        break;
        case 12: strcpy(lines[total++], "surpresa");
        break;
        case 13: strcpy(lines[0], "6 6 30 30 3 0");
        total = 1;
        break;
        case 14: strcpy(lines[0], "ATLAS_SAVE 999");
        break;
    }
    if (kind >= 1 && kind <= 5) {
        snprintf(lines[3], sizeof lines[3], "%d %d %d %d %d %d %d %d %d %d",
                 game.values[0], game.values[1], game.values[2], game.values[3], game.values[4],
                 game.values[5], game.values[6], game.values[7], game.values[8], game.values[9]);
    }
    file = fopen("partida.sav", "w");
    assert(file);
    for (int i = 0; i < total; i++) {
        fprintf(file, "%s\n", lines[i]);
    }
    fclose(file);
    copyFile("partida.sav", "before.sav");
    play("2\n0\n");
    check(strstr(output, "Nenhuma partida salva valida") != NULL, "Save incorreto precisa ser rejeitado.");
    check(filesEqual("partida.sav", "before.sav"), "Save rejeitado precisa permanecer intacto.");
    snprintf(name, sizeof name, "Save invalido: %s", kinds[kind]);
    endTest(name);
}
static void testWriteFailure(void) {
    SavedGame game;
    beginTest();
    initialSave(EASY, &game);
    copyFile("partida.sav", "before.sav");
    writeText("partida.sav.tmp", "nao sobrescrever");
    play("2\nA1\nsalvar\nsair\nn\n0\n");
    check(strstr(output, "ERRO ao salvar") != NULL, "Falha de escrita precisa ser informada.");
    check(filesEqual("partida.sav", "before.sav"), "Falha nao pode destruir o save.");
    readText("partida.sav.tmp", output, sizeof output);
    check(strcmp(output, "nao sobrescrever") == 0, "Temporario anterior nao pode ser sobrescrito.");
    endTest("Falha de armazenamento preserva o save");
}
static void testBackup(void) {
    char path[8192];
    beginTest();
    coreFile(path, sizeof path, "fixture-1.sav");
    copyFile(path, "partida.sav.bak");
    play("2\nsalvar\nsair\nn\n0\n");
    check(strstr(output, "Partida retomada!") != NULL && strstr(output, "ERRO ao salvar") != NULL, "Backup precisa permitir leitura e bloquear troca insegura.");
    check(exists("partida.sav.bak"), "Backup precisa ser preservado.");
    endTest("Leitura de backup apos interrupcao");
}
static void testInvalidRanking(void) {
    char path[8192];
    beginTest();
    coreFile(path, sizeof path, "won-1.sav");
    copyFile(path, "partida.sav");
    copyFile("partida.sav", "before.sav");
    writeText("ranking.txt", "dados invalidos\n");
    copyFile("ranking.txt", "before-ranking.txt");
    play("4\n2\n0\n");
    check(strstr(output, "ranking invalido") != NULL && strstr(output, "resultado nao registrado") != NULL, "Ranking invalido precisa ser informado.");
    check(filesEqual("partida.sav", "before.sav") && filesEqual("ranking.txt", "before-ranking.txt"), "Erro nao pode alterar dados anteriores.");
    endTest("Ranking invalido preservado ao consultar e registrar");
}
static void testRankingWriteFailure(void) {
    char path[8192];
    beginTest();
    coreFile(path, sizeof path, "won-1.sav");
    copyFile(path, "partida.sav");
    writeText("ranking.txt.tmp", "bloqueado");
    play("2\n0\n");
    check(strstr(output, "resultado nao registrado") != NULL && exists("partida.sav"), "Falha no ranking nao pode apagar o save.");
    endTest("Falha de escrita no ranking preserva o save");
}
static void testNul(void) {
    const char input[] = {'1', '\0', 'a', 'b', 'c', '\n', '0', '\n'};
    beginTest();
    runProgram(executable, input, sizeof input);
    check(strstr(output, "opcao invalida") != NULL && strstr(output, "Nome do explorador") == NULL, "Byte NUL deve invalidar a linha inteira.");
    endTest("Entrada com NUL rejeitada");
}

int main(void) {
    char name[128];
    char path[8192];
    int compiled;
#ifdef _WIN32
    assert(_getcwd(root, sizeof root));
#else
    assert(getcwd(root, sizeof root));
#endif
    createDirectory("build");
    snprintf(name, sizeof name, "c-tests-%lld-%lu", (long long)time(NULL), (unsigned long)clock());
    joinPath(path, sizeof path, root, "build");
    joinPath(work, sizeof work, path, name);
    createDirectory(work);
    joinPath(executable, sizeof executable, root, "atlas.exe");
    report = fopen("build/c-test-results.txt", "w");
    assert(report);
    fprintf(report, "Evidencias: %s\n", work);

    beginTest();
    compiled = compileAll();
    check(compiled, "Compilacao precisa passar sem avisos.");
    endTest("Compilacao C11 completa e linkagem sem -Iinclude");
    if (!compiled) {
        fclose(report);
        return 1;
    }
    beginTest();
    strcpy(coreFolder, currentCase);
    joinPath(path, sizeof path, root, "build/test_core.exe");
    runProgram(path, "", 0);
    check(strstr(output, "PASSOU") != NULL && exists("fixture-3.sav"), "Nucleo precisa gerar os cenarios controlados.");
    endTest("Nucleo, dicas, vitoria, derrota e roundtrip nas tres dificuldades");
    if (failed) {
        fclose(report);
        return 1;
    }
    beginTest();
    joinPath(path, sizeof path, root, "build/test_alloc.exe");
    runProgram(path, "", 0);
    check(strstr(output, "zero alocacoes pendentes") != NULL, "Teste de memoria precisa liberar tudo.");
    endTest("Falhas de alocacao e liberacao de memoria");
    beginTest();
    joinPath(path, sizeof path, root, "build/test_board.exe");
    runProgram(path, "", 0);
    check(strstr(output, "PASSOU") != NULL, "Mapa precisa funcionar nas tres dimensoes.");
    endTest("Apresentacao do mapa nas tres dificuldades");

    testMenu();
    testEof();
    testNames();
    testManual();
    for (int level = EASY; level <= HARD; level++) {
        testComplete(level);
        testLastAttempt(level, 1);
        testLastAttempt(level, 0);
    }
    testClues();
    testHard(0);
    testHard(1);
    testSaveProtection();
    testUnrelatedSave();
    testDuplicate();
    testBestRanking();
    testRankingOrder();
    for (int kind = 0; kind < 15; kind++) {
        corruptSave(kind);
    }
    testWriteFailure();
    testBackup();
    testInvalidRanking();
    testRankingWriteFailure();
    testNul();

    printf("RESULTADO: %d/%d PASSOU. Evidencias: %s\n", passed, tests, work);
    fprintf(report, "RESULTADO: %d/%d PASSOU\n", passed, tests);
    fclose(report);
    return passed == tests ? 0 : 1;
}