// Estas quatro trocas só são usadas para simular falta de memória nos testes.
#ifndef ATLAS_ALLOC_HOOKS_H
#define ATLAS_ALLOC_HOOKS_H
#include <stdlib.h>

void *atlasTestMalloc(size_t n);
void *atlasTestCalloc(size_t n, size_t size);
void *atlasTestRealloc(void *p, size_t n);
void atlasTestFree(void *p);

#define malloc atlasTestMalloc
#define calloc atlasTestCalloc
#define realloc atlasTestRealloc
#define free atlasTestFree

#endif
