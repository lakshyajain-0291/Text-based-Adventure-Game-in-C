#ifndef PLAYER_H
#define PLAYER_H
#include "headers.h"

// Define the Inventory structure
typedef struct {
    int size; // Size of the inventory
    Item *items; // Array of items in the inventory
} Inventory;

// Define the Player structure
typedef struct {
    char *name; // Player's name
    int level; // Player's level
    Stats stats; // Player's stats (e.g., HP, attack, defense)
    Inventory inventory; // Player's inventory
    int wtdLevel; // Wantedted level
    int xp; // Player's experience points
    int gold; // Player's gold
    char **currentLocation; // Player's current location
    char **activeQuests; // Player's active quests
    QuestManager questManager;//players' active andd completed quests
} Player;

// Function to create a new player
Player *createPlayer(char *ID);

// Function to update player stats based on XP and gold
void updatePlayer(Player *player, int xp, int gold);



// Function to add an item to the player's inventory
void addToInventory(Player *player, Item item);

// Function to update the size of the inventory
void updateInventorySize(Inventory *inventory, int newSize);

// Function to drop an item from the inventory
void dropItem(Inventory *inventory);


// Function to apply a buff to the player's stats
// void applyBUFF(Player *player, BUFF buff);



// Function to display the player's status
void showStatus(Player *player);


#endif
