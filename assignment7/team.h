#ifndef TEAM_H
#define TEAM_H

#include "globals.h"

int binarySearchByTeamName (const char* searchName);
int binarySearchByTeamId (int searchId);
void reCalcTeamAverage (int teamIndex);
void insertPlayerIntoTeam (int teamIndex, PlayerNode* currPlayer);
void initializeSystem (void);
void displayTeamPlayersById (int teamId);
void displayTeamsByAvgStrikeRate (void);
char* getStringForRole (int currRole);
void addNewPlayerToTeam (void);
void displayTopKPlayers (void);

void mergeSortTeams (Team* arr, int left, int right);
void mergeTeams (Team* arr, int left, int mid, int right);

#endif
