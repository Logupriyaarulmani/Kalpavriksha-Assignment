#ifndef TEAM_H
#define TEAM_H

#include "globals.h"

void mergeSortTeams(Team arr[], int left, int right);
void mergeTeams(Team arr[], int left, int mid, int right);

int binarySearchTeamByName(const char* currTeamName);
int binarySearchByTeamId (int searchId);
void reCalcTeamAverage (int teamIndex);
void insertPlayerIntoTeam (int teamIndex, PlayerNode* currPlayer);
void initializeSystem ();
void displayTeamPlayersById (int teamId);
void displayTeamsByAvgStrikeRate ();
char* getStringForRole (int currRole);
void addNewPlayerToTeam ();
void displayTopKPlayers ();

#endif