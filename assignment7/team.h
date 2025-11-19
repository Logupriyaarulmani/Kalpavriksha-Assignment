#ifndef TEAM_H
#define TEAM_H

#include "globals.h"

int findTeamIndexByName(const char* currTeamName);
int binarySearchByTeamId (int searchId);
void reCalcTeamAverage (int teamIndex);
void insertPlayerIntoTeam (int teamIndex, PlayerNode* currPlayer);
void initializeSystem (void);
void displayTeamPlayersById (int teamId);
void displayTeamsByAvgStrikeRate (void);
char* getStringForRole (int currRole);
void addNewPlayerToTeam (void);
void displayTopKPlayers (void);

#endif
