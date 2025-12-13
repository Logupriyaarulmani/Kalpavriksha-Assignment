#ifndef SCHEDULER_H
#define SCHEDULER_H

#define MAX_LEN 100

typedef enum {
    READY,
    RUNNING,
    WAITING,
    TERMINATED,
    KILLED
} ProcessState;

typedef struct PCB {
    int pid;
    char processName[MAX_LEN];
    int cpuBurst;
    int ioStartTime;
    int ioDuration;
    int executedCpu;
    int remainingIo;
    int turnaroundTime;
    int waitingTime;
    int killTime;
    ProcessState state;
} PCB;

typedef struct QueueNode {
    int pid;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue {
    QueueNode *front;
    QueueNode *rear;
} Queue;

typedef struct KillEvent {
    int pid;
    int killTime;
} KillEvent;

void initQueue(Queue *queue);
void enqueue(Queue *queue, int pid);
int dequeue(Queue *queue);
bool isQueueEmpty(Queue *queue);

#endif