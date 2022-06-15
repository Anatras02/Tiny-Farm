#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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

    while ((opt = getopt(argc, argv, "n:q:t:")) != -1) {
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
                continue;
        }
    }

    return 0;
}