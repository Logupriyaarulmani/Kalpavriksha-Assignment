#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "player.h"
#include "globals.h"

Role mapStringtoRole (const char* currRole)
{
    if (strcmp (currRole, "Batsman") == 0)  return BATSMAN;
    else if (strcmp (currRole, "Bowler") == 0) return BOWLER;
    else return ALL_ROUNDER;
}

float calcPerformanceOfPlayer (PlayerNode* currPlayer)
{
    if (currPlayer->playerRole == BATSMAN)
    {
        return (currPlayer->battingAverage * currPlayer->strikeRate) / 100.0f;
    }
    else if (currPlayer->playerRole == BOWLER)
    {
        return ((currPlayer->wickets * 2.0f) + (100.0f - currPlayer->economyRate));
    }

    return (((currPlayer->battingAverage * currPlayer->strikeRate) / 100.0f) + (currPlayer->wickets * 2.0f));
}

void insertRoleSortedOrder (PlayerNode** head, PlayerNode* newNode)
{
    if (*head == NULL || (*head)->performanceIndex < newNode->performanceIndex)
    {
        newNode->roleNext = *head;
        *head = newNode;
        return;
    }
    PlayerNode* temp = *head;
    while (temp->roleNext != NULL && temp->roleNext->performanceIndex >= newNode->performanceIndex)
    {
        temp = temp->roleNext;
    }
    newNode->roleNext = temp->roleNext;
    temp->roleNext = newNode;
}

void insertGlobalSortedOrder (PlayerNode** head, PlayerNode* newNode)
{
    if (*head == NULL || (*head)->performanceIndex < newNode->performanceIndex)
    {
        newNode->globalNext = *head;
        *head = newNode;
        return;
    }
    PlayerNode* temp = *head;
    while (temp->globalNext != NULL && temp->globalNext->performanceIndex >= newNode->performanceIndex)
    {
        temp = temp->globalNext;
    }
    newNode->globalNext = temp->globalNext;
    temp->globalNext = newNode;
}

void insertPlayerIntoGlobal (Role currRole, PlayerNode* newPlayer)
{
    if (currRole == BATSMAN)
    {
        insertGlobalSortedOrder (&globalBatsman, newPlayer);
    }
    else if (currRole == BOWLER)
    {
        insertGlobalSortedOrder (&globalBowler, newPlayer);
    }
    else if (currRole == ALL_ROUNDER)
    {
        insertGlobalSortedOrder (&globalAllRounder, newPlayer);
    }
}

char* getStringFromRole (Role currRole)
{
    if (currRole == BATSMAN) return "Batsman";
    else if (currRole == BOWLER) return "Bowler";
    return "All-rounder";
}

void displayAllPlayersByRole ()
{
    int currRole;
    printf("\nRole (1-Batsman, 2-Bowler, 3-All-Rounder): ");
    scanf("%d", &currRole);
    if (currRole > 3 || currRole < 1)
    {
        printf("\nEnter Valid Role!");
        return;
    }
    PlayerNode* currPlayer;
    if (currRole == 1) currPlayer = globalBatsman;
    else if (currRole == 2) currPlayer = globalBowler;
    else if (currRole == 3) currPlayer = globalAllRounder;
    printf("\n%s of All Teams: ", getStringFromRole (currRole));
    printf("\n======================================================================================");
    printf("\nID    Name                    Role         Runs  Avg    SR   Wkts   ER   Perf. Index");
    printf("\n======================================================================================");
    while (currPlayer != NULL)
    {
        printf("\n%-5d %-23s %-12s %-5d %-7.1f %-5.1f %-5d %-5.1f %-5.1f", currPlayer->playerId, currPlayer->playerName,
            getStringFromRole(currPlayer->playerRole), currPlayer->totalRuns,
            currPlayer->battingAverage, currPlayer->strikeRate,
            currPlayer->wickets, currPlayer->economyRate,
            currPlayer->performanceIndex);
            currPlayer = currPlayer->globalNext;
    }
}

void releaseMemory ()
{
    PlayerNode* temp = NULL;
    PlayerNode* curr = globalBowler;
    do
    {
        temp = curr->globalNext;
        free(curr);
        curr = temp;
    } while (temp);
    curr = globalBatsman;
    do
    {
        temp = curr->globalNext;
        free(curr);
        curr = temp;
    } while (temp);
    curr = globalAllRounder;
    do
    {
        temp = curr->globalNext;
        free(curr);
        curr = temp;
    } while (temp);
}