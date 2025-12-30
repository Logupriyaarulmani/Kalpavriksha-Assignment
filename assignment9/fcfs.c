#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduler.h"

static int processCount = 0;
static int killEventCount = 0;
static int currentTime = 0;
static int terminatedCount = 0;

Queue readyQueue;
Queue waitingQueue;
Queue terminatedQueue;

PCB pcbTable[100];
KillEvent killTable[100];

PCB* runningProcess = NULL;

PCB* getPcb(int pid) {
    return &pcbTable[pid % 100];
}

void removeProcessFromQueue(Queue* queue, int pid) {
    Queue tempQueue;
    initQueue(&tempQueue);

    while (!isQueueEmpty(queue)) {
        int currentPid = dequeue(queue);
        if (currentPid != pid) {
            enqueue(&tempQueue, currentPid);
        }
    }

    while (!isQueueEmpty(&tempQueue)) {
        enqueue(queue, dequeue(&tempQueue));
    }
    freeQueue(&tempQueue);
}

void freeAllQueues() {
    freeQueue(&readyQueue);
    freeQueue(&waitingQueue);
    freeQueue(&terminatedQueue);
}

void loadProcessData() {
    printf("Enter Number of Processes: ");
    scanf("%d", &processCount);

    for (int i = 0; i < processCount; i++) {
        char name[MAX_LEN];
        int pid, burst, ioStart, ioDur;

        scanf("%s %d %d %d %d", name, &pid, &burst, &ioStart, &ioDur);

        PCB* pcb = getPcb(pid);
        strcpy(pcb->processName, name);
        pcb->pid = pid;
        pcb->cpuBurst = burst;
        pcb->ioStartTime = ioStart;
        pcb->ioDuration = ioDur;
        pcb->executedCpu = 0;
        pcb->remainingIo = ioDur;
        pcb->turnaroundTime = 0;
        pcb->waitingTime = 0;
        pcb->killTime = 0;
        pcb->state = READY;
        pcb->arrivalTime = 0;

        enqueue(&readyQueue, pid);
    }
}

void loadKillEvents() {
    printf("Enter Number of Kill Events: ");
    scanf("%d", &killEventCount);

    for (int i = 0; i < killEventCount; i++) {
        scanf("%s %d %d", killTable[i].command, &killTable[i].pid, &killTable[i].killTime);
    }
}

void processKillEvents() {
    for (int i = 0; i < killEventCount; i++) {
        if (killTable[i].killTime == currentTime) {
            PCB* target = getPcb(killTable[i].pid);
            
            target->killTime = currentTime;
            target->turnaroundTime = 0;
            target->waitingTime = 0;
            target->state = KILLED;

            if (target == runningProcess) {
                runningProcess = NULL;
            }
            
            removeProcessFromQueue(&readyQueue, target->pid);
            removeProcessFromQueue(&waitingQueue, target->pid);
            
            enqueue(&terminatedQueue, target->pid);
            terminatedCount++;
        }
    }
}

void runCpuCycle() {
    if (runningProcess == NULL && !isQueueEmpty(&readyQueue)) {
        runningProcess = getPcb(dequeue(&readyQueue));
        runningProcess->state = RUNNING;
    }

    if (runningProcess == NULL) return;

    runningProcess->executedCpu++;

    if (runningProcess->executedCpu == runningProcess->ioStartTime && 
        runningProcess->ioDuration > 0) {
        runningProcess->state = WAITING;
        enqueue(&waitingQueue, runningProcess->pid);
        runningProcess = NULL;
        return;
    }

    if (runningProcess->executedCpu == runningProcess->cpuBurst) {
        runningProcess->state = TERMINATED;
        runningProcess->turnaroundTime = currentTime + 1 - runningProcess->arrivalTime;
        enqueue(&terminatedQueue, runningProcess->pid);
        runningProcess = NULL;
        terminatedCount++;
    }
}

void runIoCycle() {
    Queue tempQueue;
    initQueue(&tempQueue);

    while (!isQueueEmpty(&waitingQueue)) {
        int pid = dequeue(&waitingQueue);
        PCB* p = getPcb(pid);
        p->remainingIo--;

        if (p->remainingIo <= 0) {
            p->state = READY;
            enqueue(&readyQueue, pid);
        } else {
            enqueue(&tempQueue, pid);
        }
    }

    while (!isQueueEmpty(&tempQueue)) {
        enqueue(&waitingQueue, dequeue(&tempQueue));
    }
    freeQueue(&tempQueue);
}

void trackWaitingTime() {
    if (runningProcess == NULL && !isQueueEmpty(&readyQueue)) {
        Queue tempQueue;
        initQueue(&tempQueue);

        while (!isQueueEmpty(&readyQueue)) {
            int pid = dequeue(&readyQueue);
            PCB* p = getPcb(pid);
            p->waitingTime++;
            enqueue(&tempQueue, pid);
        }

        while (!isQueueEmpty(&tempQueue)) {
            enqueue(&readyQueue, dequeue(&tempQueue));
        }
        freeQueue(&tempQueue);
    }
}

void printResults() {
    printf("\nPID | Name | CPU | IO | Status | Turnaround | Waiting\n");

    Queue tempQueue;
    initQueue(&tempQueue);

    while (!isQueueEmpty(&terminatedQueue)) {
        int pid = dequeue(&terminatedQueue);
        PCB* p = getPcb(pid);

        char status[32];
        if (p->state == KILLED) {
            sprintf(status, "KILLED at %d", p->killTime);
        } else {
            sprintf(status, "OK");
        }

        printf("%-4d %-10s %-4d %-4d %-12s %-11d %d\n",
               p->pid, p->processName, p->cpuBurst, p->ioDuration,
               status, p->turnaroundTime, p->waitingTime);

        enqueue(&tempQueue, pid);
    }

    while (!isQueueEmpty(&tempQueue)) {
        enqueue(&terminatedQueue, dequeue(&tempQueue));
    }
    freeQueue(&tempQueue);
}

int main() {
    initQueue(&readyQueue);
    initQueue(&waitingQueue);
    initQueue(&terminatedQueue);

    loadProcessData();
    loadKillEvents();

    while (terminatedCount < processCount) {
        processKillEvents();
        trackWaitingTime();
        runCpuCycle();
        runIoCycle();
        currentTime++;
    }

    printResults();
    freeAllQueues();
    return 0;
}