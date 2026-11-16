#ifndef DIFFICULTY_H
#define DIFFICULTY_H

typedef enum { INVALID = -1, BACK = 0, EASY = 1, NORMAL = 2, HARD = 3 } difficulty;
typedef struct {
    int rows, columns, treasures, attempts, compasses;
} gameConfig;

difficulty showDifficulty(void);
difficulty difficultyLevel(char data[]);
gameConfig getGameConf(difficulty level);
const char *difficultyName(difficulty level);

#endif
