#ifndef VFS_OPERATIONS_H
#define VFS_OPERATIONS_H

#include "linkedList-behaviour.h"
#include "utils.h"

void mkdirCommand(FileNode *currentDirectory, const char *name);
void createCommand(FileNode *currentDirectory, const char *name);
void writeCommand(FileNode *currentDirectory, const char *filename, const char *data);
void readCommand(FileNode *currentDirectory, const char *name);
void deleteCommand(FileNode *currentDirectory, const char *name);
void rmdirCommand(FileNode *currentDirectory, const char *name);
void lsCommand(FileNode *currentDirectory);
void cdCommand(FileNode **currentDirectory, const char *name, FileNode *root);
void pwdCommand(FileNode *currentDirectory, FileNode *root);
void dfCommand(FreeBlockList *freeBlockList);
void exitCommand(FileNode *root, FreeBlockList *freeBlockList);

#endif