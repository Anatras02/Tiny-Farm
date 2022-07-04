#include "consumatore.h"
#include "../structs/structs.h"
#include "../funzioni/funzioni.h"
#include <unistd.h>
#include <arpa/inet.h>

#define HOST "127.0.0.1"
#define PORT 65432

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


        char default_file_directory[strlen(a->default_file_directory) + strlen(filename) + 1];
        strcpy(default_file_directory, a->default_file_directory);
        strcat(default_file_directory, filename);
        char *filename_directory = default_file_directory;

        FILE *f = fopen(filename_directory, "r");
        if (f == NULL) {
            perror("Errore apertura file consumatore");
            pthread_exit(NULL);
        }
        long somma = calcola_somma(f);
        fclose(f);

        char * stringa_socket = genera_stringa_socket(filename, somma);
        int dimensione = strlen(stringa_socket);

        // Apriamo una nuove connessione ogni volta per sfruttare al meglio il multithreading sul server (anche se comunque fa solo il decode e il print)
        // file descriptor associato al socket
        struct sockaddr_in serv_addr;
        size_t e;
        long tmp;
        int fd_skt;

        // assegna indirizzo
        socket_init(&fd_skt, &serv_addr, HOST, PORT);
        // apre connessione
        socket_connect(fd_skt, serv_addr);

        tmp = htonl(dimensione);
        e = writen(fd_skt, &tmp, sizeof(tmp));
        if (e != sizeof(tmp)) perror("Errore write dimensione");

        e = writen(fd_skt, stringa_socket, dimensione);
        if (e != dimensione) termina("Errore write stringa socket");

        if (close(fd_skt) < 0)
            perror("Errore chiusura socket");

        free(stringa_socket);
    } while (true);



    pthread_exit(NULL);
}