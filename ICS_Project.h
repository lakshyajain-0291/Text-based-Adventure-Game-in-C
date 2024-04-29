#ifndef ICS_PROJECT_H
#define ICS_PROJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cJSON.h"
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include "ICS_Project.h"
#include <math.h>
// #include"cJSON.c"
#include<time.h>
#include <stdarg.h>


#define MAX_PLAYER_ID_LENGTH 100

//Regular text
#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"

//Regular bold text
#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BMAG "\e[1;35m"
#define BCYN "\e[1;36m"
#define BWHT "\e[1;37m"

//Regular underline text
#define UBLK "\e[4;30m"
#define URED "\e[4;31m"
#define UGRN "\e[4;32m"
#define UYEL "\e[4;33m"
#define UBLU "\e[4;34m"
#define UMAG "\e[4;35m"
#define UCYN "\e[4;36m"
#define UWHT "\e[4;37m"

//Regular background
#define BLKB "\e[40m"
#define REDB "\e[41m"
#define GRNB "\e[42m"
#define YELB "\e[43m"
#define BLUB "\e[44m"
#define MAGB "\e[45m"
#define CYNB "\e[46m"
#define WHTB "\e[47m"

//High intensty background 
#define BLKHB "\e[0;100m"
#define REDHB "\e[0;101m"
#define GRNHB "\e[0;102m"
#define YELHB "\e[0;103m"
#define BLUHB "\e[0;104m"
#define MAGHB "\e[0;105m"
#define CYNHB "\e[0;106m"
#define WHTHB "\e[0;107m"

//High intensty text
#define HBLK "\e[0;90m"
#define HRED "\e[0;91m"
#define HGRN "\e[0;92m"
#define HYEL "\e[0;93m"
#define HBLU "\e[0;94m"
#define HMAG "\e[0;95m"
#define HCYN "\e[0;96m"
#define HWHT "\e[0;97m"

//Bold high intensity text
#define BHBLK "\e[1;90m"
#define BHRED "\e[1;91m"
#define BHGRN "\e[1;92m"
#define BHYEL "\e[1;93m"
#define BHBLU "\e[1;94m"
#define BHMAG "\e[1;95m"
#define BHCYN "\e[1;96m"
#define BHWHT "\e[1;97m"

#define LOW 4
#define MED 10
#define HIG 20
//Structures

// Define the Inventory structure
typedef struct
{
    int size;     // Size of the inventory
    char **items; // Array of items in the inventory
    int *activeItems;
} Inventory;

// Define Stats structure
typedef struct
{
    int HP;
    int atk;
    int def;
    int agi;
    int str;
    int dex;
    int intel;
    int luck;
} Stats;

// Define the Player structure
typedef struct
{
    char *id;
    char *name;            // Player's name
    int level;             // Player's level
    int wtdLevel;          // Wantedted level
    int xp;                // Player's experience points
    int gold;              // Player's gold
    char *currentLocation; // Player's current location [Change this]
    Stats *stats;          // Player's stats (e.g., HP, attack, defense)
    Inventory *inventory;  // Player's inventory
    int **NPCInfo;         // Player's interactions with NPCs (and status of Quest-line)
    char **activeQuests;   // Player's active quests
} Player;

// Define a typedef for function pointer
typedef int (*GameFunction)();

// Define a struct to hold string-function pairs
typedef struct {
    const char *str;
    GameFunction func;
} StringFunctionPair;


typedef enum
{
    SOLDIER,
    SHOPKEEPER,
    FIGHTER,
    CITIZEN,
    ///Add more as needed
}TypeOfNpc;

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

typedef enum
{
    QUEST_LVL,
    QUEST_STATUS,
    RELATIONSHIP,
}QuestManager;

typedef struct
{
    int xp;
    int gold;
    BUFF buff;
}Reward;


void MouseControl(int *x, int *y);
int return_current_status(char guessed_string[], char *current_status[], char selected_word[], int length);
void print_current_status(char *current_status[], int length);
void display_hangman(int result);
void intro();
int start_hangman();
int Hangtheman();


int Snakegame();

void genField(int size, int **field);
void reveal(int size, int x, int y, int *field, int *revealed);
void flag(int size, int x, int y, int *field, int *revealed, int *flag_count);
void printReveal(int size, int *revealed, int *field, int flag_mode);
int yesnoMine(int x, int y, int *field, int size);
int numRevealed(int size, int *revealed);
void cheat(int size, int *field);
int playMinesweeper(int size);
int Minesweeper();

