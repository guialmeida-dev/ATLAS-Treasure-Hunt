#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "treasure.h"

#define SAVE_FILE     "partida.sav"
#define RANKING_FILE  "ranking.txt"
#define RANKING_MAX   10
#define NAME_MAX_LEN  32

// Símbolos do tabuleiro conhecido pelo jogador
#define CELL_HIDDEN    '.'   // Ainda não investigada
#define CELL_EMPTY     'o'   // Investigada, sem tesouro
#define CELL_TREASURE  '$'   // Investigada, tesouro encontrado

typedef struct {
    char name[NAME_MAX_LEN];
    int won;            // 1 = venceu, 0 = perdeu
    int found;
    int attemptsUsed;
    int points;
} RankingEntry;

// ---------------- Coleção dinâmica de tesouros ----------------

static int addTreasure(TreasureGame *game, int row, int column, int found)
{
    if (game->treasureCount == game->treasureCapacity) {
        int newCapacity = (game->treasureCapacity == 0) ? 4 : game->treasureCapacity * 2;
        Treasure *bigger = realloc(game->treasures, (size_t)newCapacity * sizeof(Treasure));
        if (bigger == NULL) return 0;
        game->treasures = bigger;
        game->treasureCapacity = newCapacity;
    }
    game->treasures[game->treasureCount].row = row;
    game->treasures[game->treasureCount].column = column;
    game->treasures[game->treasureCount].found = found;
    game->treasureCount++;
    return 1;
}

// Retorna o índice do tesouro na posição, ou -1 se não houver
static int findTreasure(const TreasureGame *game, int row, int column)
{
    for (int i = 0; i < game->treasureCount; i++)
        if (game->treasures[i].row == row && game->treasures[i].column == column)
            return i;
    return -1;
}

void freeTreasureGame(TreasureGame *game)
{
    free(game->treasures);
    game->treasures = NULL;
    game->treasureCount = game->treasureCapacity = game->foundCount = 0;
}

static void clearBoard(TreasureGame *game)
{
    for (int i = 0; i < TREASURE_MAX_SIZE; i++)
        for (int j = 0; j < TREASURE_MAX_SIZE; j++)
            game->board[i][j] = CELL_HIDDEN;
}

// ---------------- Ciclo da partida ----------------

int startTreasureGame(TreasureGame *game, gameConfig config)
{
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned)time(NULL));
        seeded = 1;
    }

    memset(game, 0, sizeof(*game));
    if (config.rows < 1 || config.rows > TREASURE_MAX_SIZE
        || config.columns < 1 || config.columns > TREASURE_MAX_SIZE
        || config.treasures < 1 || config.treasures > config.rows * config.columns
        || config.attempts < 1)
        return 0;

    game->rows = config.rows;
    game->columns = config.columns;
    game->maxAttempts = config.attempts;
    game->attemptsLeft = config.attempts;
    clearBoard(game);

    while (game->treasureCount < config.treasures) {
        int row = rand() % game->rows;
        int column = rand() % game->columns;
        if (findTreasure(game, row, column) != -1) continue; // Sem repetição
        if (!addTreasure(game, row, column, 0)) {
            freeTreasureGame(game);
            return 0;
        }
    }
    return 1;
}

TreasureState getTreasureState(const TreasureGame *game)
{
    if (game->foundCount == game->treasureCount) return TREASURE_WON;
    if (game->attemptsLeft <= 0) return TREASURE_LOST;
    return TREASURE_RUNNING;
}

InvestigateResult investigateCoordinate(TreasureGame *game, int row, int column)
{
    if (row < 0 || row >= game->rows || column < 0 || column >= game->columns)
        return INVESTIGATE_INVALID;

    if (game->board[row][column] != CELL_HIDDEN)
        return INVESTIGATE_REPEATED;        // Regra 5: não consome tentativa

    game->attemptsLeft--;                   // Regra 6: tentativas limitadas

    int index = findTreasure(game, row, column);
    if (index >= 0) {
        game->treasures[index].found = 1;
        game->foundCount++;
        game->board[row][column] = CELL_TREASURE;
        return INVESTIGATE_TREASURE;
    }
    game->board[row][column] = CELL_EMPTY;  // Regra 4: posição fica marcada
    return INVESTIGATE_EMPTY;
}

// Exibe SOMENTE o estado conhecido pelo jogador (regra 3)
void showKnownBoard(const TreasureGame *game)
{
    printf("\n     ");
    for (int c = 0; c < game->columns; c++) printf("%c ", 'A' + c);
    printf("\n");
    for (int r = 0; r < game->rows; r++) {
        printf("  %2d ", r + 1);
        for (int c = 0; c < game->columns; c++) printf("%c ", game->board[r][c]);
        printf("\n");
    }
    printf("\n  Legenda: %c oculto | %c vazio | %c tesouro\n",
           CELL_HIDDEN, CELL_EMPTY, CELL_TREASURE);
    printf("  Tesouros encontrados: %d/%d | Tentativas restantes: %d/%d\n\n",
           game->foundCount, game->treasureCount, game->attemptsLeft, game->maxAttempts);
}

