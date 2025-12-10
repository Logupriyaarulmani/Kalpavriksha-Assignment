#include "queue.h"
#include <stdlib.h>

Queue* queueCreate() {
    Queue* q = malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

bool queueIsEmpty(Queue* queue) {
    return queue->front == NULL;
}

void queueInsertRear(Queue* queue, Node* node) {
    node->next = NULL;
    node->prev = queue->rear;

    if (queue->rear)
        queue->rear->next = node;

    queue->rear = node;

    if (queue->front == NULL)
        queue->front = node;
}

void queueRemoveNode(Queue* queue, Node* node) {
    if (node == queue->front)
        queue->front = node->next;

    if (node == queue->rear)
        queue->rear = node->prev;

    if (node->prev)
        node->prev->next = node->next;

    if (node->next)
        node->next->prev = node->prev;

    node->next = node->prev = NULL;
}

Node* queueGetLRU(Queue* queue) {
    return queue->front;  
}