#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "scheduler.h"

void initQueue(Queue *queue) {
    queue->front = queue->rear = NULL;
}

void enqueue(Queue *queue, int pid) {
    QueueNode *newNode = malloc(sizeof(QueueNode));
    newNode->pid = pid;
    newNode->next = NULL;

    if(queue->rear == NULL) {
        queue->front = queue->rear = newNode;
        return;
    }
    queue->rear->next = newNode;
    queue->rear = newNode;
}

int dequeue(Queue *queue) {
    if(queue->front == NULL) {
        return -1;
    }

    QueueNode *temp = queue->front;
    int pid = temp->pid;

    queue->front = queue->front->next;
    if(queue->front == NULL) queue->rear = NULL;

    free(temp);
    return pid;
}

bool isQueueEmpty(Queue *queue) {
    return queue->front == NULL;
}