#ifndef QUEST_H
#define QUEST_H

typedef enum
{
    ONE_TIME_QUEST,
    CHAIN_QUEST,
    DAILY_QUEST,
    COMPULSORY_QUEST,
}TypeOfQuest;

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
    bool status;
    bool isactive;
    TypeOfQuest type;
    struct _quest *next;
}Quest;

// Function to create a new quest
Quest *createQuest(char *name,char *desc,int xp,int gold,TypeOfQuest type);

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