#include "LRUCache.h"
#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

LRUCache* lruCreate(LRUCache* cache, int capacity) {
    cache = malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->size = 0;
    cache->table = hashMapCreate(capacity);
    cache->queue = queueCreate();
    return cache;
}

void lruGet(LRUCache* cache, int key) {
    Node* found = hashMapGet(cache->table, key, cache->capacity);
    if (!found) {
        printf("Memory allocation failed\n");
        return;
    }

    queueRemoveNode(cache->queue, found);
    queueInsertRear(cache->queue, found);
    printf("%s\n", found->value);
}

void lruPut(LRUCache* cache, int key, char* value) {
    Node* existing = hashMapGet(cache->table, key, cache->capacity);
    if (existing) {
        strcpy(existing->value, value);
        queueRemoveNode(cache->queue, existing);
        queueInsertRear(cache->queue, existing);
        return;
    }

    if (cache->size >= cache->capacity) {
        Node* lru = queueGetLRU(cache->queue);
        hashMapRemove(cache->table, lru->key, cache->capacity);
        queueRemoveNode(cache->queue, lru);
        free(lru);
        cache->size--;
    }

    Node* newNode = malloc(sizeof(Node));
    newNode->key = key;
    strcpy(newNode->value, value);
    newNode->occupied = true;

    hashMapPut(cache->table, newNode, cache->capacity);
    queueInsertRear(cache->queue, newNode);
    cache->size++;
}

void lrufree (LRUCache* cache) {
    if (cache == NULL) {
        return;
    }
    freeHashMap(cache->table, cache->capacity);
    freeQueue(cache->queue);
    free(cache);
}