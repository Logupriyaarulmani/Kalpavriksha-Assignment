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

void initializeFreeBlockList(void);
int popFreeBlockHead(void);
void pushFreeBlockTail(int index);
int countFreeBlocks(void);
void freeFreeBlockList(void);

FileNode* createFileNode(const char *name, bool isDirectory, FileNode *parent);
void insertChild(FileNode *parent, FileNode *child);
FileNode* findChild(FileNode *parent, const char *name);
int unlinkChildNode(FileNode *parent, FileNode *node);
void freeFileSystem(FileNode *node);

int rightTrim(char *str);
int leftTrim(char *str);
int freeBlockOfFile(FileNode *file);
int is_bad_name(const char *name);

#endif /* LINKEDLIST_BEHAVIOUR_H */
