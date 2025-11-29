#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "team.h"
#include "player.h"
#include "globals.h"

int binarySearchByTeamName (const char* searchName)
{
    int low = 0, high = teamCount - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        int cmp = strcmp(teamArray[mid].teamName, searchName);
        if (cmp == 0)
        {
            return mid;
        }
        else if (cmp > 0)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    return -1;
}

int binarySearchByTeamId (int searchId)
{
    int low = 0, high = teamCount - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (teamArray[mid].teamId == searchId)
        {
            return mid;
        }
        else if (teamArray[mid].teamId > searchId)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    return -1;
}

void reCalcTeamAverage (int teamIndex)
{
    float avgSum = 0;
    int playerCount = 0;
    PlayerNode* temp = teamArray[teamIndex].head;
    while (temp)
    {
        if (temp->playerRole == BATSMAN || temp->playerRole == ALL_ROUNDER)
        {
            avgSum += temp->strikeRate;
            playerCount++;
        }
        temp = temp->teamNext;
    }
    teamArray[teamIndex].avgBattingStrikeRate = ((playerCount == 0) ? 0 : avgSum / playerCount); 
}

void mergeTeams(Team arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Team* L = malloc(n1 * sizeof(Team));
    Team* R = malloc(n2 * sizeof(Team));

    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }

    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i].avgBattingStrikeRate >= R[j].avgBattingStrikeRate) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSortTeams(Team arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSortTeams(arr, left, mid);
        mergeSortTeams(arr, mid + 1, right);

        mergeTeams(arr, left, mid, right);
    }
}

void insertPlayerIntoTeam (int teamIndex, PlayerNode* currPlayer)
{
    currPlayer->teamNext = teamArray[teamIndex].head;
    teamArray[teamIndex].head = currPlayer;
    teamArray[teamIndex].totalPlayers++;
    if (currPlayer->playerRole == BATSMAN)
    {
        insertRoleSortedOrder(&teamArray[teamIndex].batsmansHead, currPlayer);
    }
    else if (currPlayer->playerRole == BOWLER)
    {
        insertRoleSortedOrder(&teamArray[teamIndex].bowlersHead, currPlayer);
    }
    else if (currPlayer->playerRole == ALL_ROUNDER)
    {
        insertRoleSortedOrder(&teamArray[teamIndex].allRoundersHead, currPlayer);
    }
    reCalcTeamAverage (teamIndex);
}

void initializeSystem ()
{
    for (int index = 0; index < teamCount; index++)
    {
        teamArray[index].teamId = index + 1;
        teamArray[index].batsmansHead = NULL;
        teamArray[index].head = NULL;
        teamArray[index].bowlersHead = NULL;
        teamArray[index].allRoundersHead = NULL;
        strcpy(teamArray[index].teamName, teams[index]);
        teamArray[index].totalPlayers = 0;
        teamArray[index].avgBattingStrikeRate = 0;
    }

    for (int index = 0; index < playerCount; index++)
    {
        int teamIndex = binarySearchByTeamName(players[index].team);
        if (teamIndex > 9 || teamIndex < 0)
        {
            printf("\n No such team found");
            break;
        }
        else {
            PlayerNode* newPlayer = malloc (sizeof(PlayerNode));
            newPlayer->playerId = players[index].id;
            strcpy(newPlayer->playerName, players[index].name);
            strcpy(newPlayer->teamName, players[index].team);
            newPlayer->playerRole = mapStringtoRole (players[index].role);
            newPlayer->battingAverage = players[index].battingAverage;
            newPlayer->economyRate = players[index].economyRate;
            newPlayer->strikeRate = players[index].strikeRate;
            newPlayer->totalRuns = players[index].totalRuns;
            newPlayer->wickets = players[index].wickets;
            newPlayer->teamNext = NULL;
            newPlayer->roleNext = NULL;
            newPlayer->globalNext = NULL;
            newPlayer->performanceIndex = calcPerformanceOfPlayer (newPlayer);
            insertPlayerIntoTeam (teamIndex, newPlayer);
            insertPlayerIntoGlobal (newPlayer->playerRole, newPlayer);
        }
    }
}

void displayTeamPlayersById (int teamId)
{
    int teamIndex = binarySearchByTeamId(teamId);
    if (teamIndex > 9 || teamIndex < 0)
    {
        printf("\nNo such Team Found!");
        return;
    }
    PlayerNode* currPlayer = teamArray[teamIndex].head;
    printf("\nPlayers of Team %s:", teamArray[teamIndex].teamName);
    printf("\n======================================================================================");
    printf("\nID    Name                    Role         Runs  Avg    SR   Wkts   ER   Perf. Index");
    printf("\n======================================================================================");
    while (currPlayer)
    {
        printf("\n%-5d %-23s %-12s %-5d %-7.1f %-5.1f %-5d %-5.1f %-5.1f", 
            currPlayer->playerId, 
            currPlayer->playerName,
            getStringFromRole(currPlayer->playerRole), 
            currPlayer->totalRuns,
            currPlayer->battingAverage, 
            currPlayer->strikeRate,
            currPlayer->wickets, 
            currPlayer->economyRate,
            currPlayer->performanceIndex);
            currPlayer = currPlayer->teamNext;
    }
    printf("\n======================================================================================");
    printf("\nTotal Players: %d", teamArray[teamIndex].totalPlayers);
    printf("\nAverage Batting Strike Rate: %.2f", teamArray[teamIndex].avgBattingStrikeRate);
}

