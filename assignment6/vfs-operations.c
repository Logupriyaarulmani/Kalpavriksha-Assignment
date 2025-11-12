#include "vfs-operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned char virtualDisk[NUMBER_OF_BLOCKS][BLOCK_SIZE];

static FileNode *root = NULL;
static FileNode *currentDirectory = NULL;

static void decodeEscapesInPlace(char *text);
static void extractWriteContent(const char *inputText, char *outputText, int outputSize);

void initializeRootDirectory(void)
{
    root = createFileNode("", 1, NULL);
    root->next = root->prev = root;
    root->child = NULL;
    currentDirectory = root;
}

void initializeFileSystem(void)
{
    initializeFreeBlockList();
    initializeRootDirectory();
    printf("Compact VFS - ready. Type 'exit' to quit.\n");
}


void mkdirCommand(const char *name)
{
    if (name == NULL) {
        printf("mkdir: missing operand\n");
        return;
    }
    if (is_bad_name(name)) {
        printf("mkdir: invalid name\n");
        return;
    }
    if (findChild(currentDirectory, (char*)name) != NULL) {
        printf("mkdir: cannot create directory '%s': File exists\n", name);
        return;
    }
    FileNode *d = createFileNode(name, 1, currentDirectory);
    insertChild(currentDirectory, d);
    printf("Directory '%s' created successfully.\n", name);
}

void createCommand(const char *name)
{
    if (name == NULL) {
        printf("create: missing operand\n");
        return;
    }
    if (is_bad_name(name)) {
        printf("create: invalid name\n");
        return;
    }
    if (findChild(currentDirectory, (char*)name) != NULL) {
        printf("create: cannot create file '%s': File exists\n", name);
        return;
    }
    FileNode *f = createFileNode(name, 0, currentDirectory);
    insertChild(currentDirectory, f);
    printf("File '%s' created successfully.\n", name);
}

static int allocateBlocksForFile(FileNode *file, int blocksNeeded)
{
    if (!file || file->isDirectory || blocksNeeded <= 0) return 0;
    file->blockPointers = (int*)malloc(sizeof(int) * blocksNeeded);
    if (!file->blockPointers) return 0;
    for (int i = 0; i < blocksNeeded; ++i) {
        int idx = popFreeBlockHead();
        if (idx == -1) {
            for (int j = 0; j < i; ++j) pushFreeBlockTail(file->blockPointers[j]);
            free(file->blockPointers);
            file->blockPointers = NULL;
            file->dynamicBlocksCount = 0;
            return 0;
        }
        file->blockPointers[i] = idx;
    }
    file->dynamicBlocksCount = blocksNeeded;
    return blocksNeeded;
}

static void writeAppend(FileNode *file, const char *data)
{
    if (!file || file->isDirectory) { printf("write: target is not a file\n"); return; }
    int addBytes = (int)strlen(data ? data : "");
    if (addBytes == 0) {
        printf("Data written successfully (size=0 bytes).\n");
        return;
    }

    int currentSize = file->fileSize;
    int currentBlocks = currentSize ? ((currentSize + BLOCK_SIZE - 1) / BLOCK_SIZE) : 0;
    int off = currentSize % BLOCK_SIZE;
    int totalSize = currentSize + addBytes;
    int needBlocks = (totalSize + BLOCK_SIZE - 1) / BLOCK_SIZE;
    int extra = needBlocks - currentBlocks;
    if (extra < 0) extra = 0;
    if (extra > countFreeBlocks()) {
        printf("Disk full! Cannot write data.\n");
        return;
    }

    if (file->blockPointers == NULL && needBlocks > 0) {
        file->blockPointers = (int*)malloc(sizeof(int) * needBlocks);
        if (!file->blockPointers) { printf("Memory error.\n"); return; }
        for (int i = 0; i < needBlocks; ++i) file->blockPointers[i] = -1;
        file->dynamicBlocksCount = 0;
    } else if (extra > 0) {
        int *np = (int*)realloc(file->blockPointers, sizeof(int) * needBlocks);
        if (!np) { printf("Memory error.\n"); return; }
        file->blockPointers = np;
        for (int i = file->dynamicBlocksCount; i < needBlocks; ++i) file->blockPointers[i] = -1;
    }

    for (int i = currentBlocks; i < needBlocks; ++i) {
        int idx = popFreeBlockHead();
        if (idx == -1) { printf("Disk full! Cannot write data.\n"); return; }
        file->blockPointers[i] = idx;
    }
    if (needBlocks > file->dynamicBlocksCount) file->dynamicBlocksCount = needBlocks;

    const unsigned char *src = (const unsigned char*)data;
    int remaining = addBytes;

    if (off != 0 && currentBlocks > 0) {
        int index = file->blockPointers[currentBlocks - 1];
        int can = BLOCK_SIZE - off;
        int take = remaining < can ? remaining : can;
        memcpy(virtualDisk[index] + off, src, take);
        src += take;
        remaining -= take;
    }

    for (int i = currentBlocks; remaining > 0 && i < needBlocks; ++i) {
        int index = file->blockPointers[i];
        int take = remaining > BLOCK_SIZE ? BLOCK_SIZE : remaining;
        memcpy(virtualDisk[index], src, take);
        if (take < BLOCK_SIZE) memset(virtualDisk[index] + take, 0, BLOCK_SIZE - take);
        src += take;
        remaining -= take;
    }

    file->fileSize = totalSize;
    printf("Data written successfully (size=%d bytes).\n", addBytes);
}

