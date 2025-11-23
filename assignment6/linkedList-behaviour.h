#ifndef LINKEDLIST_BEHAVIOUR_H
#define LINKEDLIST_BEHAVIOUR_H

#include <stdbool.h>

#define BLOCK_SIZE 512
#define NUMBER_OF_BLOCKS 1024
#define MAX_NAME 50
#define MAX_LINE 256

typedef struct FreeBlock {
    int index;
    struct FreeBlock *next;
    struct FreeBlock *prev;
} FreeBlock;

typedef struct {
    FreeBlock *head;
    FreeBlock *tail;
    int count;
} FreeBlockList;

typedef struct FileNode {
    char name[MAX_NAME];
    bool isDirectory;
    struct FileNode *next;
    struct FileNode *prev;
    struct FileNode *parent;
    struct FileNode *child;
    int *blockPointers;
    int dynamicBlocksCount;
    int fileSize;
} FileNode;

extern FreeBlockList *freeBlockList;

void initializeFreeBlockList(FreeBlockList *list);
int popFreeBlockHead(FreeBlockList *list);
void pushFreeBlockTail(FreeBlockList *list, int index);
int countFreeBlocks(FreeBlockList *list);
void freeFreeBlockList(FreeBlockList *list);

FileNode* createFileNode(const char *name, bool isDirectory, FileNode *parent);
void insertChild(FileNode *parent, FileNode *child);
FileNode* findChild(FileNode *parent, const char *name);
int unlinkChildNode(FileNode *parent, FileNode *node);
void freeFileSystem(FileNode *node, FreeBlockList *list);

#endif