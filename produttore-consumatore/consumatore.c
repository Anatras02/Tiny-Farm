#include "consumatore.h"
#include "../structs/structs.h"

void *consumatore_body(void *arg) {
    cdati *a = (cdati *) arg;

    char *filename;
    do {
        xsem_wait(a->sem_data_items, QUI);
        xpthread_mutex_lock(a->cmutex, QUI);
        filename = a->buffer[*(a->cindex) % a->dimensione_buffer];
        *(a->cindex) += 1;
        xpthread_mutex_unlock(a->cmutex, QUI);
        xsem_post(a->sem_free_slots, QUI);

        if (strcmp(filename, "BREAK") == 0) break;


        char default_file_directory[strlen(a->default_file_directory) + 1];
        strcpy(default_file_directory, a->default_file_directory);
        strcat(default_file_directory, filename);
        filename = default_file_directory;

        FILE *f = fopen(filename, "r");
        if (f == NULL) {
            perror("Errore apertura file consumatore");
            pthread_exit(NULL);
        }

        long n;
        long sum = 0;
        int counter = 0;
        while (true) {
            int e = fread(&n, sizeof(long), 1, f);
            if (e != 1) break;

            sum += counter * n;
            counter += 1;
        }

        printf("%s %ld\n", filename, sum);

    } while (true);

    pthread_exit(NULL);
}