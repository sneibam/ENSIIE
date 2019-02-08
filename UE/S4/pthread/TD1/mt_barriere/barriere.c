#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct
{
	/* condition et mutex */
	pthread_cond_t cond;
	pthread_mutex_t mutex;

	/* nombre de threads qui est entré dans la barriere */
	int n;

	/* nombre total de threads */
	int m;
}my_barrier_t;



/* Initialise une barrière pour num threads*/
int my_barrier_init(my_barrier_t * barrier, int num)
{
	/* Complété */ 
	barrier->m = num;
	return pthread_mutex_init(&(barrier->mutex), NULL) ||
		pthread_cond_init(&(barrier->cond), NULL);
}

/* Bloque tant que tous les threads ne sont pas arrivés */
int my_barrier_wait(my_barrier_t * barrier)
{
	/* Complété */
	pthread_mutex_lock(&(barrier->mutex));
	++barrier->n;
	if (barrier->n == barrier->m) {
		barrier->n = 0;
		pthread_cond_broadcast(&(barrier->cond));
	} else {
		pthread_cond_wait(&(barrier->cond), &(barrier->mutex));
	}
	pthread_mutex_unlock(&(barrier->mutex));
	return 0;
}


my_barrier_t barrier;

void * run(void * arg)
{
	long rank = (long)arg;
	int i;
	for(i = 0 ; i < 5 ; i++)
	{
		sleep( rand()%5 );
		printf("*Iter %d,Thread %ld* AVANT \n", i, rank);
		my_barrier_wait(&barrier);
		printf("*Iter %d,Thread %ld* APRES \n", i, rank);
	}
	return NULL;
}

#define NUM_THREADS 5

int main(int argc, char ** argv)
{  
	pthread_t threads[NUM_THREADS];
	long i;

	my_barrier_init(&barrier, NUM_THREADS);

	srand(time(0));

	for(i=0; i < NUM_THREADS; ++i)
		pthread_create(&threads[i], NULL, run, (void*)i);

	for(i=0; i < NUM_THREADS; ++i)
		pthread_join(threads[i], NULL);

	return 0;
}