void swap(int *a, int *b);
void shuffle_array(int arr[], int n);
void genMat(int size, int num, int mat[size][size]);
void printMemory1(int size, int num, int mat[size][size], int prog_arr[num]);
void printMemory2(int size, int num, int mat[size][size], int prog_arr[num], int click1_x, int click1_y, int click2_x, int click2_y);
void gameMemory(int size, int num, int mat[size][size], int *attempts, int prog_arr[num], int *progress);
int playMemory(int size, int num, int attempts);
int start_memory();
int Memory();

typedef struct cell
{
    int x, y;
} cell_pos;


void print_prison(int size, int i, int j, int k);
void printPrisonEscape(int size, int prison[2 * size][2 * size], cell_pos exit, cell_pos pl_pos, cell_pos guard_pos);
void result(int size, int prison[2 * size][2 * size], cell_pos exit, cell_pos pl_pos, cell_pos guard_pos);
void movePrison(int size, int prison[2 * size][2 * size], cell_pos *pl_pos, char pl_move, cell_pos exit);
void playermove(int size, int prison[2 * size][2 * size], cell_pos *pl_pos, cell_pos guard_pos, cell_pos exit);
void guardmove(int size, int prison[2 * size][2 * size], cell_pos *pl_pos, cell_pos *guard_pos, cell_pos exit);
void makePrison(int size, int prison[2 * size][2 * size]);
int playPrisonEscape(int size, int map);
int start_prison_escape();
int PrisonEscape();

int dig(int n, int num, int i);
void wordlegame(int n, int num, int guess);
int playWordle(int n, int attempts);
int start_wordle();
int Lockpicking();

void printMessage(int x, int y, char message);
void printFalcon(int x, int y);
void deleteFalcon(int x, int y);
void flyFalcon(int *falcon_x, int *falcon_y, int *prev_flight);
void printScopeFalcon(int x, int y, char c);
void openScopeFalcon(int x, int y, int side);
void deleteScopeFalcon(int x, int y, int side);
int playFalcon();
int falconry();

void print_combat(int plStamina, int oppStamina);
void gameCombat(int plWeapon, int *plStamina, int oppWeapon, int *oppStamina, int pl_att, int pl_def, int opp_att, int opp_def);
int playCombat(int plWeapon, int plStamina, int oppWeapon, int oppStamina);
int start_combat();
int Combat();
//From world.h


void delay2(int milliseconds);

void flushInputBuffer();

void gameLost(Player *player,int *state);

void printStory(const char *sentence,char *textStyle,int time);

void printFormattedStringWithColorAndDelay(const char *format, char *textStyle, int delay, ...);

Player* gameInitializer(char *PlayerID);

void selectState(Player *player,int *state);

void processState(Player *player,int *state);

char *createFileName( char *name);



//From player.h

// Function to create a new player
Player *createNewPlayer(char *playerID);

void savePlayerData(Player *player);

Player *loadPlayerData(char *playerID);

int playMinigame(Player *player,char *gameName);

void freePlayer(Player *player);

void showPlayerInventory(Player *player);

void showPlayerStats(Player *player);


//From characters.h

int getNpcNumber();
int getNPCID(char *npcName);
void interactWith(Player *player, char *npc); // Requires Quest Submission
void chooseNPC(char **NPCsAvailable, Player *player,int *state);




//From item.h

// Function prototypes

// void createItem(Item *item,const char name[],const char desc[],Grade grade,Itemtype itemtype,TypeOfBuff effect,int duration,int valueType,int type,int value);
// void modifyItem(Item *item,const char name[],const char desc[],Grade grade,Itemtype itemtype,TypeOfBuff effect,int duration,int valueType,int type,int value);

// void inspectItem(Item item);//generates item data
// void useItem(Player *player,Item *item);
// void destroyItem(Item *item);

// void serializeItem(Item item, FILE *file);
// void deserializeItem(Item *item, FILE *file);





//From locations.h

void navigationMode(Player *player,int *state);
char *getQuestLocation(char *NPCQuestID);
char **returnNPCsAvailable(char *locationNode);




//From quest.h

void questMode(Player *player,int *state);
void addActiveQuest(Player *player, char *questID);
void activateQuest(Player *player, char *npc, char *NPCQuestID);
char *getQuestDescription(char *NPCQuestID);
void giveQuestReward(Player *player, char *questID);
bool anyQuesttoSubmit(Player *player, char *npcName);
char *getQuestID(Player *player, char *npcName);
char *questDialogues(char *questID);
void interactionMode(Player* player,int *state);








//From story.h




//From minigame

#endif