#ifndef QUEST_H
#define QUEST_H
#include "headers.h"

typedef struct
{
    int xp;
    int gold;
    BUFF buff;
}Reward;

typedef struct 
{
    char **activeQuests; //array of active quests 
    char **completedQuests;//array of completed quests
}QuestManager;

#endif