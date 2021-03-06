/**
 * 1 - Crée un thread qui lance une routine qui termine immédiatement
 * 2 - Initialise un sémaphore
 */

#include <stdio.h>
#include <mthread.h>

static void * run(void * unused) {
    puts("Thread created");
    return NULL;
}

int main(void) {
    mthread_t thrd;
    mthread_create(&thrd, NULL, run, NULL);

    mthread_sem_t sem;
    puts("Initializing semaphore...");
    mthread_sem_init(&sem, 4);
    puts("Done");
    puts("Joining threads");
    mthread_join(thrd, NULL);
    puts("Success");
    return 0;
}

