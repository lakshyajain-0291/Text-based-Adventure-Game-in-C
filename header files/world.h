#ifndef WORLD_H
#define WORLD_H
#include "headers.h"

void printRules();

Player* gameInitialize(char *PlayerID);

void selectState(int *state);

void processState(Player *player,int *state);
#endif