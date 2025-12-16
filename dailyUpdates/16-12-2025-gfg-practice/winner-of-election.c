#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h> 

#define TABLE_SIZE 100
#define DELETED_NODE (Person *)(0xFFFFFFFFFFFFFFFFUL)

typedef struct {
    char name[256];
    int count;
} Person;

Person *hash_table[TABLE_SIZE];

bool insert(Person *p) {
    if (p == NULL) return false;

    int index = hash(p->name, TABLE_SIZE);
    int original_index = index;

    do {
        if (hash_table[index] == NULL || hash_table[index] == DELETED_NODE) {
            hash_table[index] = p;
            return true;
        }

        index = (index + 1) % TABLE_SIZE;
    } while (index != original_index);

    return false;  // Table is full
}

int hash(const char *key) {
    unsigned int hash_value = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hash_value = (hash_value * 31 + key[i]) % TABLE_SIZE;
    }
    return hash_value;
}

Person* get(char *name) {
    int index = hash(name);
    Person *temp = hash_table[index];
    
    while (temp) {
        if (strcmp(temp->name, name) == 0) {
            return temp;
        }
    }
    return NULL;
}
char **winner(char **arr, int n){
    // code here
    for (int i = 0; i < n; i++) {
        Person *p1 = get(arr[i]);
        if (p1 == NULL){
            Person *p = malloc(sizeof(Person));
            strcpy(p->name, arr[i]);
            p->count = 1;
            insert(p);
        }
        else {
            p1->count++;
            }
        }
    int maxVote = 0;
    char *winnerName = NULL;
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hash_table[i] != NULL && hash_table[i] != DELETED_NODE) {
            if (hash_table[i]->count > maxVote) {
                maxVote = hash_table[i]->count;
                winnerName = hash_table[i]->name;
            }
            else if (hash_table[i]->count == maxVote) {
                if (strcmp(hash_table[i]->name, winnerName) < 0) {
                    winnerName = hash_table[i]->name;
                }
            }
        }
    }
    
    char **result = (char**)malloc(2 * (sizeof(char *)));
    result[0] = (char * )malloc (strlen(winnerName) + 1);
    strcpy(result[0], winnerName);
    result[1] = (char*)malloc(12);
    sprintf(result[1], "%d", maxVote);
    
    return result;
    
};
