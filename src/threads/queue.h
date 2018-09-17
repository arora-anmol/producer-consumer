/*
    Header for circular queue implementation, for the buffer
    Taken help from: https://www.programiz.com/dsa/circular-queue

*/


typedef struct queue {
    int size;
    int front;
    int rear;
    int *items;
}Queue ;

extern void enQueue(Queue *buffer, int value);
extern int deQueue(Queue *buffer);
extern int isFull(Queue *buffer);
extern int isEmpty(Queue *buffer);
extern int init_queue(Queue *buffer, int size);
