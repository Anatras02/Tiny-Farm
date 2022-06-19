#include "../xerrori/xerrori.h"

typedef struct {
    int *pindex;  // indice nel buffer
    char **buffer;
    int dimensione_buffer;
    pthread_mutex_t *pmutex;
    sem_t *sem_free_slots;
    sem_t *sem_data_items;
    char **nomi_file;
    int numero_file;
    int delay;
} pdati;

typedef struct {
    int *cindex;  // indice nel buffer
    char **buffer;
    int dimensione_buffer;
    pthread_mutex_t *cmutex;
    sem_t *sem_free_slots;
    sem_t *sem_data_items;
    char *default_file_directory;
} cdati;
