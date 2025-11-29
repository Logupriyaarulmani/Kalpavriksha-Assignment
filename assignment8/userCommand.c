#include "userCommand.h"
#include "LRUCache.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_VALUE_LENGTH 100

int toInt(const char* command) {
    int num = 0;
    int i = 0;

    while (command[i] != '\0') {
        if (command[i] < '0' || command[i] > '9') break;
        num = num * 10 + (command[i] - '0');
        i++;
    }
    return num;
}

void runUserCommandLoop() {
    char line[MAX_VALUE_LENGTH];
    LRUCache* cache = NULL;
    bool isRunning = true;

    while (isRunning) {

        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        line[strcspn(line, "\n")] = '\0';

        char* cmd = strtok(line, " ");
        if (cmd == NULL) {
            continue;
        }

        switch (cmd[0]) {

            case 'c':  
                if (strcmp(cmd, "createCache") == 0) {
                    char* capStr = strtok(NULL, " ");

                    if (capStr == NULL) {
                        printf("Enter Valid Command!\n");
                        break;
                    }

                    int capacity = toInt(capStr);
                    if (capacity <= 0 || capacity > MAX_CAPACITY) {
                        printf("Enter Valid Capacity!\n");
                        break;
                    }
                    cache = lruCreate(cache, capacity);
                } 
                else {
                    printf("Enter Valid Command!\n");
                }
                break;

            case 'p':   
                if (strcmp(cmd, "put") == 0) {
                    char* keyStr = strtok(NULL, " ");
                    char* valStr = strtok(NULL, " ");

                    if (keyStr == NULL || valStr == NULL) {
                        printf("Enter Valid Command!\n");
                        break;
                    }

                    int key = toInt(keyStr);
                    lruPut(cache, key, valStr);

                } 
                else {
                    printf("Enter Valid Command!\n");
                }
                break;

            case 'g':  
                if (strcmp(cmd, "get") == 0) {
                    char* keyStr = strtok(NULL, " ");
                    if (keyStr == NULL) {
                        printf("Enter Valid Command!\n");
                        break;
                    }
                    int key = toInt(keyStr);
                    lruGet(cache, key);

                } 
                else {
                    printf("Enter Valid Command!\n");
                }
                break;

            case 'e': 
                if (strcmp(cmd, "exit") == 0) {
                    printf("Exiting Program and Releasing Memory!\n");
                    lrufree(cache);
                    isRunning = false;
                } 
                else {
                    printf("Enter Valid Command!\n");
                }
                break;

            default:
                printf("Enter Valid Command!\n");
                break;
        }
    }
}