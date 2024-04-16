#include "header files/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Define the Inventory structure
typedef struct
{
    int size;    // Size of the inventory
    char *items; // Array of items in the inventory
} Inventory;

// Define Stats structure
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
} Stats;

// Define the Player structure
typedef struct
{
    char *name;             // Player's name
    int level;              // Player's level
    Stats stats;            // Player's stats (e.g., HP, attack, defense)
    Inventory inventory;    // Player's inventory
    int wtdLevel;           // Wantedted level
    int xp;                 // Player's experience points
    int gold;               // Player's gold
    char **currentLocation; // Player's current location
    char **activeQuests;    // Player's active quests
    // QuestManager questManager;//players' active
} Player;

// Function to parse dialogues.json for dialogues corresponding to Quest
char *questDialogues(const char *questID)
{
    char *dialoguesString = NULL;

    // Read the contents of the dialogues.json file
    FILE *file = fopen("dialogues.json", "r");
    if (file != NULL)
    {
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        char *fileContent = (char *)malloc(fileSize + 1);
        if (fileContent != NULL)
        {
            fread(fileContent, 1, fileSize, file);
            fileContent[fileSize] = '\0';

            // Parse the JSON content
            cJSON *root = cJSON_Parse(fileContent);
            if (root != NULL)
            {
                cJSON *dialogues = cJSON_GetObjectItem(root, "dialogues");
                if (dialogues != NULL && cJSON_IsArray(dialogues))
                {
                    cJSON *dialogue;
                    cJSON_ArrayForEach(dialogue, dialogues)
                    {
                        cJSON *questIDJSON = cJSON_GetObjectItem(dialogue, "Quest_ID");
                        if (questIDJSON != NULL && cJSON_IsString(questIDJSON) && strcmp(questIDJSON->valuestring, questID) == 0)
                        {
                            cJSON *textArray = cJSON_GetObjectItem(dialogue, "Text");
                            if (textArray != NULL && cJSON_IsArray(textArray))
                            {
                                cJSON *text;
                                cJSON *tempString = cJSON_CreateString("");
                                cJSON_ArrayForEach(text, textArray)
                                {
                                    cJSON *temp = cJSON_GetObjectItem(dialogue, "Text");
                                    if (temp != NULL && cJSON_IsString(temp))
                                    {
                                        cJSON *tempText = cJSON_CreateString(temp->valuestring);
                                        cJSON_AddItemToArray(tempString, tempText);
                                    }
                                }
                                dialoguesString = cJSON_Print(tempString);
                                cJSON_Delete(tempString);
                            }
                            break; // No need to continue searching
                        }
                    }
                }
                cJSON_Delete(root);
            }
            free(fileContent);
        }
        fclose(file);
    }

    return dialoguesString;
}

void interactWith(Player *player, char *npc)
{
    printf("Debug 0 : %s\n", npc);

    //NPCQuestID = getNPCQuestID(player, npc)

    char *dialogues = questDialogues("Metalsmith Gaius_1"); //NPCQuestID instead of String
    if (dialogues != NULL)
    {
        printf("%s\n", dialogues);
        free(dialogues); // Free the allocated memory
    }
    // Get choice to accept or reject

    /*
    if accepted: change **activeQuests
    else return to chooseNPC
    */
}

void chooseNPC(char **NPCsAvailable, Player *player)
{   
    while(1)
    {
        printf("Available NPCs:\n");
        for (int i = 0; NPCsAvailable[i] != NULL; i++)
        {
            printf("%d. %s\n", i + 1, NPCsAvailable[i]);
        }

        printf("Choose an NPC to interact with (enter the corresponding number) (or enter 0 to return to Navigation state): ");
        int choice;
        scanf("%d", &choice);

        // Check if the choice is valid
        if (choice >= 1 && choice <= sizeof(NPCsAvailable) / sizeof(NPCsAvailable[0]) + 1)
        {
            char *chosenNPC = NPCsAvailable[choice - 1];
            interactWith(player, chosenNPC);
        }
        else if (choice == 0)
        {
            return;
        }
        else
        {
            printf("Invalid choice. Please choose a valid NPC.\n");
        }
    }
}

char *stringdum(const char *str)
{
    size_t len = strlen(str) + 1;
    char *new_str = (char *)malloc(len);
    if (new_str == NULL)
        return NULL;
    strcpy(new_str, str);
    return new_str;
}
// Function to return array of NPCs available at the specified locationNode
char **returnNPCsAvailable(const char *locationNode)
{
    char **NPCsAvailable = NULL;

    // Read the contents of the locations.json file
    FILE *file = fopen("locations.json", "r");
    if (file != NULL)
    {
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        char *fileContent = (char *)malloc(fileSize + 1);
        if (fileContent != NULL)
        {
            fread(fileContent, 1, fileSize, file);
            fileContent[fileSize] = '\0';

            // Parse the JSON content
            cJSON *root = cJSON_Parse(fileContent);
            if (root != NULL)
            {
                cJSON *locations = cJSON_GetObjectItem(root, "locations");
                if (locations != NULL && cJSON_IsArray(locations))
                {
                    cJSON *location;
                    cJSON_ArrayForEach(location, locations)
                    {
                        cJSON *name = cJSON_GetObjectItem(location, "name");
                        if (name != NULL && cJSON_IsString(name) && strcmp(name->valuestring, locationNode) == 0)
                        {
                            cJSON *npcs = cJSON_GetObjectItem(location, "NPCs");
                            if (npcs != NULL && cJSON_IsArray(npcs))
                            {
                                int numNPCs = cJSON_GetArraySize(npcs);
                                NPCsAvailable = (char **)malloc((numNPCs + 1) * sizeof(char *));
                                if (NPCsAvailable != NULL)
                                {
                                    for (int i = 0; i < numNPCs; i++)
                                    {
                                        cJSON *npcObj = cJSON_GetArrayItem(npcs, i);
                                        cJSON *npc = cJSON_GetObjectItem(npcObj, "npc");
                                        if (npc != NULL && cJSON_IsString(npc))
                                        {
                                            NPCsAvailable[i] = strdup(npc->valuestring);
                                        }
                                    }
                                    NPCsAvailable[numNPCs] = NULL; // Null-terminate the array
                                }
                            }
                            break; // No need to continue searching
                        }
                    }
                }
                cJSON_Delete(root);
            }
            free(fileContent);
        }
        fclose(file);
    }

    return NPCsAvailable;
}
// Function to find the last string in an array of strings
char *findLastString(char **strings, int count)
{
    if (count <= 0)
    {
        // If the array is empty or count is invalid, return NULL
        return NULL;
    }
    else
    {
        // Return the last string in the array
        return strings[count - 1];
    }
}

// Function to find the last location node in the currentLocation array of Player
void interactWithLocalNPCs(Player *player)
{
    int count = 0;
    // Determine the number of strings in currentLocation array
    while (player->currentLocation[count] != NULL)
    {
        count++;
    }
    // Find and return the last string in currentLocation array
    char *locationNode = findLastString(player->currentLocation, count);

    // Now go to locations.json and find "locationNode", and return array of NPCs available
    char **NPCsAvailable = returnNPCsAvailable(locationNode);

    chooseNPC(NPCsAvailable, player);

    return;
}

int main()
{
    Player p1;
    p1.currentLocation = (char **)malloc(3 * sizeof(char *));
    p1.currentLocation[0] = "CITY";
    p1.currentLocation[1] = "COLLOSEUM";
    p1.currentLocation[2] = NULL; // Null-terminate the array

    interactWithLocalNPCs(&p1);
}
