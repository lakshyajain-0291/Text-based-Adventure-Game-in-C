#ifndef CHARACTERS_H
#define CHARACTERS_H
#include "headers.h"


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



#endif