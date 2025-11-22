#ifndef UTILS_H
#define UTILS_H

#include "linkedList-behaviour.h"
#include "vfs-operations.h"

void initializeRootDirectory(void);
void initializeFileSystem(void);
int freeBlockOfFile(FileNode *file);
void freeFileSystem(FileNode *node);
bool is_bad_name(const char *name);
void processCommand(char *line);
void extractWriteContent(const char *input, char *output, int maxSize);

#endif