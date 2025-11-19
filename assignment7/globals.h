#ifndef GLOBALS_H
#define GLOBALS_H

#include "Players_data.h"
#include <stdbool.h>

typedef enum {
    BATSMAN,
    BOWLER,
    ALL_ROUNDER
} Role;

typedef struct PlayerNode {
    int playerId;
    char playerName[50];
    char teamName[50];
    Role playerRole;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
    struct PlayerNode* teamNext;
    struct PlayerNode* roleNext;
    struct PlayerNode* globalNext;
} PlayerNode;

typedef struct Team {
    int teamId;
    char teamName[50];
    int totalPlayers;
    float avgBattingStrikeRate;
    PlayerNode* head;
    PlayerNode* batsmansHead;
    PlayerNode* bowlersHead;
    PlayerNode* allRoundersHead;
} Team;

extern Team teamArray[10];
extern PlayerNode* globalBatsman;
extern PlayerNode* globalBowler;
extern PlayerNode* globalAllRounder;
extern bool isRunning;

#endif