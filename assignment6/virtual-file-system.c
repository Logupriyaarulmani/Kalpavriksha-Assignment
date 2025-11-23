#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vfs-operations.h"
#include "utils.h"

int main() {
    FileNode *root = NULL;
    FileNode *currentDirectory = NULL;
    FreeBlockList *freeBlockList = NULL;

    initializeFileSystem(&root, &currentDirectory);

    char line[MAX_LINE];
    while (1) {
        printf("/ > ");
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        processCommand(line, &root, &currentDirectory);
    }

    return 0;
}