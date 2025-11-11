#include "linkedList-behaviour.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static FreeBlock *freeBlockListHead = NULL;
static FreeBlock *freeBlockListTail = NULL;
static int freeBlockCount = 0;

void initializeFreeBlockList(void)
{
    freeBlockListHead = freeBlockListTail = NULL;
    freeBlockCount = 0;
    for (int index = 0; index < NUMBER_OF_BLOCKS; ++index) {
        FreeBlock *newBlock = (FreeBlock*)malloc(sizeof(FreeBlock));
        if (!newBlock) { perror("malloc"); exit(1); }
        newBlock->index = index;
        newBlock->next = NULL;
        newBlock->prev = freeBlockListTail;

        if (!freeBlockListHead) freeBlockListHead = newBlock;
        if (freeBlockListTail) freeBlockListTail->next = newBlock;
        freeBlockListTail = newBlock;
        freeBlockCount++;
    }
}

int popFreeBlockHead(void)
{
    if (freeBlockListHead == NULL) return -1;
    int index = freeBlockListHead->index;
    FreeBlock *node = freeBlockListHead;
    freeBlockListHead = freeBlockListHead->next;
    if (freeBlockListHead) freeBlockListHead->prev = NULL;
    else freeBlockListTail = NULL;
    free(node);
    freeBlockCount--;
    return index;
}

void pushFreeBlockTail(int index)
{
    if (index < 0 || index >= NUMBER_OF_BLOCKS) return;
    FreeBlock *newBlock = (FreeBlock*)malloc(sizeof(FreeBlock));
    if (!newBlock) { perror("malloc"); exit(1); }
    newBlock->index = index;
    newBlock->next = NULL;
    newBlock->prev = freeBlockListTail;
    if (!freeBlockListHead) freeBlockListHead = newBlock;
    if (freeBlockListTail) freeBlockListTail->next = newBlock;
    freeBlockListTail = newBlock;
    freeBlockCount++;
}

int countFreeBlocks(void)
{
    return freeBlockCount;
}

void freeFreeBlockList(void)
{
    while (freeBlockListHead) {
        FreeBlock *n = freeBlockListHead;
        freeBlockListHead = freeBlockListHead->next;
        free(n);
    }
    freeBlockListTail = NULL;
    freeBlockCount = 0;
}

int is_bad_name(const char *name)
{
    if (!name || !(*name)) return 1;
    if (!strcmp(name, ".") || !strcmp(name, "..")) return 1;
    if (strlen(name) >= MAX_NAME) return 1;
    return 0;
}

FileNode* createFileNode(const char *name, bool isDirectory, FileNode *parent)
{
    FileNode *newNode = (FileNode*)malloc(sizeof(FileNode));
    if (!newNode) { perror("malloc"); exit(1); }
    memset(newNode, 0, sizeof(*newNode));
    strncpy(newNode->name, name ? name : "", MAX_NAME - 1);
    newNode->name[MAX_NAME - 1] = '\0';
    newNode->isDirectory = true;
    newNode->next = newNode->prev = NULL;
    newNode->parent = parent;
    newNode->child = NULL;
    newNode->blockPointers = NULL;
    newNode->dynamicBlocksCount = 0;
    newNode->fileSize = 0;
    return newNode;
}

void insertChild(FileNode *parent, FileNode *child)
{
    if (!parent || !child) return;
    child->parent = parent;
    if (parent->child == NULL) {
        parent->child = child;
        child->next = child->prev = child;
        return;
    }
    FileNode *head = parent->child;
    FileNode *tail = head->prev;
    tail->next = child;
    head->prev = child;
    child->next = head;
    child->prev = tail;
}

FileNode* findChild(FileNode *parent, const char *name)
{
    if (!parent || !parent->child || !name) return NULL;
    FileNode *cur = parent->child;
    do {
        if (strcmp(cur->name, name) == 0) return cur;
        cur = cur->next;
    } while (cur != parent->child);
    return NULL;
}

int unlinkChildNode(FileNode *parent, FileNode *node)
{
    if (!parent || !node || !parent->child) return 0;
    FileNode *cur = parent->child;
    do {
        if (cur == node) {
            if (cur->next == cur) {
                parent->child = NULL;
            } else {
                cur->prev->next = cur->next;
                cur->next->prev = cur->prev;
                if (parent->child == cur) parent->child = cur->next;
            }
            cur->next = cur->prev = NULL;
            cur->parent = NULL;
            return 1;
        }
        cur = cur->next;
    } while (cur != parent->child);
    return 0;
}

int freeBlockOfFile(FileNode *file)
{
    if (!file || file->isDirectory || file->blockPointers == NULL) return 0;
    int freed = 0;
    for (int i = 0; i < file->dynamicBlocksCount; ++i) {
        int b = file->blockPointers[i];
        if (b >= 0) {
            pushFreeBlockTail(b);
            freed++;
        }
    }
    free(file->blockPointers);
    file->blockPointers = NULL;
    file->dynamicBlocksCount = 0;
    file->fileSize = 0;
    return freed;
}

void freeFileSystem(FileNode *node)
{
    if (!node) return;
    
    while (node->isDirectory && node->child) {
        FileNode *c = node->child;
        unlinkChildNode(node, c);
        freeFileSystem(c);
    }
   
    if (!node->isDirectory && node->blockPointers) {
        for (int i = 0; i < node->dynamicBlocksCount; ++i) {
            int b = node->blockPointers[i];
            if (b >= 0) pushFreeBlockTail(b);
        }
        free(node->blockPointers);
        node->blockPointers = NULL;
    }
    free(node);
}

int rightTrim(char *str)
{
    int len = (int)strlen(str);
    int idx = len - 1;
    while (idx >= 0 && (str[idx] == ' ' || str[idx] == '\t' || str[idx] == '\n')) {
        str[idx] = '\0';
        idx--;
    }
    return idx + 1;
}

int leftTrim(char *str)
{
    int len = (int)strlen(str);
    int i = 0;
    while (i < len && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')) i++;
    if (i > 0) memmove(str, str + i, len - i + 1);
    return len - i;
}