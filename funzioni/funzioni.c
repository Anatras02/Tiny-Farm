#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "../xerrori/xerrori.h"

char **get_nomi_file(int argc, char *argv[], int *n) {
    char **file_names = malloc(sizeof(char *) * argc - 1);
    if (file_names == NULL) {
        termina("Malloc fallita");
    }

    int trovati = 0;

    for (int i = 1; i < argc; i++) {
        if (strstr(argv[i], "-n") || strstr(argv[i], "-q") || strstr(argv[i], "-t") || strstr(argv[i], "-d")) {
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

void socket_init(int *fd_skt, struct sockaddr_in *serv_addr, char *host, int port) {
    if ((*fd_skt = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        termina("Errore creazione socket");
    }

    serv_addr->sin_family = AF_INET;
    // il numero della porta deve essere convertito
    // in network order
    serv_addr->sin_port = htons(port);
    serv_addr->sin_addr.s_addr = inet_addr(host);
}

void socket_connect(int fd_skt, struct sockaddr_in serv_addr) {
    if (connect(fd_skt, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        termina("Errore apertura connessione");
}

void socket_connect();

long calcola_somma(FILE *f) {
    long n;
    long somma = 0;
    int counter = 0;

    while (true) {
        int e = fread(&n, sizeof(long), 1, f);
        if (e != 1) break;

        somma += counter * n;
        counter += 1;
    }

    return somma;
}

int get_lunghezza_numero_come_stringa(long n) {
    int count = 0;

    if (n < 0) count++;

    while (n != 0) {
        n = n / 10;
        count++;
    }

    return count;
}

char *genera_stringa_socket(char *filename, long somma) {
    int dimensione = get_lunghezza_numero_come_stringa(somma) +
                     strlen(filename); //+ 2 perché uno spazio è per il | e uno per lo zero finale
    char *stringa_socket = malloc(dimensione * sizeof(char));

    sprintf(stringa_socket, "%s|%ld", filename, somma);
    return stringa_socket;
}

/* Read "n" bytes from a descriptor */
ssize_t readn(int fd, void *ptr, size_t n) {
    size_t nleft;
    ssize_t nread;

    nleft = n;
    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (nleft == n) return -1; /* error, return -1 */
            else break; /* error, return amount read so far */
        } else if (nread == 0) break; /* EOF */
        nleft -= nread;
        ptr += nread;
    }
    return (n - nleft); /* return >= 0 */
}

/* Write "n" bytes to a descriptor */
ssize_t writen(int fd, void *ptr, size_t n) {
    size_t nleft;
    ssize_t nwritten;

    nleft = n;
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) < 0) {
            if (nleft == n) return -1; /* error, return -1 */
            else break; /* error, return amount written so far */
        } else if (nwritten == 0) break;
        nleft -= nwritten;
        ptr += nwritten;
    }
    return (n - nleft); /* return >= 0 */
}
