# include <stdio.h>
# include <mthread.h>
# include <unistd.h>

/* nombre total de thread */
# define NB_THREADS 	128

/* nombre de thread pouvant entrer dans le semaphore en parallèle */
# define THREAD_PER_SEM	4

mthread_sem_t sem;

static void * run(void * arg) {

    printf("[(sem.value=%d) THREAD %ld] tente de passer le semaphore\n", sem.value, (long)arg);
    fflush(stdout);
    mthread_sem_wait(&sem);
    printf("[(sem.value=%d) THREAD %ld] a passé le semaphore\n", sem.value, (long)arg);
    fflush(stdout);

    usleep(200);

    mthread_sem_post(&sem);
    printf("[(sem.value=%d) THREAD %ld] a libéré le semaphore\n", sem.value, (long)arg);
    fflush(stdout);
    return NULL;
}

int main(int argc, char ** argv) {
    mthread_t threads[NB_THREADS];
    mthread_sem_init(&sem, THREAD_PER_SEM);

    int j;
    for (j = 0 ; j < NB_THREADS ; j++) {
        mthread_create(threads + j, NULL, run, (void *)((long)(j + 1)));
    }

    for (j = 0 ; j < NB_THREADS ; j++) {
        mthread_join(threads[j], NULL);
    }

    printf("Success");

    return 0;
}
