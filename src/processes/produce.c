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

double g_time[2];
char *MESSAGE_QUEUE_NAME = "/pc";			// Message queue name

void consumer(int consumer_id );
void producer(int producer_id, int total_producers, int nums);

int main(int argc, char *argv[])
{
	int num;
	int maxmsg;
	int num_p;
	int num_c;
	int i;
	struct timeval tv;
	mqd_t qdes;												// Message queue descriptor
	struct mq_attr attr;									// attributes
	int *return_value = NULL;


	if (argc != 5) {
		printf("Usage: %s <N> <B> <P> <C>\n", argv[0]);
		exit(1);
	}

	num = atoi(argv[1]);	/* number of items to produce */
	maxmsg = atoi(argv[2]); /* buffer size                */
	num_p = atoi(argv[3]);  /* number of producers        */
	num_c = atoi(argv[4]);  /* number of consumers        */

    attr.mq_maxmsg = maxmsg;
    attr.mq_msgsize = sizeof(int);
    attr.mq_flags = 0;

	// Opening/ Creating the message queue
	qdes = mq_open(MESSAGE_QUEUE_NAME, (O_CREAT | O_RDWR), (S_IRUSR | S_IWUSR), &attr);		

    if (qdes == -1) {
        printf("Message Queue failed to create  \n");
        return 1;
    }

	pid_t *producer_id = (pid_t *) malloc(sizeof(int)*num_p);	// Initializing producer/consumer ids;
	pid_t *consumer_id = (pid_t *) malloc(sizeof(int)* num_c);


	gettimeofday(&tv, NULL);
	g_time[0] = (tv.tv_sec) + tv.tv_usec/1000000.;

	for (i = 0; i< num_p; i++) {				// Creating producers

		producer_id[i] = fork();

		if (producer_id[i] == 0) {
			producer(i, num_p, num);
			exit(0);

		} else if (producer_id[i]< 0) {
			//printf(" Failed to fork(); Exiting \n ");
			return -1;
		} 
		//printf("Producer ID: %d, Created \n", producer_id[i]);

	}

	for (i = 0; i< num_c; i++) {			// Creating consumers

		consumer_id[i] = fork();

		if (consumer_id[i] == 0) {
			consumer(i);
			exit(0);

		} else if (consumer_id[i]< 0) {
			//printf(" Failed to fork(); Exiting \n ");
			return -1;
		} 
		//printf("Consumer ID: %d, Created \n", consumer_id[i]);
	}

	for (i = 0; i<num_p; i++) {
		waitpid(producer_id[i], return_value, 0);		// Waiting for producer ids
	}

	int kill_message = -1;								

	for (i = 0; i< num_c; i++){							// Adding message for killing consumers.
		if (mq_send(qdes,(char *) &kill_message, sizeof(int), 0) == -1) {
           	printf("Kill message failure in sending \n");
            exit(1);
		//printf("Kill Message Success \n");
		}
	}

	for(i = 0; i<num_c; i++){
		waitpid(consumer_id[i], return_value, 0);		// Waiting for consumer ids
	}

	// Clean up: Closing message queue, from sample code
	if (mq_close(qdes) == -1) {
		printf("mq_close() failed\n");
		exit(2);
	}

	if (mq_unlink(MESSAGE_QUEUE_NAME) != 0) {
		printf("mq_unlink() failed\n");
		exit(3);
	}

	// Clean up: Free memory
	free(producer_id);
	free(consumer_id);

    gettimeofday(&tv, NULL);
    g_time[1] = (tv.tv_sec) + tv.tv_usec/1000000.;

    printf("System execution time: %.6lf seconds\n", \
            g_time[1] - g_time[0]);
	exit(0);
}

void producer(int producer_id, int total_producers, int total_items) {

	int i;
	mqd_t message_queue = mq_open(MESSAGE_QUEUE_NAME, O_RDWR);

    if (message_queue == -1) {
        printf("Producer ID: %d, Message Queue Opening Failure\n", producer_id);
        exit(1);
    }

    for (i = 0; i < total_items; i++) {
        if (i%total_producers == producer_id) {
            if (mq_send(message_queue,(char *) &i, sizeof(int), 0) == -1) {
                printf("Producer ID: %d, Message Send Failure", producer_id);
                exit(1);
            }
        }
    }

	mq_close(message_queue);
}

void consumer(int consumer_id) {

	int val;
	mqd_t message_queue = mq_open(MESSAGE_QUEUE_NAME, O_RDWR);

	if (message_queue == -1) {
		printf("Consumer ID: %d, Message Queue Failure\n", consumer_id);
		exit(1);
	}	

	while(1) {

		if (mq_receive(message_queue, (char *) &val, sizeof(int), NULL) == -1) {
            printf("Consumer ID: %d, Message Receive Failed; EXITINGGG \n", consumer_id);
            exit(1);
        } else {
            
        if (val == -1) {						// QUEUE IS EMPTY
			//printf("Consumer ID: %d, Queue is empty \n", consumer_id);
            break;
        }

		if (sqrt(val) - (int)sqrt(val) < EPS)  // Check for perfect square
			printf("%d %d %d \n", consumer_id, val,(int)sqrt(val));
        }
	}
	mq_close(message_queue);			
}
