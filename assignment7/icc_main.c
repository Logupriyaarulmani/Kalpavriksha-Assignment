#include <stdio.h>
#include <stdbool.h>

#include "team.h"
#include "player.h"
#include "globals.h"

void menu ()
{
    printf("\n==============================================================================");
    printf("\nICC ODI Player Performance Analyzer");
    printf("\n==============================================================================");
    printf("\n1. Add Player to Team");
    printf("\n2. Display Players of a Specific Team");
    printf("\n3. Display Teams by Average Batting Strike Rate");
    printf("\n4. Display Top K Players of a Specific Team by Role");
    printf("\n5. Display all Players of Specific Role Across All Teams by Performance Index");
    printf("\n6. Exit");
    printf("\n==============================================================================");
}

int main ()
{
    initializeSystem();
    while (isRunning)
    {
        int choice;
        menu ();
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        switch(choice)
        {
        case 1:
            addNewPlayerToTeam ();
            break;
        case 2:
            printf("\nEnter Team ID: ");
            {
                int searchTeamId;
                scanf("%d", &searchTeamId);
                displayTeamPlayersById (searchTeamId);
            }
            break;
        case 3:
            displayTeamsByAvgStrikeRate ();
            break;
        case 4:
            displayTopKPlayers ();
            break;
        case 5:
            displayAllPlayersByRole ();
            break;
        case 6:
            releaseMemory ();
            printf("Releasing Memory........Exiting Program!");
            isRunning = false;
            break;
        default:
            break;
        }
    }
    return 0;
}