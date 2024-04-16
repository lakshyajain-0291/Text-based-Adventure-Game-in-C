#include "headers.h"

#ifndef ITEM_H
#define ITEM_H

typedef enum
{
    WEAPON,
    ARMOR,
    POTION,
    QUEST_ITEM,
    RING,
    CONSUMABLE,
// ADD MORE AS NEEDED

}Itemtype;

typedef enum {
    NO_BUFF,
    ATT_INC,
    ATT_DEC,
    DEF_INC,
    DEF_DEC,
    HP_INC,
    HP_DEC,
    AGI_INC,
    AGI_DEC,
    STR_INC,
    STR_DEC,
    DEX_INC,
    DEX_DEC,
    INT_INC,
    INT_DEC,
    LUCK_INC,
    LUCK_DEC,
    // Add more buffs and debuffs as needed
} TypeOfBuff;

typedef struct {
    TypeOfBuff effect;
    int duration;
    int valueType; // 0 for direct value change, 1 for percentage change
    int type;      //ACTIVE(0) OR PASSIVE(1)
    int value;
} BUFF;

typedef enum
{
    COMMON,
    UNCOMMON,
    RARE,
    UNIQUE,
    LEGENDARY,
    MYTHICAL,
// ADD MORE AS NEEDED
}Grade;

typedef struct 
{
    char *name;  //Use strdup
    char *description;
    int durability;
    Grade grade;
    BUFF buff;
    Itemtype type;
}Item;

// Function prototypes

void createItem(Item *item,const char name[],const char desc[],Grade grade,Itemtype itemtype,TypeOfBuff effect,int duration,int valueType,int type,int value);
void modifyItem(Item *item,const char name[],const char desc[],Grade grade,Itemtype itemtype,TypeOfBuff effect,int duration,int valueType,int type,int value);

void inspectItem(Item item);//generates item data
void useItem(Player *player,Item *item);
void destroyItem(Item *item);

// void serializeItem(Item item, FILE *file);
// void deserializeItem(Item *item, FILE *file);

#endif