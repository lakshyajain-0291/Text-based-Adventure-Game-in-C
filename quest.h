#ifndef QUEST_H
#define QUEST_H
#include "headers.h"

typedef enum
{
    ONE_TIME_QUEST,
    CHAIN_QUEST,
    DAILY_QUEST,
    COMPULSORY_QUEST,
}TypeOfQuest;

typedef enum
{   
    ACTIVE,
    COMPLETED,
    FAILED,
    TO_SUBMIT,

}QuestStatus;

typedef struct
{
    int xp;
    int gold;
    BUFF buff;
}Reward;

typedef struct _quest
{
    char *name;
    char *description;
    Reward reward;
    QuestStatus status;
    bool (*completionCond)();//a pointer to function thar returns if the quest is completed or not
    TypeOfQuest type;
    struct _quest *next;
}Quest;

typedef struct 
{
    Quest **activeQuests; //array of active quests 
    Quest **completedQuests;//array of completed quests
}QuestManager;

// Function to create a new quest 
Quest *createQuest(char *name,char *desc,int xp,int gold,TypeOfQuest type,bool(*completionCond)());

// Function to add a quest to a quest list
void   insertQuest(Quest *parentQuest,Quest *childQuest);

// Function to remove a quest from a quest list
void   removeQuest(Quest *parentQuest,Quest *childQuest);

//update the status
void statusUpdate(Quest *quest,bool status);

// Function to check if a quest is completed
bool isQuestCompleted(Quest *quest);

// Function to check if a quest is active
bool isQuestActive(Quest *quest);

// Function to check if a quest is failed
bool isQuestFailed(Quest *quest);



#endif