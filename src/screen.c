#include <stdlib.h>
#include "../include/screen.h"

// Limpa a tela de acordo com o sistema operacional.
void clearScreen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}