// ---------------- Salvar / retomar partida ----------------

int saveTreasureGame(const TreasureGame *game, const char *fileName)
{
    FILE *file = fopen(fileName, "w");
    if (file == NULL) return 0;

    fprintf(file, "%d %d %d %d %d %d\n", game->rows, game->columns, game->maxAttempts,
            game->attemptsLeft, game->treasureCount, game->foundCount);
    for (int i = 0; i < game->treasureCount; i++)
        fprintf(file, "%d %d %d\n", game->treasures[i].row, game->treasures[i].column,
                game->treasures[i].found);
    for (int r = 0; r < game->rows; r++) {
        for (int c = 0; c < game->columns; c++) fputc(game->board[r][c], file);
        fputc('\n', file);
    }
    fclose(file);
    return 1;
}

int loadTreasureGame(TreasureGame *game, const char *fileName)
{
    FILE *file = fopen(fileName, "r");
    if (file == NULL) return 0;

    memset(game, 0, sizeof(*game));
    int rows, columns, maxAttempts, attemptsLeft, treasureCount, foundCount;
    if (fscanf(file, "%d %d %d %d %d %d", &rows, &columns, &maxAttempts,
               &attemptsLeft, &treasureCount, &foundCount) != 6
        || rows < 1 || rows > TREASURE_MAX_SIZE
        || columns < 1 || columns > TREASURE_MAX_SIZE
        || maxAttempts < 1 || attemptsLeft < 0 || attemptsLeft > maxAttempts
        || treasureCount < 1 || treasureCount > rows * columns
        || foundCount < 0 || foundCount > treasureCount) {
        fclose(file);
        return 0;
    }
    game->rows = rows;
    game->columns = columns;
    game->maxAttempts = maxAttempts;
    game->attemptsLeft = attemptsLeft;
    game->foundCount = foundCount;
    clearBoard(game);

    for (int i = 0; i < treasureCount; i++) {
        int r, c, found;
        if (fscanf(file, "%d %d %d", &r, &c, &found) != 3
            || r < 0 || r >= rows || c < 0 || c >= columns
            || findTreasure(game, r, c) != -1
            || !addTreasure(game, r, c, found ? 1 : 0)) {
            freeTreasureGame(game);
            fclose(file);
            return 0;
        }
    }
    for (int r = 0; r < rows; r++) {
        char line[TREASURE_MAX_SIZE + 8];
        if (fscanf(file, "%15s", line) != 1 || (int)strlen(line) != columns) {
            freeTreasureGame(game);
            fclose(file);
            return 0;
        }
        for (int c = 0; c < columns; c++) {
            if (line[c] != CELL_HIDDEN && line[c] != CELL_EMPTY && line[c] != CELL_TREASURE) {
                freeTreasureGame(game);
                fclose(file);
                return 0;
            }
            game->board[r][c] = line[c];
        }
    }
    fclose(file);
    return 1;
}

// ---------------- Ranking ----------------

static int compareRanking(const void *a, const void *b)
{
    const RankingEntry *x = a, *y = b;
    return y->points - x->points;           // Maior pontuação primeiro
}

// Lê o ranking para um vetor dinâmico. Retorna a quantidade lida
static int loadRanking(RankingEntry **entries)
{
    *entries = NULL;
    FILE *file = fopen(RANKING_FILE, "r");
    if (file == NULL) return 0;

    int count = 0, capacity = 0;
    RankingEntry entry;
    while (fscanf(file, "%31s %d %d %d %d", entry.name, &entry.won, &entry.found,
                  &entry.attemptsUsed, &entry.points) == 5) {
        if (count == capacity) {
            capacity = capacity ? capacity * 2 : 8;
            RankingEntry *bigger = realloc(*entries, (size_t)capacity * sizeof(*bigger));
            if (bigger == NULL) break;
            *entries = bigger;
        }
        (*entries)[count++] = entry;
    }
    fclose(file);
    return count;
}

void registerRanking(const char *name, const TreasureGame *game, int won)
{
    RankingEntry *entries;
    int count = loadRanking(&entries);

    RankingEntry *bigger = realloc(entries, (size_t)(count + 1) * sizeof(*bigger));
    if (bigger == NULL) { free(entries); return; }
    entries = bigger;

    RankingEntry *entry = &entries[count];
    snprintf(entry->name, NAME_MAX_LEN, "%s", name);
    entry->won = won;
    entry->found = game->foundCount;
    entry->attemptsUsed = game->maxAttempts - game->attemptsLeft;
    // 100 por tesouro; bônus de 10 por tentativa que sobrou em vitória
    entry->points = game->foundCount * 100 + (won ? game->attemptsLeft * 10 : 0);
    count++;

    qsort(entries, (size_t)count, sizeof(*entries), compareRanking);
    if (count > RANKING_MAX) count = RANKING_MAX;

    FILE *file = fopen(RANKING_FILE, "w");
    if (file != NULL) {
        for (int i = 0; i < count; i++)
            fprintf(file, "%s %d %d %d %d\n", entries[i].name, entries[i].won,
                    entries[i].found, entries[i].attemptsUsed, entries[i].points);
        fclose(file);
    }
    free(entries);
}

