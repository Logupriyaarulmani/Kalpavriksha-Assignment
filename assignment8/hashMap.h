#ifndef HASHMAP_H
#define HASHMAP_H

#include "queue.h"

Node** hashMapCreate(int capacity);
int hashMapGetIndex(int key, int capacity);
Node* hashMapGet(Node** table, int key, int capacity);
void hashMapPut(Node** table, Node* node, int capacity);
void hashMapRemove(Node** table, int key, int capacity);
void freeHashMap(Node** table, int capacity);

#endif