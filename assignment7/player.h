#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"

Role mapStringtoRole (const char* currRole);
float calcPerformanceOfPlayer (PlayerNode* currPlayer);

void insertRoleSortedOrder (PlayerNode** head, PlayerNode* newNode);
void insertGlobalSortedOrder (PlayerNode** head, PlayerNode* newNode);
void insertPlayerIntoGlobal (Role currRole, PlayerNode* newPlayer);

char* getStringFromRole (Role currRole);

void displayAllPlayersByRole ();
void releaseMemory ();

#endif