void showRanking()
{
    RankingEntry *entries;
    int count = loadRanking(&entries);

    printf("\n===== RANKING (top %d) =====\n", RANKING_MAX);
    if (count == 0) {
        printf("Nenhum resultado registrado ainda.\n");
    } else {
        printf("%-3s %-16s %-9s %-9s %-10s %s\n",
               "#", "Jogador", "Resultado", "Tesouros", "Tentativas", "Pontos");
        for (int i = 0; i < count; i++)
            printf("%-3d %-16s %-9s %-9d %-10d %d\n", i + 1, entries[i].name,
                   entries[i].won ? "Vitoria" : "Derrota", entries[i].found,
                   entries[i].attemptsUsed, entries[i].points);
    }
    printf("\n");
    free(entries);
}

// ---------------- Entrada e laço da partida ----------------

static int readLine(const char *prompt, char *buffer, size_t size)
{
    printf("%s", prompt);
    fflush(stdout);
    if (fgets(buffer, (int)size, stdin) == NULL) return 0;
    buffer[strcspn(buffer, "\r\n")] = '\0';
    return 1;
}

// Converte "B3" (coluna letra, linha número) em índices a partir de 0
static int parseCoordinate(const char *text, int *row, int *column)
{
    char letter;
    int number;
    if (sscanf(text, " %c%d", &letter, &number) != 2 || !isalpha((unsigned char)letter))
        return 0;
    *column = toupper((unsigned char)letter) - 'A';
    *row = number - 1;
    return 1;
}

static void finishGame(TreasureGame *game)
{
    TreasureState state = getTreasureState(game);
    char name[NAME_MAX_LEN];

    if (state == TREASURE_WON)
        printf("*** VITORIA! Todos os tesouros foram encontrados! ***\n");
    else
        printf("*** DERROTA! As tentativas acabaram. ***\n");

    if (state == TREASURE_LOST) {
        printf("Os tesouros restantes estavam em: ");
        for (int i = 0; i < game->treasureCount; i++)
            if (!game->treasures[i].found)
                printf("%c%d ", 'A' + game->treasures[i].column, game->treasures[i].row + 1);
        printf("\n");
    }

    if (!readLine("Seu nome para o ranking (sem espacos): ", name, sizeof name)
        || name[0] == '\0')
        snprintf(name, sizeof name, "Anonimo");
    for (char *p = name; *p; p++) if (isspace((unsigned char)*p)) *p = '_';

    registerRanking(name, game, state == TREASURE_WON);
    remove(SAVE_FILE);                      // Partida encerrada
    showRanking();
}

static void playGame(TreasureGame *game)
{
    char input[64];

    printf("\nDigite uma coordenada (ex.: B3), 'salvar' ou 'sair'.\n");
    while (getTreasureState(game) == TREASURE_RUNNING) {
        showKnownBoard(game);
        if (!readLine("Coordenada> ", input, sizeof input)) return;

        if (strcmp(input, "salvar") == 0) {
            printf(saveTreasureGame(game, SAVE_FILE) ? "Partida salva em %s.\n"
                                                     : "Erro ao salvar em %s.\n",
                   SAVE_FILE);
            continue;
        }
        if (strcmp(input, "sair") == 0) {
            printf("Salvar a partida antes de sair? (s/n) ");
            if (readLine("", input, sizeof input) && tolower((unsigned char)input[0]) == 's')
                printf(saveTreasureGame(game, SAVE_FILE) ? "Partida salva.\n"
                                                         : "Erro ao salvar.\n");
            return;
        }

        int row, column;
        if (!parseCoordinate(input, &row, &column)) {
            printf("Entrada invalida. Use letra + numero, ex.: B3.\n");
            continue;
        }
        switch (investigateCoordinate(game, row, column)) {
            case INVESTIGATE_INVALID:
                printf("Coordenada fora do tabuleiro (A-%c, 1-%d).\n",
                       'A' + game->columns - 1, game->rows);
                break;
            case INVESTIGATE_REPEATED:
                printf("Posicao ja investigada. Nenhuma tentativa foi gasta.\n");
                break;
            case INVESTIGATE_EMPTY:
                printf("Nada por aqui...\n");
                break;
            case INVESTIGATE_TREASURE:
                printf("TESOURO ENCONTRADO em %c%d!\n", 'A' + column, row + 1);
                break;
        }
    }
    showKnownBoard(game);
    finishGame(game);
}

void runTreasureHunt(gameConfig config)
{
    TreasureGame game;
    if (!startTreasureGame(&game, config)) {
        printf("Erro ao iniciar a partida.\n");
        return;
    }
    playGame(&game);
    freeTreasureGame(&game);
}

void resumeTreasureHunt()
{
    TreasureGame game;
    if (!loadTreasureGame(&game, SAVE_FILE)) {
        printf("\nNenhuma partida salva valida encontrada.\n");
        return;
    }
    printf("\nPartida retomada!\n");
    playGame(&game);
    freeTreasureGame(&game);
}