void writeCommand(const char *filename, const char *data)
{
    if (filename == NULL) {
        printf("write: missing operand\n");
        return;
    }
    if (is_bad_name(filename)) { printf("write: invalid filename\n"); return; }
    FileNode *f = findChild(currentDirectory, (char*)filename);
    if (!f || f->isDirectory) { printf("write: no such file: %s\n", filename); return; }
    writeAppend(f, data ? data : "");
}

void readCommand(const char *name)
{
    if (name == NULL) { printf("read: missing operand\n"); return; }
    FileNode *f = findChild(currentDirectory, (char*)name);
    if (!f || f->isDirectory) { printf("read: no such file: %s\n", name); return; }
    if (f->fileSize == 0) { printf("File '%s' is empty.\n", name); return; }
    int bytesRead = 0;
    for (int i = 0; i < f->dynamicBlocksCount; ++i) {
        int blockIndex = f->blockPointers[i];
        int toRead = (f->fileSize - bytesRead < BLOCK_SIZE) ? (f->fileSize - bytesRead) : BLOCK_SIZE;
        fwrite(virtualDisk[blockIndex], 1, toRead, stdout);
        bytesRead += toRead;
        if (bytesRead >= f->fileSize) break;
    }
    printf("\n");
}

void deleteCommand(const char *name)
{
    if (name == NULL) { printf("delete: missing operand\n"); return; }
    FileNode *f = findChild(currentDirectory, (char*)name);
    if (!f || f->isDirectory) { printf("delete: no such file: %s\n", name); return; }
    freeBlockOfFile(f);
    unlinkChildNode(currentDirectory, f);
    free(f);
    printf("File deleted successfully.\n");
}

void rmdirCommand(const char *name)
{
    if (name == NULL) { printf("rmdir: missing operand\n"); return; }
    FileNode *d = findChild(currentDirectory, (char*)name);
    if (!d || !d->isDirectory) { printf("rmdir: no such directory: %s\n", name); return; }
    if (d->child != NULL) { printf("rmdir: directory '%s' is not empty\n", name); return; }
    unlinkChildNode(currentDirectory, d);
    free(d);
    printf("Directory removed successfully.\n");
}

void lsCommand(void)
{
    if (!currentDirectory->child) {
        printf("(empty)\n");
        return;
    }
    FileNode *cur = currentDirectory->child;
    do {
        if (cur->isDirectory) printf("%s/\n", cur->name);
        else printf("%s\n", cur->name);
        cur = cur->next;
    } while (cur != currentDirectory->child);
}

void cdCommand(const char *name)
{
    if (name == NULL) { printf("cd: missing operand\n"); return; }
    if (strcmp(name, "..") == 0) {
        if (currentDirectory->parent != NULL) {
            currentDirectory = currentDirectory->parent;
        }
        printf("Moved to %s\n", (currentDirectory == root) ? "/" : ("/") ); 
        return;
    }
    FileNode *t = findChild(currentDirectory, (char*)name);
    if (!t || !t->isDirectory) { printf("cd: no such directory: %s\n", name); return; }
    currentDirectory = t;
    if (currentDirectory == root) {
        printf("Moved to /\n");
    } else {
        char path[MAX_LINE] = "";
        FileNode *tmp = currentDirectory;
        while (tmp && tmp != root) {
            char tmpbuf[MAX_LINE];
            snprintf(tmpbuf, sizeof(tmpbuf), "/%s%s", tmp->name, path);
            strncpy(path, tmpbuf, sizeof(path)-1);
            path[sizeof(path)-1] = '\0';
            tmp = tmp->parent;
        }
        printf("Moved to %s\n", path[0] ? path : "/");
    }
}

