#include <sys/socket.h>

char **get_nomi_file(int, char **, int *);

int socket_init(int *, struct sockaddr_in *, char *host, int port);

void socket_connect(int, struct sockaddr_in);

void socket_connect();

long calcola_somma(FILE *);

int get_lunghezza_numero_come_stringa(long);

char *genera_stringa_socket(char *, long);

ssize_t readn(int, void *, size_t);

ssize_t writen(int, void *, size_t);

