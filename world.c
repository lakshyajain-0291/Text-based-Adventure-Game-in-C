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

    printf("\n1----Start a New Game");
    printf("\n2----Load an Old Game");
    printf("Pleade enter a Choise : ");
    scanf("%d",input);

    if(input==1)
    {
        
    }
    else
    {

    }
}

void selectState(int *state);

void processState(int *state);
