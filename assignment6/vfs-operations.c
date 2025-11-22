#include "vfs-operations.h"
#include "utils.h"
#include "linkedList-behaviour.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char virtualDisk[NUMBER_OF_BLOCKS][BLOCK_SIZE];
int freeBlockOfFile(FileNode *file);

void mkdirCommand(const char *name) {
    if (name == NULL) {
        printf("mkdir: missing operand\n");
        return;
    }

    if (is_bad_name(name)) {
        printf("mkdir: invalid name\n");
        return;
    }

    if (findChild(currentDirectory, name) != NULL) {
        printf("mkdir: cannot create directory '%s': File exists\n", name);
        return;
    }

    FileNode *newDirectory = createFileNode(name, true, currentDirectory);
    insertChild(currentDirectory, newDirectory);
    printf("Directory '%s' created successfully.\n", name);
}

void createCommand(const char *name) {
    if (name == NULL) {
        printf("create: missing operand\n");
        return;
    }
    if (is_bad_name(name)) {
        printf("create: invalid name\n");
        return;
    }
    if (findChild(currentDirectory, name) != NULL) {
        printf("create: cannot create file '%s': File exists\n", name);
        return;
    }
    FileNode *newFileNode = createFileNode(name, false, currentDirectory);
    insertChild(currentDirectory, newFileNode);
    printf("File '%s' created successfully.\n", name);
}

void writeCommand(const char *filename, const char *data) {
    if (!filename) {
        printf("write: missing operand\n");
        return;
    }

    if (is_bad_name(filename)) {
        printf("write: invalid filename\n");
        return;
    }

    FileNode *targetFile = findChild(currentDirectory, filename);
    if (!targetFile || targetFile->isDirectory) {
        printf("write: no such file: %s\n", filename);
        return;
    }

    if (!data) data = "";

    int addBytes = (int)strlen(data);
    int currentSize = targetFile->fileSize;

    int currentBlocks =
        (currentSize > 0) ? ((currentSize + BLOCK_SIZE - 1) / BLOCK_SIZE) : 0;

    int offset = currentSize % BLOCK_SIZE;
    int totalSize = currentSize + addBytes;

    int requiredBlocks = (totalSize + BLOCK_SIZE - 1) / BLOCK_SIZE;
    int extraBlocks = (requiredBlocks > currentBlocks)
                        ? requiredBlocks - currentBlocks
                        : 0;

    if (extraBlocks > countFreeBlocks()) {
        printf("Disk full! Cannot write data.\n");
        return;
    }

    if (!targetFile->blockPointers && requiredBlocks > 0) {
        targetFile->blockPointers = (int *)malloc(sizeof(int) * requiredBlocks);
        if (!targetFile->blockPointers) {
            printf("Memory error\n");
            return;
        }

        for (int i = 0; i < requiredBlocks; i++)
            targetFile->blockPointers[i] = -1;

        targetFile->dynamicBlocksCount = 0;

    } else if (extraBlocks > 0) {
        int *newTable =
            realloc(targetFile->blockPointers, sizeof(int) * requiredBlocks);

        if (!newTable) {
            printf("Memory allocation failed.\n");
            return;
        }

        targetFile->blockPointers = newTable;

        for (int i = targetFile->dynamicBlocksCount; i < requiredBlocks; i++)
            targetFile->blockPointers[i] = -1;
    }

    for (int i = currentBlocks; i < requiredBlocks; i++) {
        int idx = popFreeBlockHead();
        if (idx == -1) {
            printf("Disk full! Cannot write data.\n");
            return;
        }
        targetFile->blockPointers[i] = idx;
    }

    if (requiredBlocks > targetFile->dynamicBlocksCount)
        targetFile->dynamicBlocksCount = requiredBlocks;

    const unsigned char *src = (const unsigned char *)data;
    int remaining = addBytes;

    if (offset != 0 && currentBlocks > 0) {
        int idx = targetFile->blockPointers[currentBlocks - 1];

        int writable = BLOCK_SIZE - offset;
        int toWrite = (remaining < writable) ? remaining : writable;

        memcpy(virtualDisk[idx] + offset, src, toWrite);

        src += toWrite;
        remaining -= toWrite;
    }

    for (int i = currentBlocks; remaining > 0 && i < requiredBlocks; i++) {
        int idx = targetFile->blockPointers[i];
        int toWrite = (remaining > BLOCK_SIZE) ? BLOCK_SIZE : remaining;

        memcpy(virtualDisk[idx], src, toWrite);

        if (toWrite < BLOCK_SIZE)
            memset(virtualDisk[idx] + toWrite, 0, BLOCK_SIZE - toWrite);

        src += toWrite;
        remaining -= toWrite;
    }

    targetFile->fileSize = totalSize;
    printf("Data written successfully (size=%d bytes).\n", addBytes);
}

