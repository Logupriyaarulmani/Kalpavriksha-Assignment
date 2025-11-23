#include "linkedList-behaviour.h"
#include "vfs-operations.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

FreeBlockList *freeBlockList = NULL;

FreeBlockList* initializeFreeBlockList() {
    FreeBlockList *list = malloc(sizeof(FreeBlockList));
    if (!list) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    list->head = list->tail = NULL;
    list->count = 0;
    for (int index = 0; index < NUMBER_OF_BLOCKS; index++) {
        FreeBlock *newBlock = malloc(sizeof(FreeBlock));
        if (!newBlock) {
            printf("Memory allocation failed.\n");
            break;
        }
        newBlock->index = index;
        newBlock->next = NULL;
        newBlock->prev = list->tail;

        if (!list->head) {
            list->head = newBlock;
        }
        if (list->tail) {
            list->tail->next = newBlock;
        }
        list->tail = newBlock;
        list->count++;
    }
    return list;
}

int popFreeBlockHead(FreeBlockList *list) {
    if (list->head == NULL) {
        return -1;
    }

    int index = list->head->index;
    FreeBlock *node = list->head;
    list->head = list->head->next;
    if (list->head) {
        list->head->prev = NULL;
    } else {
        list->tail = NULL;
    }

    free(node);
    list->count--;
    return index;
}

void pushFreeBlockTail(FreeBlockList *list, int index) {
    if (index < 0 || index >= NUMBER_OF_BLOCKS) {
        return;
    }

    FreeBlock *newBlock = malloc(sizeof(FreeBlock));
    if (!newBlock) {
        printf("Memory allocation failed.\n");
        return;
    }

    newBlock->index = index;
    newBlock->next = NULL;
    newBlock->prev = list->tail;
    if (!list->head) {
        list->head = newBlock;
    }

    if (list->tail) {
        list->tail->next = newBlock;
    }

    list->tail = newBlock;
    list->count++;
}

int countFreeBlocks(FreeBlockList *list) {
    return list->count;
}

void freeFreeBlockList(FreeBlockList *list) {
    while (list->head) {
        FreeBlock *n = list->head;
        list->head = list->head->next;
        free(n);
    }

    list->tail = NULL;
    list->count = 0;
}

FileNode* createFileNode(const char *name, bool isDirectory, FileNode *parent) {
    FileNode *newNode = malloc(sizeof(FileNode));
    if (!newNode) {
        printf("Memory allocation failed.\n");
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
    } 
    else {
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

void freeFileSystem(FileNode *node, FreeBlockList *list) {
    if (!node) return;

    while (node->isDirectory && node->child) {
        FileNode *child = node->child;
        unlinkChildNode(node, child);
        freeFileSystem(child, list);
    }
    
    if (!node->isDirectory && node->blockPointers) {
        for (int i = 0; i < node->dynamicBlocksCount; i++) {
            int blk = node->blockPointers[i];
            if (blk >= 0) {
                pushFreeBlockTail(list, blk);
            }
        }
        free(node->blockPointers);
        node->blockPointers = NULL;
    }
    free(node);
}