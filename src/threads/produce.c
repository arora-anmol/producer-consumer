// Use this to see if a number has an integer square root
#define EPS 1.E-7


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h> 
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
// #include "queue.h"
#include <semaphore.h>
#include <pthread.h>


double g_time[2];

sem_t spaces;	// Semaphores
sem_t items;
pthread_mutex_t mutex;
pthread_mutex_t printing;	// Mutex for print statements for debugging code
int total_producers;		
int total_items_produced = 0;	
int items_to_produce;
int pindex = 0;			// to create the circular buffer; thanks to Prof Zarnett
int cindex = 0;
int *buffer;
int BUFFER_SIZE;

void *consumer(void *value);	
void *producer(void *value);
int produce(int pid, int *offset);		// to produce number depending on pid and offset

int main(int argc, char *argv[])
{
	int num;
	int maxmsg;
	int num_p;
	int num_c;
	int i;
	int j;
	struct timeval tv;

	if (argc != 5) {
		printf("Usage: %s <N> <B> <P> <C>\n", argv[0]);
		exit(1);
	}

	num = atoi(argv[1]);	/* number of items to produce */
	maxmsg = atoi(argv[2]); /* buffer size                */
	num_p = atoi(argv[3]);  /* number of producers        */
	num_c = atoi(argv[4]);  /* number of consumers        */

	total_producers = num_p;
	items_to_produce = num;

	buffer = (int *) malloc(sizeof(int)* maxmsg);		// Creating the buffer
	BUFFER_SIZE = maxmsg;

  	sem_init( &spaces, 0, maxmsg);						// Init sem/mutex
  	sem_init( &items, 0, 0 );  
  	pthread_mutex_init( &mutex, NULL );
	pthread_mutex_init( &printing, NULL);				// Used for printing, debug code
	//init_queue(buffer, maxmsg);	// not using
	  
	gettimeofday(&tv, NULL);
	g_time[0] = (tv.tv_sec) + tv.tv_usec/1000000.;

	pthread_t consumers[num_c];							// consumer threads
	pthread_t producers[num_p];							// producer threads
	int *pid = (int *) malloc(sizeof(int)* num_p);
	int *cid = (int *) malloc(sizeof(int)* num_c);

	for ( i = 0; i< num_p; i++) {
		pid[i]=i;
		pthread_create(&producers[i], NULL, producer,(void *) &pid[i]);	// Create the producers
	}

	for (j = 0; j < num_c; j++) {
		cid[j] = j;
		pthread_create(&consumers[j], NULL, consumer,(void*) &cid[j]); // Create the consumers 
	}

	for (i = 0 ; i< num_p; i++){					// Joining producer threads
		pthread_join(producers[i], NULL);
	}

	for (j = 0 ; j< num_c; j++){					// Joining consumer threads	
		pthread_join(consumers[j], NULL);
	}

	gettimeofday(&tv, NULL);
	g_time[1] = (tv.tv_sec) + tv.tv_usec/1000000.;

	printf("System execution time: %.6lf seconds\n", \
		g_time[1] - g_time[0]);

	// Code Cleanup: Destroying mutexes and Sems	
	pthread_mutex_destroy(&printing);
	pthread_mutex_destroy(&mutex);
	sem_destroy(&items);
	sem_destroy(&spaces);

	// Code Cleanup: Freeing memory
	free(pid);
	free(cid);
	free(buffer);
	exit(0);
}


void *producer( void* value){
	int item;
	int offset = 0;
	int *pid = (int *)value;
	while(1) {
		item = produce(*pid, &offset);				// Producing item
		sem_wait(&spaces);
		pthread_mutex_lock(&mutex);
		if (items_to_produce == total_items_produced) { // thread exit condition
			pthread_mutex_unlock(&mutex);
			sem_post(&items);
			sem_post(&spaces);						// Releasing sem for producer thread waiting, P > C
			//free(pid);
			pthread_exit(0);

		} 

		buffer[pindex] = item;						// Adding to buffer
    	pindex = (pindex + 1) % BUFFER_SIZE;		// Setting index in buffer
		total_items_produced++;						// incrementing items produced counter
		pthread_mutex_unlock(&mutex);
		sem_post(&items);
		offset++;									// Incrementing offset
	}
}


int produce(int pid, int *offset){

	return ((*offset)* total_producers) + pid;				// Returning produced value
}


void *consumer(void* value){
	int val;												// val takes the value consumed
	int *cid = (int *) value;

	while(1){
		sem_wait(&items);
		pthread_mutex_lock(&mutex);
		if ((buffer[cindex] == -1) && total_items_produced == items_to_produce){	// exit condition; buffer[cindex] = -1 for empty buffer
			pthread_mutex_unlock( &mutex);
			sem_post(&spaces);
			sem_post(&items); 								// to make sure all consumers return
			pthread_exit(0);
		}
		val = buffer[cindex];								//consuming buffer val
    	buffer[cindex] = -1;
    	cindex = (cindex + 1) % BUFFER_SIZE;

		if (sqrt(val) - (int)sqrt(val) < EPS)  				// condition for perfect square
			printf("%d %d %d \n", *cid, val, (int)sqrt(val));

		pthread_mutex_unlock( &mutex);						
		sem_post(&spaces);
	}
}
