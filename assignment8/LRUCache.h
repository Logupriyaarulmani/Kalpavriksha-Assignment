#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include "queue.h"

typedef struct LRUCache {
    int capacity;
    int size;
    Node** table;
    Queue* queue;
} LRUCache;

LRUCache* lruCreate(int capacity);
void lruPut(LRUCache* cache, int key, char* value);
void lruGet(LRUCache* cache, int key);

#endif