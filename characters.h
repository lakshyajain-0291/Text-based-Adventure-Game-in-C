#include "headers.h"

#ifndef CHARACTERS_H
#define CHARACTERS_H


typedef enum
{
    SOLDIER,
    SHOPKEEPER,
    FIGHTER,
    CITIZEN,
    ///Add more as needed
}TypeOfNpc;

typedef struct 
{
    int HP;
    int att;
    int def;
    int agi;
    int str;
    int dex;
    int inte;
    int luck;
}Stats;

typedef struct 
{
    char* name; //Use strdup to dynamically allocate memopry to these
    char* backstory;
    Stats stats;
    Quest *quest;
}NamedNPC;

typedef struct
{
    TypeOfNpc type;
    Stats stats;
    bool questGiver;
    Quest *quest;
}CommonNPC;

NamedNPC* createNamedNPC(char* name, char* backstory, Stats stats, Quest* quest);
CommonNPC* createCommonNPC(TypeOfNpc type, Stats stats, bool questGiver, Quest* quest);
void destroyNamedNPC(NamedNPC* npc);
void destroyCommonNPC(CommonNPC* npc);

// Function prototypes for random NPC generation and quest assignment
NamedNPC* generateRandomNPC();
Quest* assignRandomQuest(NamedNPC* npc);

// Function prototypes for character-player interactions
void initiateDialogue(Player* player, NamedNPC* npc);
void progressQuest(Player* player, Quest* quest);

#endif