void pwdCommand(void)
{
    if (currentDirectory == root) { printf("/\n"); return; }
    char path[MAX_LINE] = "";
    FileNode *tmp = currentDirectory;
    while (tmp && tmp != root) {
        char t[MAX_LINE];
        snprintf(t, sizeof(t), "/%s%s", tmp->name, path);
        strncpy(path, t, sizeof(path)-1);
        path[sizeof(path)-1] = '\0';
        tmp = tmp->parent;
    }
    printf("%s\n", path[0] ? path : "/");
}

void dfCommand(void)
{
    int freeBlocks = countFreeBlocks();
    int usedBlocks = NUMBER_OF_BLOCKS - freeBlocks;
    double usagePercent = ((double)usedBlocks / (double)NUMBER_OF_BLOCKS) * 100.0;
    printf("Total Blocks: %d\n", NUMBER_OF_BLOCKS);
    printf("Used Blocks: %d\n", usedBlocks);
    printf("Free Blocks: %d\n", freeBlocks);
    printf("Disk Usage: %.2f%%\n", usagePercent);
}

void exitCommand(void)
{
    freeFileSystem(root);
    freeFreeBlockList();
    printf("Memory released. Exiting program...\n");
    exit(0);
}

static void decodeEscapesInPlace(char *text)
{
    char *r = text, *w = text;
    while (*r) {
        if (*r == '\\') {
            r++;
            if (*r == 'n') { *w++ = '\n'; r++; }
            else if (*r == 't') { *w++ = '\t'; r++; }
            else if (*r == 'r') { *w++ = '\r'; r++; }
            else if (*r == '\"') { *w++ = '\"'; r++; }
            else if (*r == '\\') { *w++ = '\\'; r++; }
            else {
                *w++ = '\\';
                if (*r) *w++ = *r++;
            }
        } else {
            *w++ = *r++;
        }
    }
    *w = '\0';
}

static void extractWriteContent(const char *inputText, char *outputText, int outputSize)
{
    while (*inputText == ' ' || *inputText == '\t') inputText++;
    if (*inputText == '\"') {
        inputText++;
        int outi = 0;
        while (*inputText && *inputText != '\"' && outi < outputSize - 1) {
            if (*inputText == '\\' && inputText[1] != '\0') {
                outputText[outi++] = *inputText++;
                if (outi < outputSize - 1) outputText[outi++] = *inputText++;
            } else {
                outputText[outi++] = *inputText++;
            }
        }
        outputText[outi] = '\0';
        decodeEscapesInPlace(outputText);
    } else {
        strncpy(outputText, inputText, outputSize - 1);
        outputText[outputSize - 1] = '\0';
        decodeEscapesInPlace(outputText);
    }
}

void processCommand(char *line)
{
    if (!line) return;
    rightTrim(line);
    char *cmd = strtok(line, " ");
    if (!cmd) return;

    if (strcmp(cmd, "mkdir") == 0) {
        char *arg = strtok(NULL, " \t\n");
        mkdirCommand(arg);
    } else if (strcmp(cmd, "ls") == 0) {
        lsCommand();
    } else if (strcmp(cmd, "cd") == 0) {
        char *arg = strtok(NULL, " \t\n");
        if (arg) cdCommand(arg); else printf("cd: missing operand\n");
    } else if (strcmp(cmd, "create") == 0) {
        char *arg = strtok(NULL, " \t\n");
        createCommand(arg);
    } else if (strcmp(cmd, "write") == 0) {
        char *filename = strtok(NULL, " \t\n");
        char *data = strtok(NULL, "\n");
        if (filename && data) {
            char content[MAX_LINE];
            extractWriteContent(data, content, sizeof(content));
            writeCommand(filename, content);
        } else {
            printf("write: missing operand\n");
        }
    } else if (strcmp(cmd, "read") == 0) {
        char *arg = strtok(NULL, " \t\n");
        readCommand(arg);
    } else if (strcmp(cmd, "delete") == 0) {
        char *arg = strtok(NULL, " \t\n");
        deleteCommand(arg);
    } else if (strcmp(cmd, "rmdir") == 0) {
        char *arg = strtok(NULL, " \t\n");
        rmdirCommand(arg);
    } else if (strcmp(cmd, "pwd") == 0) {
        pwdCommand();
    } else if (strcmp(cmd, "df") == 0) {
        dfCommand();
    } else if (strcmp(cmd, "exit") == 0) {
        exitCommand();
    } else {
        printf("Invalid command!\n");
    }
}