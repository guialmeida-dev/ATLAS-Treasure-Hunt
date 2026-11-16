#ifndef STORAGE_H
#define STORAGE_H
#include <stdio.h>
// Grava primeiro em um temporário e preserva uma cópia do arquivo anterior.
FILE *beginStoredWrite(const char *path, char *temporary, size_t size);
int finishStoredWrite(FILE *file, const char *path, const char *temporary);
FILE *openStoredRead(const char *path);

#endif
