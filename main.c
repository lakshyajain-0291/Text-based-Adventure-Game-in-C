#include "headers.h"

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

    selectState(&state);//function that can be called from anywhere by player to choose a state
    processState(&state);//function that processes the state

    // Cleanup
    free(playerID);
    return EXIT_SUCCESS;
}