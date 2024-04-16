#ifndef QUEST_H
#define QUEST_H
#include "headers.h"

typedef enum
{
    QUEST_LVL,
    QUEST_STATUS,
    RELATONSHIP,
}QuestManager;
typedef struct
{
    int xp;
    int gold;
    BUFF buff;
}Reward;

#endif