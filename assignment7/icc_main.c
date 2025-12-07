#include <stdio.h>
#include <stdbool.h>

#include "menu.h"
#include "team.h"
#include "player.h"
#include "globals.h"

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