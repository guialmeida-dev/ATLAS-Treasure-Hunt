#include <stdio.h>
#include <stdlib.h>
#include "../include/screen.h"

void clearScreen(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
#endif
}