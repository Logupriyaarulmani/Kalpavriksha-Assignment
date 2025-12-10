#include "userCommand.h"
#include "LRUCache.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void runUserCommandLoop() {
    char command[MAX_VALUE_LENGTH];
    LRUCache* cache = NULL;

    bool isRunning = true;

    while (isRunning) {
        fgets(command, sizeof(command), stdin);
        
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break; 
        }

        switch (command[0]){
            case 'c':
                if (strcmp(command, "createCache") == 0)
                {
                    int newCapacity;
                    scanf("%d", &newCapacity);
                    createCacheTable(newCapacity);
                }
                else
                {
                    printf("Enter Valid Command!\n");
                }
                break;
            
            case 'p':
                if (strcmp(command, "put") == 0)
                {
                    int newKey;
                    char value[100];
                    scanf("%d", &newKey);
                    scanf("%s", value);
                    put(newKey, value);
                }
                else
                {
                    printf("Enter Valid Command!\n");
                }
                break;

            case 'g':
                if (strcmp(command, "get") == 0)
                {
                    int currKey;
                    scanf("%d", &currKey);
                    get(currKey);
                }
                else
                {
                    printf("Enter Valid Command!\n");
                }
                break;

            case 'e':
                if (strcmp(command, "exit") == 0)
                {
                    isRunning = 0;
                    break;
                }
                else
                {
                    printf("Enter Valid Command!\n");
                }
                break;

            default:
                printf("Enter Valid Command!\n");
                break;
        }
    }
}