void readCommand(const char *name) {
    if (!name) {
        printf("read: missing operand\n");
        return;
    }
    FileNode *targetFile = findChild(currentDirectory, name);
    if (!targetFile || targetFile->isDirectory) {
        printf("read: no such file: %s\n", name);
        return;
    }
    if (targetFile->fileSize == 0) {
        printf("File '%s' is empty.\n", name);
        return;
    }
    int bytesRead = 0;
    for (int i = 0; i < targetFile->dynamicBlocksCount; ++i) {
        int blockIndex = targetFile->blockPointers[i];
        int toRead = (targetFile->fileSize - bytesRead < BLOCK_SIZE) ? (targetFile->fileSize - bytesRead) : BLOCK_SIZE;
        fwrite(virtualDisk[blockIndex], 1, toRead, stdout);
        bytesRead += toRead;
        if (bytesRead >= targetFile->fileSize) break;
    }
    printf("\n");
}

void deleteCommand(const char *name) {
    if (!name) {
        printf("delete: missing operand\n");
        return;
    }

    FileNode *fileToDelete = findChild(currentDirectory, name);
    if (!fileToDelete || fileToDelete->isDirectory) {
        printf("delete: no such file: %s\n", name);
        return;
    }

    freeBlockOfFile(fileToDelete);
    unlinkChildNode(currentDirectory, fileToDelete);
    free(fileToDelete);
    printf("File deleted successfully.\n");
}

void rmdirCommand(const char *name) {
    if (!name) {
        printf("rmdir: missing operand\n");
        return;
    }

    FileNode *targetDir = findChild(currentDirectory, name);
    if (!targetDir || !targetDir->isDirectory) {
        printf("rmdir: no such directory: %s\n", name);
        return;
    }

    if (targetDir->child != NULL) {
        printf("rmdir: directory '%s' is not empty\n", name);
        return;
    }

    unlinkChildNode(currentDirectory, targetDir);

    freeFileSystem(targetDir);
    printf("Directory removed successfully.\n");
}

void lsCommand(void) {
    if (!currentDirectory->child) {
        printf("(empty)\n");
        return;
    }

    FileNode *cur = currentDirectory->child;
    do {
        if (cur->isDirectory)
            printf("%s/\n", cur->name);
        else
            printf("%s\n", cur->name);
        cur = cur->next;
    } while (cur != currentDirectory->child);
}

void cdCommand(const char *name) {
    if (!name) {
        printf("cd: missing operand\n");
        return;
    }

    if (strcmp(name, "..") == 0) {
        if (currentDirectory->parent != NULL) {
            currentDirectory = currentDirectory->parent;
        }
        printf("Moved to %s\n", (currentDirectory == root) ? "/" : currentDirectory->name);
        return;
    }

    FileNode *target = findChild(currentDirectory, name);
    if (!target || !target->isDirectory) {
        printf("cd: no such directory: %s\n", name);
        return;
    }

    currentDirectory = target;
    if (currentDirectory == root) {
        printf("Moved to /\n");
    } 
    else {
        char path[MAX_LINE] = "";
        FileNode *tmp = currentDirectory;
        while (tmp && tmp != root) {
            char tmpbuf[MAX_LINE];
            snprintf(tmpbuf, sizeof(tmpbuf), "/%s%s", tmp->name, path);
            strncpy(path, tmpbuf, sizeof(path) - 1);
            path[sizeof(path) - 1] = '\0';
            tmp = tmp->parent;
        }
        printf("Moved to %s\n", path[0] ? path : "/");
    }
}

void pwdCommand(void) {
    if (currentDirectory == root) {
        printf("/\n");
        return;
    }
    
    char path[MAX_LINE] = "";
    FileNode *tmp = currentDirectory;
    while (tmp && tmp != root) {
        char t[MAX_LINE];
        snprintf(t, sizeof(t), "/%s%s", tmp->name, path);
        strncpy(path, t, sizeof(path) - 1);
        path[sizeof(path) - 1] = '\0';
        tmp = tmp->parent;
    }
    printf("%s\n", path[0] ? path : "/");
}

void dfCommand(void) {
    int freeBlocks = countFreeBlocks();
    int usedBlocks = NUMBER_OF_BLOCKS - freeBlocks;
    double usagePercent = ((double)usedBlocks / NUMBER_OF_BLOCKS) * 100.0;
    printf("Total Blocks: %d\n", NUMBER_OF_BLOCKS);
    printf("Used Blocks: %d\n", usedBlocks);
    printf("Free Blocks: %d\n", freeBlocks);
    printf("Disk Usage: %.2f%%\n", usagePercent);
}

void exitCommand(void) {
    freeFileSystem(root);
    freeFreeBlockList();
    printf("Memory released. Exiting program...\n");
    exit(0);
}