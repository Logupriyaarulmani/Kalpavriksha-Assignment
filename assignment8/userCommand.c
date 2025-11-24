#include "userCommand.h"
#include "LRUCache.h"
#include <stdio.h>
#include <string.h>

void runUserCommandLoop() {
    char command[20];
    LRUCache* cache = NULL;

    while (1) {
        scanf("%s", command);

        if (strcmp(command, "createCache") == 0) {
            int cap; scanf("%d", &cap);
            cache = lruCreate(cap);
        }
        else if (strcmp(command, "put") == 0) {
            int key; char value[100];
            scanf("%d %s", &key, value);
            lruPut(cache, key, value);
        }
        else if (strcmp(command, "get") == 0) {
            int key; scanf("%d", &key);
            lruGet(cache, key);
        }
        else if (strcmp(command, "exit") == 0) {
            break;
        }
        else {
            printf("Enter Valid Command!\n");
        }
    }
}