#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <mqueue.h>

int main(int argc, char *argv[]){

    int NOT_BLOCKED;
    
    mqd_t qdes;
	char  *qname = NULL;
	mode_t mode = S_IRUSR | S_IWUSR;
	struct mq_attr attr;

    if ( argc !=2 ) {
		printf("Usage: %s <qname>\n", argv[0]);
		printf("The qname must start with a \"/\". \n");
		printf("An example qname: /mailbox1_userid\n");
		exit(1);
	}

    qname = argv[1];

    attr.mq_maxmsg  = QUEUE_SIZE;
	attr.mq_msgsize = sizeof(int);
	attr.mq_flags   = 0;		/* a blocking queue  */

    qdes  = mq_open(qname, O_RDONLY, mode, &attr);
    if (qdes == -1 ) {
		perror("mq_open() failed");
		exit(1);
	}

    while(NOT_BLOCKED) {    // BASICALLY THE EXIT CONDITION
    

    }

    


}