void displayTeamsByAvgStrikeRate ()
{
    Team* sortedTeamsArray = malloc(teamCount * sizeof(Team));
    if (sortedTeamsArray == NULL)
    {
        printf("\nMemory allocation failed!");
        return;
    }

    for (int index = 0; index < teamCount; index++)
    {
        sortedTeamsArray[index] = teamArray[index];
    }

    mergeSortTeams(sortedTeamsArray, 0, teamCount - 1);

    printf("\nTeams Sorted by Average Batting Strike Rate");
    printf("\n=============================================================");
    printf("\nID    Team Name            Avg Bat SR   Total Players");
    printf("\n=============================================================");
    for (int index = 0; index < teamCount; index++)
    {
        printf("\n%-5d %-20s   %-15.1f%-5d", sortedTeamsArray[index].teamId, sortedTeamsArray[index].teamName,
            sortedTeamsArray[index].avgBattingStrikeRate, sortedTeamsArray[index].totalPlayers);
    }
    printf("\n=============================================================");

    free(sortedTeamsArray);
}

char* getStringForRole (int currRole)
{
    if (currRole == 1) return "Batsman";
    else if (currRole == 2) return "Bowler";
    return "All-rounder";
}

void addNewPlayerToTeam ()
{
    int targetTeamId;
    printf("\nEnter Team ID to add player: ");
    scanf("%d", &targetTeamId);
    int teamIndex = binarySearchByTeamId (targetTeamId), currRole;
    if (teamIndex > 9 || teamIndex < 0)
    {
        printf("\nNo Such Team Found!");
        return;
    }
    PlayerNode* newPlayer = malloc (sizeof(PlayerNode));
    strcpy(newPlayer->teamName, teamArray[teamIndex].teamName);
    printf("\nEnter PLayer Details: ");
    printf("\nPlayer ID: ");
    scanf("%d", &(newPlayer->playerId));
    getchar();
    printf("\nName: ");
    fgets(newPlayer->playerName, 50, stdin);
    (newPlayer->playerName)[strcspn(newPlayer->playerName, "\n")] = '\0';
    printf("\nRole (1-Batsman, 2-Bowler, 3-All-Rounder): ");
    scanf("%d", &currRole);
    newPlayer->playerRole = mapStringtoRole (getStringForRole(currRole));
    printf("\nTotal Runs: ");
    scanf("%d", &(newPlayer->totalRuns));
    printf("\nBatting Average: ");
    scanf("%f", &(newPlayer->battingAverage));
    printf("\nStrike Rate: ");
    scanf("%f", &(newPlayer->strikeRate)); 
    printf("\nWickets: ");
    scanf("%d", &(newPlayer->wickets));
    printf("\nEconomy Rate: ");
    scanf("%f", &(newPlayer->economyRate));
    newPlayer->roleNext = NULL;
    newPlayer->teamNext = NULL;
    newPlayer->globalNext = NULL;
    newPlayer->performanceIndex = calcPerformanceOfPlayer (newPlayer);

    insertPlayerIntoTeam (teamIndex, newPlayer);
    insertPlayerIntoGlobal (newPlayer->playerRole, newPlayer);
    printf("\nPlayer Added Successfully to Team %s!", teamArray[teamIndex].teamName);
}

void displayTopKPlayers ()
{
    int currTeamId, currRole, topK;
    printf("\nEnter Team ID: ");
    scanf("%d", &currTeamId);
    int teamIndex = binarySearchByTeamId (currTeamId);
    if (teamIndex > 9 || teamIndex < 0)
    {
        printf("\nNo Such Team Found!");
        return;
    }
    printf("\nRole (1-Batsman, 2-Bowler, 3-All-Rounder): ");
    scanf("%d", &currRole);
    if (currRole > 3 || currRole < 1)
    {
        printf("\nEnter Valid Role!");
        return;
    }
    printf("\nEnter number of players: ");
    scanf("%d", &topK);
    PlayerNode* currPlayer;
    if (currRole == 1) currPlayer = teamArray[teamIndex].batsmansHead;
    else if (currRole == 2) currPlayer = teamArray[teamIndex].bowlersHead;
    else if (currRole == 3) currPlayer = teamArray[teamIndex].allRoundersHead;
    printf("\nTop %d %s of Team %s:", topK, getStringForRole(currRole), teamArray[teamIndex].teamName);
    printf("\n======================================================================================");
    printf("\nID    Name                    Role         Runs  Avg    SR   Wkts   ER   Perf. Index");
    printf("\n======================================================================================");
    while (currPlayer != NULL && topK--)
    {
        printf("\n%-5d %-23s %-12s %-5d %-7.1f %-5.1f %-5d %-5.1f %-5.1f", currPlayer->playerId, currPlayer->playerName,
            getStringFromRole(currPlayer->playerRole), currPlayer->totalRuns,
            currPlayer->battingAverage, currPlayer->strikeRate,
            currPlayer->wickets, currPlayer->economyRate,
            currPlayer->performanceIndex);
            currPlayer = currPlayer->roleNext;
    }
}
