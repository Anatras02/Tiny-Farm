#include "xerrori/xerrori.h"
#include "funzioni/funzioni.h"
#include "structs/structs.h"
#include "produttore-consumatore/produttore.h"
#include "produttore-consumatore/consumatore.h"

#define QUI __LINE__,__FILE__

int main(int argc, char *argv[]) {
    // controlla numero argomenti
    if (argc < 2) {
        printf("Uso: %s file [file ...] \n", argv[0]);
        return 1;
    }

    int opt;
    int numero_thread = 4;
    int dimensione_buffer = 8;
    int delay = 0;
    char *default_file_directory = "";

    opterr = 0;

    //Se lo stesso parametro viene passato più volte prende l'ultimo
    //Rispetto al progetto iniziale è stato aggiunto un nuovo parametro -d per definire la directory home da dove i file andranno presi, questo è stato
    //fatto perché in fase di testing sono stati messi i file dentro una cartella separata ed era meglio in questa maniera piuttosto che scrivere tutta
    //la path completa ogni volta.
    while ((opt = getopt(argc, argv, "n:q:t:d:")) != -1) {
        switch (opt) {
            case 'n':
                numero_thread = atoi(optarg);
                break;
            case 'q':
                dimensione_buffer = atoi(optarg);
                break;
            case 't':
                delay = atoi(optarg);
                break;
            case 'd':
                default_file_directory = optarg;
                break;
            default:
                continue; //Se vengono passati altri parametri li salta
        }
    }
    opterr = 1;

    assert(numero_thread > 0);
    assert(dimensione_buffer > 0);
    assert(delay >= 0);

    //Creo il buffer
    char **buffer = malloc(sizeof(long) * dimensione_buffer);

    //creo i semafori che mi serviranno
    sem_t sem_free_slots, sem_data_items;
    xsem_init(&sem_free_slots, 0, dimensione_buffer, QUI);
    xsem_init(&sem_data_items, 0, 0, QUI);

    //creo le variabili condivise
    int cindex = 0;
    int pindex = 0;
    pthread_mutex_t cmutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t pmutex = PTHREAD_MUTEX_INITIALIZER;

    // Dichiaro il thread produttore e i suoi dati
    int numero_file;
    char **file_names = get_nomi_file(argc, argv, &numero_file);

    for (int i = 0; i < numero_file; i++) {
        assert(strlen(file_names[i]) < 256);
        assert(strcmp(file_names[i], "BREAK") != 0);
    }

    pthread_t produttore_thread;
    pdati dati_produttore;
    dati_produttore.buffer = buffer;
    dati_produttore.dimensione_buffer = dimensione_buffer;
    dati_produttore.pindex = &pindex;  // indice nel buffer
    dati_produttore.pmutex = &pmutex;
    dati_produttore.sem_free_slots = &sem_free_slots;
    dati_produttore.sem_data_items = &sem_data_items;
    dati_produttore.nomi_file = file_names;
    dati_produttore.numero_file = numero_file;
    dati_produttore.delay = delay;

    //faccio partire il produttore
    xpthread_create(&produttore_thread, NULL, produttore_body, &dati_produttore, QUI);

    // Setto i consumatori
    pthread_t consumatori[numero_thread];
    cdati dati_consumatore[numero_thread];

    //faccio partire i consumatori
    for (int i = 0; i < numero_thread; i++) {
        dati_consumatore[i].buffer = buffer;
        dati_consumatore[i].dimensione_buffer = dimensione_buffer;
        dati_consumatore[i].cindex = &cindex;
        dati_consumatore[i].cmutex = &cmutex;
        dati_consumatore[i].sem_data_items = &sem_data_items;
        dati_consumatore[i].sem_free_slots = &sem_free_slots;
        dati_consumatore[i].default_file_directory = default_file_directory;

        xpthread_create(&consumatori[i], NULL, consumatore_body, dati_consumatore + i, QUI);
    }

    xpthread_join(produttore_thread, NULL, QUI);

    for (int i = 0; i < numero_thread; i++) {
        xsem_wait(&sem_free_slots, QUI);
        buffer[pindex++ % dimensione_buffer] = "BREAK";
        xsem_post(&sem_data_items, QUI);
    }

    for (int i = 0; i < numero_thread; i++) {
        xpthread_join(consumatori[i], NULL, QUI);
    }


    free(buffer);
    free(file_names);

    return 0;
}