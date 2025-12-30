#include "hashMap.h"
#include <stdlib.h>

Node** hashMapCreate(int capacity) {
    return calloc(capacity, sizeof(Node*));
}

int hashMapGetIndex(int key, int capacity) {
    return key % capacity;
}

Node* hashMapGet(Node** table, int key, int capacity) {
    int index = hashMapGetIndex(key, capacity);
    Node* node = table[index];
    return (node && node->key == key && node->occupied) ? node : NULL;
}

void hashMapPut(Node** table, Node* node, int capacity) {
    int index = hashMapGetIndex(node->key, capacity);
    table[index] = node;
}

void hashMapRemove(Node** table, int key, int capacity) {
    int index = hashMapGetIndex(key, capacity);
    table[index] = NULL;
}

void freeHashMap(Node** table, int capacity) {
    for (int i = 0; i < capacity; i++) {
        if (table[i]) {
            free(table[i]);
        }
    }
    free(table);
}