#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

typedef struct Node {
    int key;
    char value[100];
    bool occupied;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct Queue {
    Node* front;  
    Node* rear;    
} Queue;

Queue* queueCreate();
bool queueIsEmpty(Queue* queue);
void queueInsertRear(Queue* queue, Node* node); 
void queueRemoveNode(Queue* queue, Node* node);
Node* queueGetLRU(Queue* queue);

#endif