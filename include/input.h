#ifndef INPUT_H
#define INPUT_H
#include <stdio.h>
#include <stddef.h>
// 1: linha completa, 0: EOF, -1: linha longa/invalida, -2: erro de leitura.
int readStreamLine(FILE *stream, char *buffer, size_t size);
int readInput(const char *prompt, char *buffer, size_t size);
void trimText(char *text);
int parseInteger(const char *text, int minimum, int maximum, int *value);
int validPlayerName(const char *text);

#endif
