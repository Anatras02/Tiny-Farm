#include "produttore.h"
#include "../structs/structs.h"


#define QUI __LINE__,__FILE__


void *produttore_body(void *arg) {
    pdati *a = (pdati *) arg;

    for (int i = 0; i < a->numero_file; i++) {
        xsem_wait(a->sem_free_slots, QUI);
        xpthread_mutex_lock(a->pmutex, QUI);
        a->buffer[*(a->pindex) % a->dimensione_buffer] = a->nomi_file[i];
        *(a->pindex) += 1;
        xpthread_mutex_unlock(a->pmutex, QUI);
        xsem_post(a->sem_data_items, QUI);
        usleep(a->delay);
    }


    pthread_exit(NULL);
}