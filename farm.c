#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[]) {
    // controlla numero argomenti
    if (argc < 2) {
        printf("Uso: %s file [file ...] \n", argv[0]);
        return 1;
    }

    int opt;
    int numero_thread = 4;
    int lunghezza_buffer = 8;
    int delay = 0;

    opterr = 0;
    while ((opt = getopt(argc, argv, "n:q:t:")) !=
           -1) { //Se lo stesso parametro viene passato più volte prende l'ultimo
        switch (opt) {
            case 'n':
                numero_thread = atoi(optarg);
                break;
            case 'q':
                lunghezza_buffer = atoi(optarg);
                break;
            case 't':
                delay = atoi(optarg);
                break;
            default:
                continue; //Se vengono passati altri parametri li salta
        }
    }
    opterr = 1;

    assert(numero_thread > 0);
    assert(lunghezza_buffer > 0);
    assert(delay >= 0);


    return 0;
}