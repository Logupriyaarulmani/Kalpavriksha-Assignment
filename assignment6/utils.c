#include "utils.h"
#include "linkedList-behaviour.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void initializeRootDirectory(FileNode **root, FileNode **currentDirectory) {
    *root = createFileNode("", true, NULL);
    (*root)->next = (*root)->prev = *root;
    (*root)->child = NULL;
    *currentDirectory = *root;
}

void initializeFileSystem(FileNode **root, FileNode **currentDirectory) {
    initializeFreeBlockList();
    initializeRootDirectory(root, currentDirectory);
    printf("Compact VFS - ready. Type 'exit' to quit.\n");
}

int freeBlockOfFile(FileNode *file) {
    if (!file || file->isDirectory || file->blockPointers == NULL) {
        return 0;
    }

    int freed = 0;
    for (int i = 0; i < file->dynamicBlocksCount; ++i) {
        int blockTobeFreed = file->blockPointers[i];
        if (blockTobeFreed >= 0) {
            pushFreeBlockTail(freeBlockList, blockTobeFreed);
            freed++;
        }
    }

    free(file->blockPointers);
    file->blockPointers = NULL;
    file->dynamicBlocksCount = 0;
    file->fileSize = 0;
    return freed;
}

bool is_bad_name(const char *name) {
    if (!name || !(*name))
        return true;
    if (!strcmp(name, ".") || !strcmp(name, ".."))
        return true;
    if (strlen(name) >= MAX_NAME)
        return true;
    return false;
}

static void decodeEscapesInPlace(char *text) {
    char *readPtr = text, *writePtr = text;
    while (*readPtr) {
        if (*readPtr == '\\') {
            readPtr++;
            if (*readPtr == '\0') {
                *writePtr++ = '\\';
                break;
            }
            if (*readPtr == 'n') *writePtr++ = '\n';
            else if (*readPtr == 't') *writePtr++ = '\t';
            else if (*readPtr == 'r') *writePtr++ = '\r';
            else if (*readPtr == '\"') *writePtr++ = '\"';
            else if (*readPtr == '\\') *writePtr++ = '\\';
            else {
                *writePtr++ = '\\';
                *writePtr++ = *readPtr;
            }
            readPtr++;
        } else
            *writePtr++ = *readPtr++;
    }
    *writePtr = '\0';
}

void extractWriteContent(const char *inputText, char *outputText, int outputSize) {
    while (*inputText == ' ' || *inputText == '\t') {
        inputText++;
    }

    if (*inputText == '\"') {
        inputText++;
        int outi = 0;
        while (*inputText && *inputText != '\"' && outi < outputSize - 1) {
            if (*inputText == '\\' && inputText[1] != '\0') {
                outputText[outi++] = *inputText++;
                if (outi < outputSize - 1)
                    outputText[outi++] = *inputText++;
            } else
                outputText[outi++] = *inputText++;
        }
        outputText[outi] = '\0';
        decodeEscapesInPlace(outputText);
    } 
    else {
        strncpy(outputText, inputText, outputSize - 1);
        outputText[outputSize - 1] = '\0';
        decodeEscapesInPlace(outputText);
    }
}

void processCommand(char *line, FileNode **root, FileNode **currentDirectory) {
    if (!line) return;

    line[strcspn(line, "\r\n")] = '\0';
    char *cmd = strtok(line, " \t\r\n");

    if (!cmd) return;

    if (strcmp(cmd, "mkdir") == 0) {
        char *arg = strtok(NULL, " \t\n");
        mkdirCommand(*currentDirectory, arg);
    } 
    else if (strcmp(cmd, "ls") == 0) {
        lsCommand(*currentDirectory);
    } 
    else if (strcmp(cmd, "cd") == 0) {
        char *arg = strtok(NULL, " \t\n");
        if (arg)
            cdCommand(currentDirectory, arg, *root);
        else
            printf("cd: missing operand\n");
    } 
    else if (strcmp(cmd, "create") == 0) {
        char *arg = strtok(NULL, " \t\n");
        createCommand(*currentDirectory, arg);
    } 
    else if (strcmp(cmd, "write") == 0) {
        char *filename = strtok(NULL, " \t\n");
        char *data = strtok(NULL, "\n");
        if (filename && data) {
            char content[MAX_LINE];
            extractWriteContent(data, content, sizeof(content));
            writeCommand(*currentDirectory, filename, content);
        } else
            printf("write: missing operand\n");
    } 
    else if (strcmp(cmd, "read") == 0) {
        char *arg = strtok(NULL, " \t\n");
        if (arg) {
            readCommand(*currentDirectory, arg);
        } else {
            printf("read: missing operand\n");
        }
}

    else if (strcmp(cmd, "delete") == 0) {
        char *arg = strtok(NULL, " \t\n");
        deleteCommand(*currentDirectory, arg);
    } 
    else if (strcmp(cmd, "rmdir") == 0) {
        char *arg = strtok(NULL, " \t\n");
        rmdirCommand(*currentDirectory, arg);
    } 
    else if (strcmp(cmd, "pwd") == 0) {
        pwdCommand(*currentDirectory, *root);
    } 
    else if (strcmp(cmd, "df") == 0) {
        dfCommand(freeBlockList);
    } 
    else if (strcmp(cmd, "exit") == 0) {
        exitCommand(*root, freeBlockList);
    } 
    else {
        printf("Invalid command!\n");
    }
}