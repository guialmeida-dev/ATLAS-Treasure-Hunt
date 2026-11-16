#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "../include/input.h"

// Lê a linha inteira, mesmo quando ela não cabe no vetor recebido.
int readStreamLine(FILE *stream, char *buffer, size_t size) {
    size_t length = 0;
    int ch;
    int invalid = 0;
    int received = 0;

    if (!stream || !buffer || size < 2) {
        return -2;
    }
    while ((ch = fgetc(stream)) != EOF && ch != '\n') {
        received = 1;
        // Aceita a quebra de linha usada tanto no Windows quanto no Linux.
        if (ch == '\r') {
            int next = fgetc(stream);
            if (next == '\n') {
                break;
            }
            if (next != EOF) {
                ungetc(next, stream);
            }
        }
        if (ch == 0) {
            invalid = 1;
        }
        if (length + 1 < size) {
            buffer[length] = (char)ch;
            length++;
        } else {
            // Descarta o excesso para não atrapalhar a próxima leitura.
            invalid = 1;
        }
    }
    if (length > 0 && buffer[length - 1] == '\r') {
        length--;
    }
    buffer[length] = '\0';

    if (ferror(stream)) {
        return -2;
    }
    if (!received && ch == EOF) {
        return 0;
    }
    if (invalid) {
        return -1;
    }
    return 1;
}

int readInput(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    fflush(stdout);
    return readStreamLine(stdin, buffer, size);
}

// Retira apenas os espaços das extremidades do texto.
void trimText(char *text) {
    char *start = text;
    size_t length;

    while (isspace((unsigned char)*start)) {
        start++;
    }
    memmove(text, start, strlen(start) + 1);
    length = strlen(text);
    while (length > 0 && isspace((unsigned char)text[length - 1])) {
        length--;
        text[length] = '\0';
    }
}

// strtol permite detectar números grandes demais e sobras como "1abc".
int parseInteger(const char *text, int minimum, int maximum, int *value) {
    char *end;
    long number;

    if (!text || !value || !*text) {
        return 0;
    }
    errno = 0;
    number = strtol(text, &end, 10);
    if (errno != 0 || end == text) {
        return 0;
    }
    while (isspace((unsigned char)*end)) {
        end++;
    }
    if (*end != '\0' || number < minimum || number > maximum) {
        return 0;
    }
    *value = (int)number;
    return 1;
}

int validPlayerName(const char *text) {
    size_t length;
    int visible = 0;

    if (!text) {
        return 0;
    }
    length = strlen(text);
    if (length == 0 || length > 50) {
        return 0;
    }
    for (size_t i = 0; i < length; i++) {
        unsigned char character = (unsigned char)text[i];
        if (character < 32 || character == 127) {
            return 0;
        }
        if (!isspace(character)) {
            visible = 1;
        }
    }
    return visible;
}