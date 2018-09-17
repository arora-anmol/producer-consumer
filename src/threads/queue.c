
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


int init_queue(Queue *buffer, int size) {
    buffer->size = size;
    buffer->front = -1;
    buffer->rear = -1;
    buffer->items = (int*) malloc(sizeof(int) * size);

    return 0;
}


int isFull(Queue *buffer)
{
    if( (buffer->front == buffer->rear + 1) || (buffer->front == 0 && buffer->rear == buffer->size-1))
        return 1;
    return 0;
}

int isEmpty(Queue *buffer) {
    if(buffer->front == -1) return 1;
    return 0;
}

void enQueue( Queue *buffer, int element)
{
    if(isFull(buffer)) 
        printf("\n Queue is full!! \n");
    else {
        if(buffer->front == -1) buffer->front = 0;
        buffer->rear = (buffer->rear + 1) % buffer->size;
        buffer->items[buffer->rear] = element;
        printf("\n Inserted -> %d", element);
    }
}


int deQueue(Queue *buffer)
{
    int element;
    if(isEmpty(buffer)) {
        printf("\n Queue is empty !! \n");
        return(-1);
    } else {
        element = buffer->items[buffer->front];
        if (buffer->front == buffer->rear){
            buffer->front = -1;
            buffer->rear = -1;
        } /* Q has only one element, so we reset the queue after dequeing it. ? */
        else {
            buffer->front = (buffer->front + 1) % buffer->size;
            
        }
        printf("\n Deleted element -> %d \n", element);
        return(element);
    }
}

// int main() {
//     int val = 10;
//     Queue *buffer = (Queue *) malloc(sizeof(Queue));
//     val = init_queue(buffer, val);
//     printf("Successfull  step1: %d \n ", buffer->size);
//     for (int i = 0; i< 11; i++){
//         enQueue(buffer, i);
//         if (isFull(buffer)){
//             printf("The buffer is full");
//             break;
//         }

//     }
//     val = deQueue(buffer);
    
//     // for (int j = 0; j<5;j++){
//     //     val = deQueue(buffer);
//     // }

//     return 0;

// }