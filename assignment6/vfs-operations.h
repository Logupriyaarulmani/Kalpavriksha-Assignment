#ifndef VFS_OPERATIONS_H
#define VFS_OPERATIONS_H

#include "linkedList-behaviour.h"
#include "utils.h"

extern FileNode *root;
extern FileNode *currentDirectory;

void mkdirCommand(const char *name);
void createCommand(const char *name);
void writeCommand(const char *filename, const char *data);
void readCommand(const char *name);
void deleteCommand(const char *name);
void rmdirCommand(const char *name);
void lsCommand(void);
void cdCommand(const char *name);
void pwdCommand(void);
void dfCommand(void);
void exitCommand(void);

#endif