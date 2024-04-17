#ifndef PLAYER_H
#define PLAYER_H
#include "headers.h"

// Define the Inventory structure
typedef struct {
    int size; // Size of the inventory
    char **items; // Array of items in the inventory
} Inventory;

// Define the Player structure
typedef struct {
    char *id; // Player's id
    char *name;
    int level; // Player's level
    Stats *stats; // Player's stats (e.g., HP, attack, defense)
    Inventory *inventory; // Player's inventory
    int wtdLevel; // Wantedted level
    int xp; // Player's experience points
    int gold; // Player's gold
    char *currentLocation; // Player's current location as string
    char **activeQuests; // Player's active quests
    int **npcInfo;//players' active 
} Player;

// Define a structure to hold string-function mappings
typedef struct {
    char *str;
    void (*func)();
} StringFunctionPair;


// Function to create a new player
Player *createNewPlayer(char *playerID);

void savePlayerData(Player *player);

Player *loadPlayerData(char *playerID);

int playMinigame(Player *player,char *gameName);

#endif
