#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../xerrori/xerrori.h"

char **get_file_names(int argc, char *argv[], int *n) {
    char **file_names = malloc(sizeof(char *) * argc - 1);
    if (file_names == NULL) {
        termina("Malloc fallita");
    }

    int trovati = 0;

    for (int i = 1; i < argc; i++) {
        if (strstr(argv[i], "-n") || strstr(argv[i], "-q") || strstr(argv[i], "-t")) {
            i += 1;
        } else {
            file_names[trovati++] = argv[i];
        }
    }

    file_names = realloc(file_names, sizeof(char *) * trovati);
    if (file_names == NULL) {
        termina("Malloc fallita");
    }
    *n = trovati;

    return file_names;
}