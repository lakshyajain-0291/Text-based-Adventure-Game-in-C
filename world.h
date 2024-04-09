#include<stdio.h>

#ifndef WORLD_H
#define WORLD_H

typedef struct
{
    Location *curLocation;
    Location *world;
    Player *player;
}World;

// Function to initialize the game world
World* initializeWorld();

// Function to handle player movement between locations
void movePlayer(World *world, Location *destination);

// Function to load game state from file
void loadGameState(World *world, const char *filename);

// Function to save game state to file
void saveGameState(World *world, const char *filename);

// Function to handle global events (e.g., time passing, random events)
void handleGlobalEvents(World *world); //Lite lena abhia

#endif