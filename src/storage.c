#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "../include/storage.h"

// Reserva espaço para juntar o nome do arquivo com .tmp ou .bak.
static char *withSuffix(const char *path, const char *suffix) {
    size_t length = strlen(path) + strlen(suffix) + 1;
    char *result = malloc(length);
    if (result) {
        snprintf(result, length, "%s%s", path, suffix);
    }
    return result;
}

// Se o arquivo principal estiver ausente, tenta ler a cópia de segurança.
FILE *openStoredRead(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file && errno == ENOENT) {
        char *backup = withSuffix(path, ".bak");
        if (backup) {
            file = fopen(backup, "rb");
            free(backup);
        }
    }
    return file;
}

// Prepara um arquivo novo sem apagar dados de uma gravação interrompida.
FILE *beginStoredWrite(const char *path, char *temporary, size_t size) {
    FILE *existing;
    char *backup;
    int length = snprintf(temporary, size, "%s.tmp", path);
    if (length < 0 || (size_t)length >= size) {
        return NULL;
    }
    backup = withSuffix(path, ".bak");
    if (!backup) {
        return NULL;
    }
    existing = fopen(backup, "rb");
    if (existing) {
        fclose(existing);
        free(backup);
        return NULL;
    }
    if (errno != ENOENT) {
        free(backup);
        return NULL;
    }
    free(backup);

    existing = fopen(path, "rb");
    if (existing) {
        fclose(existing);
    } else if (errno != ENOENT) {
        return NULL;
    }
    // O modo x recusa um temporário que já exista.
    return fopen(temporary, "wx");
}

// Confere a escrita e só então troca o arquivo anterior pelo novo.
int finishStoredWrite(FILE *file, const char *path, const char *temporary) {
    char *backup;
    int ok;
    int hadOriginal = 0;

    if (!file) {
        return 0;
    }
    ok = !ferror(file);
    if (fflush(file) != 0) {
        ok = 0;
    }
    if (fclose(file) != 0) {
        ok = 0;
    }
    if (!ok) {
        remove(temporary);
        return 0;
    }

    backup = withSuffix(path, ".bak");
    if (!backup) {
        remove(temporary);
        return 0;
    }
    if (rename(path, backup) == 0) {
        hadOriginal = 1;
    } else if (errno != ENOENT) {
        remove(temporary);
        free(backup);
        return 0;
    }

    if (rename(temporary, path) != 0) {
        // Tenta recolocar o arquivo anterior se a troca falhar.
        if (hadOriginal) {
            (void)rename(backup, path);
        }
        remove(temporary);
        free(backup);
        return 0;
    }
    if (hadOriginal && remove(backup) != 0) {
        fprintf(stderr, "Aviso: gravado, mas backup auxiliar preservado em %s.\n", backup);
    }
    free(backup);
    return 1;
}