#include "linkedList-behaviour.h"
#include "vfs-operations.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern FreeBlock *freeBlockListHead;
extern FreeBlock *freeBlockListTail;
static int freeBlockCount = 0;

void initializeFreeBlockList() {
    freeBlockListHead = freeBlockListTail = NULL;
    freeBlockCount = 0;
    for (int index = 0; index < NUMBER_OF_BLOCKS; ++index) {
        FreeBlock *newBlock = (FreeBlock*)malloc(sizeof(FreeBlock));
        if (!newBlock) {
            perror("malloc");
            exit(1);
        }
        newBlock->index = index;
        newBlock->next = NULL;
        newBlock->prev = freeBlockListTail;

        if (!freeBlockListHead) {
            freeBlockListHead = newBlock;
        }
        if (freeBlockListTail) {
            freeBlockListTail->next = newBlock;
        }
        freeBlockListTail = newBlock;
        freeBlockCount++;
    }
}

int popFreeBlockHead() {
    if (freeBlockListHead == NULL) {
        return -1;
    }
    int index = freeBlockListHead->index;
    FreeBlock *node = freeBlockListHead;
    freeBlockListHead = freeBlockListHead->next;
    if (freeBlockListHead) {
        freeBlockListHead->prev = NULL;
    } else {
        freeBlockListTail = NULL;
    }
    free(node);
    freeBlockCount--;
    return index;
}

void pushFreeBlockTail(int index) {
    if (index < 0 || index >= NUMBER_OF_BLOCKS) {
        return;
    }
    FreeBlock *newBlock = (FreeBlock*)malloc(sizeof(FreeBlock));
    if (!newBlock) {
        printf("Memory allocation failed.\n");
        return;
    }
    newBlock->index = index;
    newBlock->next = NULL;
    newBlock->prev = freeBlockListTail;
    if (!freeBlockListHead) {
        freeBlockListHead = newBlock;
    }
    if (freeBlockListTail) {
        freeBlockListTail->next = newBlock;
    }
    freeBlockListTail = newBlock;
    freeBlockCount++;
}

int countFreeBlocks() {
    return freeBlockCount;
}

void freeFreeBlockList() {
    while (freeBlockListHead) {
        FreeBlock *n = freeBlockListHead;
        freeBlockListHead = freeBlockListHead->next;
        free(n);
    }
    freeBlockListTail = NULL;
    freeBlockCount = 0;
}

FileNode* createFileNode(const char *name, bool isDirectory, FileNode *parent) {
    FileNode *newNode = (FileNode*)malloc(sizeof(FileNode));
    if (!newNode) {
        perror("malloc");
        exit(1);
    }
    memset(newNode, 0, sizeof(*newNode));
    strncpy(newNode->name, name ? name : "", MAX_NAME - 1);
    newNode->name[MAX_NAME - 1] = '\0';
    newNode->isDirectory = isDirectory;
    newNode->next = newNode->prev = NULL;
    newNode->parent = parent;
    newNode->child = NULL;
    newNode->blockPointers = NULL;
    newNode->dynamicBlocksCount = 0;
    newNode->fileSize = 0;
    return newNode;
}

void insertChild(FileNode *parent, FileNode *newNode) {
    if (!parent->child) {
        parent->child = newNode;
        newNode->next = newNode;
        newNode->prev = newNode;
    } else {
        FileNode *head = parent->child;
        FileNode *tail = head->prev;

        newNode->next = head;
        newNode->prev = tail;

        tail->next = newNode;
        head->prev = newNode;
    }
}


FileNode* findChild(FileNode *parent, const char *name) {
    if (!parent || !parent->child || !name) return NULL;
    FileNode *cur = parent->child;
    do {
        if (strcmp(cur->name, name) == 0) {
            return cur;
        }
        cur = cur->next;
    } while (cur != parent->child);
    return NULL;
}


int unlinkChildNode(FileNode *parent, FileNode *node) {
    if (!parent || !node || !parent->child) return 0;
    FileNode *cur = parent->child;
    do {
        if (cur == node) {
            if (cur->next == cur) {
                parent->child = NULL;
            } else {
                cur->prev->next = cur->next;
                cur->next->prev = cur->prev;
                if (parent->child == cur)
                    parent->child = cur->next;
            }
            cur->next = cur->prev = NULL;
            cur->parent = NULL;
            return 1;
        }
        cur = cur->next;
    } while (cur != parent->child);
    return 0;
}

void freeFileSystem(FileNode *node) {
    if (!node) return;
    while (node->isDirectory && node->child) {
        FileNode *child = node->child;
        unlinkChildNode(node, child);
        freeFileSystem(child);
    }
    if (!node->isDirectory && node->blockPointers) {
        for (int i = 0; i < node->dynamicBlocksCount; i++) {
            int blk = node->blockPointers[i];
            if (blk >= 0) {
                pushFreeBlockTail(blk);
            }
        }
        free(node->blockPointers);
        node->blockPointers = NULL;
    }
    free(node);
}