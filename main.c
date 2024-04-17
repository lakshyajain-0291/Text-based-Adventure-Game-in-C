#include "header files/headers.h"

#define MAX_PLAYER_ID_LENGTH 100
int state;

int main()
{
    char *playerID = (char *)malloc(sizeof(char) * MAX_PLAYER_ID_LENGTH);
    if (playerID == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    Player *player = NULL;

    printRules();

    printf("\nEnter your Unique ID : ");
    if (scanf("%s", playerID) != 1) {
        fprintf(stderr, "Error reading player ID\n");
        free(playerID);
        return EXIT_FAILURE;
    }

    player = gameInitialize(playerID);
    if (player == NULL) {
        fprintf(stderr, "Failed to initialize player\n");
        free(playerID);
        return EXIT_FAILURE;
    }

    printPrologue();

    selectState(&state);//function that can be called from anywhere by player to choose a state

    while(state>=0)
        processState(player,&state);//function that processes the state

    printf("");///EXIT STATEMENT
    // Cleanup
    free(playerID);
    return EXIT_SUCCESS;
}