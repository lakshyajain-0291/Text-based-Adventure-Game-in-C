#include"header files/headers.h"

void printRules()
{
    //make a function with time 
    // delay that has rules of the game to print
}

Player* gameInitialize(char *PlayerID)
{
    // asks player wheather to start a new game
    // (warns that load game will be deleted) 
    // or to load game and if the file does not exist 
    // it gives a prompt telling player to start a new game.
    // It also makes a new json file or load json files based on 
    // player choise and returns a player variable
    int input;
    Player *playerPointer;

    printf("\n1----Start a New Game");
    printf("\n2----Load an Old Game");
    printf("Pleade enter a Choise : ");
    scanf("%d",input);

    if (input == 1)
    {
        // Start a new game
        // Create a new player object and initialize its properties
        playerPointer=createNewPlayer(PlayerID);
        savePlayerData(playerPointer);
    }
    else if (input == 2)
    {
        // Load an old game
        playerPointer = loadPlayerData(PlayerID);
        if (playerPointer == NULL)
        {
            printf("No saved game found. Starting a new game...\n");
            playerPointer = createNewPlayer(PlayerID);
            savePlayerData(playerPointer);
        }
        else
        {
            printf("Old game loaded successfully.\n");
        }
    }
    else
    {
        printf("Invalid choice. Starting a new game...\n");
        playerPointer = createNewPlayer(PlayerID);
        savePlayerData(playerPointer);
    }

    return playerPointer;
}

void selectMode(int *state)
{
    printf("\n0----Choose Navigation  Mode");
    printf("\n1----Choose Interaction Mode");
    printf("\n2----Choose Quest       Mode");
    printf("\nChoose a Mode to continue your Journey : ");

    scanf("%d",state);

}

void processState(Player *player,int *state)
{
    if(*state==0)
        navigationMode(player,state);
    else
    if(*state==1)
        interactionMode(player,state);
    else
    if(*state==2)
        questMode(player,state);

}
