#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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

void enqueueProcess(Queue* queue, PCB* process) {
    enqueue(queue, process->pid);
}

PCB* dequeueProcess(Queue* queue) {
    int pid = dequeue(queue);
    if (pid == -1) return NULL;
    return getPcb(pid);
}

void removeProcessFromQueue(Queue* queue, int pid) {
    Queue tempQueue;
    initQueue(&tempQueue);

    int removed = 0;

    while (!isQueueEmpty(queue)) {
        int currentPid = dequeue(queue);
        if (currentPid == pid) {
            removed = 1;
            continue;
        }
        enqueue(&tempQueue, currentPid);
    }

    while (!isQueueEmpty(&tempQueue)) {
        enqueue(queue, dequeue(&tempQueue));
    }
}

void loadProcessData() {
    printf("Enter Number of Processes: ");
    scanf("%d", &processCount);

    for (int i = 0; i < processCount; i++) {
        char name[MAX_LEN];
        int pid, burst, ioStart, ioDur;

        scanf("%s", name);
        scanf("%d %d %d %d", &pid, &burst, &ioStart, &ioDur);

        PCB* pcb = getPcb(pid);
        strcpy(pcb->processName, name);
        pcb->pid = pid;
        pcb->cpuBurst = burst;
        pcb->remainingIo = ioDur;
        pcb->executedCpu = 0;
        pcb->ioStartTime = ioStart;
        pcb->ioDuration = ioDur;
        pcb->turnaroundTime = 0;
        pcb->waitingTime = 0;
        pcb->state = READY;

        enqueueProcess(&readyQueue, pcb);
    }
}

void loadKillEvents() {
    printf("Enter Number of Kill Events: ");
    scanf("%d", &killEventCount);

    for (int i = 0; i < killEventCount; i++) {
        char tmp[10];
        int pid, timestamp;

        scanf("%s", tmp);
        scanf("%d %d", &pid, &timestamp);

        killTable[i].pid = pid;
        killTable[i].killTime = timestamp;
    }
}

void processKillEvents() {
    for (int i = 0; i < killEventCount; i++) {
        if (killTable[i].killTime != currentTime) continue;

        PCB* target = getPcb(killTable[i].pid);

        target->waitingTime = 0;
        target->turnaroundTime = 0;

        int killTime;
        target->killTime = killTable[i].killTime;

        target->ioStartTime = killTable[i].killTime;

        if (target == runningProcess) {
            target->state = KILLED;
            enqueueProcess(&terminatedQueue, target);
            runningProcess = NULL;
            terminatedCount++;
            continue;
        }

        if (target->state == READY) {
            removeProcessFromQueue(&readyQueue, target->pid);
            target->state = KILLED;
            enqueueProcess(&terminatedQueue, target);
            terminatedCount++;
            continue;
        }

        if (target->state == WAITING) {
            removeProcessFromQueue(&waitingQueue, target->pid);
            target->state = KILLED;
            enqueueProcess(&terminatedQueue, target);
            terminatedCount++;
            continue;
        }
    }
}

void runCpuCycle() {
    if (runningProcess == NULL) {
        if (!isQueueEmpty(&readyQueue)) {
            runningProcess = dequeueProcess(&readyQueue);
            runningProcess->state = RUNNING;
        }
    }

    if (runningProcess == NULL) return;

    runningProcess->executedCpu++;

    if (runningProcess->executedCpu == runningProcess->ioStartTime &&
        runningProcess->remainingIo > 0) {

        runningProcess->state = WAITING;
        enqueueProcess(&waitingQueue, runningProcess);
        runningProcess = NULL;
        return;
    }

    if (runningProcess->executedCpu == runningProcess->cpuBurst) {
        runningProcess->state = TERMINATED;
        runningProcess->turnaroundTime = currentTime + 1;
        enqueueProcess(&terminatedQueue, runningProcess);
        runningProcess = NULL;
        terminatedCount++;
    }
}

void runIoCycle() {
    Queue tempQueue;
    initQueue(&tempQueue);

    while (!isQueueEmpty(&waitingQueue)) {
        PCB* p = dequeueProcess(&waitingQueue);
        p->remainingIo--;

        if (p->remainingIo == 0) {
            p->state = READY;
            enqueueProcess(&readyQueue, p);
        } else {
            enqueueProcess(&tempQueue, p);
        }
    }

    while (!isQueueEmpty(&tempQueue)) {
        enqueueProcess(&waitingQueue, dequeueProcess(&tempQueue));
    }
}

void trackWaitingTime() {
    Queue tempQueue;
    initQueue(&tempQueue);

    while (!isQueueEmpty(&readyQueue)) {
        PCB* p = dequeueProcess(&readyQueue);
        p->waitingTime++;
        enqueueProcess(&tempQueue, p);
    }

    while (!isQueueEmpty(&tempQueue)) {
        enqueueProcess(&readyQueue, dequeueProcess(&tempQueue));
    }
}

void printResults() {
    printf("\nPID | Name | CPU | IO | Status | Turnaround | Waiting\n");

    Queue tempQueue;
    initQueue(&tempQueue);

    while (!isQueueEmpty(&terminatedQueue)) {
        PCB* p = dequeueProcess(&terminatedQueue);

        char status[32];
        if (p->state == KILLED) {
            sprintf(status, "KILLED at %d", p->killTime);
        } else {
            sprintf(status, "OK");
        }

        printf("%-4d %-10s %-4d %-4d %-12s %-11d %d\n",
               p->pid,
               p->processName,
               p->cpuBurst,
               p->ioDuration,
               status,
               p->turnaroundTime,
               p->waitingTime);

        enqueueProcess(&tempQueue, p);
    }

    while (!isQueueEmpty(&tempQueue)) {
        enqueueProcess(&terminatedQueue, dequeueProcess(&tempQueue));
    }
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
    return 0;
}