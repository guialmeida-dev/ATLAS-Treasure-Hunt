#include <stdlib.h>
#include <time.h>
#include "../include/menu.h"

int main(void) {
    // Muda o sorteio das posições a cada execução do jogo.
    srand((unsigned)time(NULL) ^ (unsigned)clock());
    runMainMenu();
    return 0;
}