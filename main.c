#include "ICS_Project.h"
// #include"cJSON.c"


int state;

int main()
{
    char *playerID = (char *)malloc(sizeof(char) * MAX_PLAYER_ID_LENGTH);
    if (playerID == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    Player *player = NULL;

    // printRules();

    printf("\nEnter your Unique ID : ");
    if (scanf("%s", playerID) != 1)
    {
        fprintf(stderr, "Error reading player ID\n");
        free(playerID);
        return EXIT_FAILURE;
    }

    player = gameInitializer(playerID);
    if (player == NULL)
    {
        fprintf(stderr, "Failed to initialize player\n");
        free(playerID);
        return EXIT_FAILURE;
    }
    printf("\nid-%s",player->id);
    // printf("\nname-%s",player->name);
    // printf("\nlevel-%d",player->level);
    // printf("\nstats,hp-%d",player->stats->HP);
    // printf("\ninventory,items[0]-%s",player->inventory->items[0]);
    // printf("\ninventory,aitems[0]-%d",player->inventory->activeItems[0]);
    // printf("\ngold-%d",player->gold);
    // printf("\ncur_loc-%s",player->currentLocation);
    // printf("\nact_que[0]-%s",player->activeQuests[0]);
    // printf("\nnpcInfo[0][0]-%d",player->NPCInfo[0][0]);
//     printPrologue();

    selectState(&state); // function that can be called from anywhere by player to choose a state
    while (state >= 0)
        processState(player, &state); // function that processes the state

    printf("END OF GAME!"); /// EXIT STATEMENT

//     savePlayerData(player);
//     // Cleanup
//     freePlayer(playerID);
//     return EXIT_SUCCESS;
}
