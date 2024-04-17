// Tokenize, as change in currentLocation
// Quest status: 0=Incomplete/Inactive, 1=Active, 2=To-Submit

// Header files inclusion
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
    char *name;            // Player's name
    int level;             // Player's level
    Stats stats;           // Player's stats (e.g., HP, attack, defense)
    Inventory inventory;   // Player's inventory
    int wtdLevel;          // Wantedted level
    int xp;                // Player's experience points
    int gold;              // Player's gold
    char *currentLocation; // Player's current location [Change this]
    char **activeQuests;   // Player's active quests
    int **NPCInfo;         // Player's interactions with NPCs (and status of Quest-line)
} Player;

// Function to activate quest
void activateQuest(Player *player, char *npc, char *NPCQuestID)
{
    // Assume NPCInfo is properly initialized and contains valid data
    // Find the NPC ID based on the given NPC name
    int npcID = getNPCID(npc);

    // Find the index corresponding to the NPC ID in NPCInfo
    int npcIndex = npcID; // For simplicity, assuming the NPC ID corresponds to its index in NPCInfo

    // Find the Quest level based on the given NPCQuestID
    int questLevel = getQuestLevel(NPCQuestID);

    // Update the quest status of the relevant NPC for the given quest level
    player->NPCInfo[npcIndex][questLevel] = 1; // Set quest status to 1 (active)
}

// Function to parse quests.json and get the quest location
char *getQuestLocation(const char *NPCQuestID)
{
    char *location = NULL;

    // Read the contents of the quests.json file
    FILE *file = fopen("quests.json", "r");
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
                cJSON *quests = cJSON_GetObjectItem(root, "quests");
                if (quests != NULL && cJSON_IsArray(quests))
                {
                    cJSON *quest;
                    cJSON_ArrayForEach(quest, quests)
                    {
                        cJSON *questIDJSON = cJSON_GetObjectItem(quest, "QuestID");
                        if (questIDJSON != NULL && cJSON_IsString(questIDJSON) && strcmp(questIDJSON->valuestring, NPCQuestID) == 0)
                        {
                            // Quest found, get its location
                            cJSON *questLocation = cJSON_GetObjectItem(quest, "Location");
                            if (questLocation != NULL && cJSON_IsString(questLocation))
                            {
                                location = strdup(questLocation->valuestring);
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

    return location;
}

// Function to parse quests.json and get the quest description
char *getQuestDescription(const char *NPCQuestID)
{
    char *description = NULL;

    // Read the contents of the quests.json file
    FILE *file = fopen("quests.json", "r");
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
                cJSON *quests = cJSON_GetObjectItem(root, "quests");
                if (quests != NULL && cJSON_IsArray(quests))
                {
                    cJSON *quest;
                    cJSON_ArrayForEach(quest, quests)
                    {
                        cJSON *questIDJSON = cJSON_GetObjectItem(quest, "QuestID");
                        if (questIDJSON != NULL && cJSON_IsString(questIDJSON) && strcmp(questIDJSON->valuestring, NPCQuestID) == 0)
                        {
                            // Quest found, get its description
                            cJSON *questDesc = cJSON_GetObjectItem(quest, "Description");
                            if (questDesc != NULL && cJSON_IsString(questDesc))
                            {
                                description = strdup(questDesc->valuestring);
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

    return description;
}

// Function to parse quests.json and get the quest reward
void getQuestReward(Player *player, const char *questID)
{
    // Read the contents of the quests.json file
    FILE *file = fopen("quests.json", "r");
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
                cJSON *quests = cJSON_GetObjectItem(root, "quests");
                if (quests != NULL && cJSON_IsArray(quests))
                {
                    cJSON *quest;
                    cJSON_ArrayForEach(quest, quests)
                    {
                        cJSON *questIDJSON = cJSON_GetObjectItem(quest, "QuestID");
                        if (questIDJSON != NULL && cJSON_IsString(questIDJSON) && strcmp(questIDJSON->valuestring, questID) == 0)
                        {
                            // Quest found, get its reward
                            cJSON *reward = cJSON_GetObjectItem(quest, "Reward");
                            if (reward != NULL && cJSON_IsObject(reward))
                            {
                                // Update player's attributes based on the reward
                                cJSON *xp = cJSON_GetObjectItem(reward, "xp");
                                cJSON *gold = cJSON_GetObjectItem(reward, "gold");
                                cJSON *item = cJSON_GetObjectItem(reward, "item");
                                cJSON *relationship = cJSON_GetObjectItem(reward, "relationship");

                                if (xp != NULL && cJSON_IsNumber(xp))
                                {
                                    player->xp += xp->valueint;
                                }
                                if (gold != NULL && cJSON_IsNumber(gold))
                                {
                                    player->gold += gold->valueint;
                                }
                                if (item != NULL && cJSON_IsString(item))
                                {
                                    // Append the item to the inventory
                                    if (player->inventory.items == NULL)
                                    {
                                        player->inventory.items = (char **)malloc(sizeof(char *));
                                        player->inventory.size = 1;
                                    }
                                    else
                                    {
                                        player->inventory.items = (char **)realloc(player->inventory.items, (player->inventory.size + 1) * sizeof(char *));
                                        player->inventory.size++;
                                    }
                                    player->inventory.items[player->inventory.size - 1] = strdup(item->valuestring);
                                }
                                if (relationship != NULL && cJSON_IsNumber(relationship))
                                {
                                    // Update the relationship with the NPC
                                    int npcID = atoi(strtok(questID, "_"));
                                    player->NPCInfo[npcID][2] += relationship->valueint;
                                }

                                // Increase quest level and reset quest status
                                int npcID = atoi(strtok(questID, "_"));
                                player->NPCInfo[npcID][0]++;   // Increase quest level
                                player->NPCInfo[npcID][1] = 0; // Reset quest status to 0 (incomplete/inactive)
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
}

// Function to check if there is any quest to submit to the NPC
bool anyQuesttoSubmit(Player *player, const char *npcName)
{
    int npcID = getNPCID(npcName);
    if (npcID == -1)
    {
        // NPC not found
        return false;
    }

    // Get the quest ID for the NPC
    char *questID = getQuestID(player, npcName);
    if (questID == NULL)
    {
        // Quest ID not found
        return false;
    }

    // Check if the quest status is "to-submit" (status = 2)
    if (player->NPCInfo[npcID][1] == 2)
    {
        // Quest to submit found
        free(questID); // Free the allocated memory
        return true;
    }

    // No quest to submit
    free(questID); // Free the allocated memory
    return false;
}

// Function to parse characters.json and get the NPC ID
int getNPCID(const char *npcName)
{
    // Read the contents of the characters.json file
    FILE *file = fopen("characters.json", "r");
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
                cJSON *characters = cJSON_GetObjectItem(root, "characters");
                if (characters != NULL && cJSON_IsArray(characters))
                {
                    cJSON *character;
                    cJSON_ArrayForEach(character, characters)
                    {
                        cJSON *name = cJSON_GetObjectItem(character, "name");
                        if (name != NULL && cJSON_IsString(name) && strcmp(name->valuestring, npcName) == 0)
                        {
                            cJSON *id = cJSON_GetObjectItem(character, "ID");
                            if (id != NULL && cJSON_IsNumber(id))
                            {
                                // NPC found, return its ID
                                cJSON_Delete(root);
                                fclose(file);
                                free(fileContent);
                                return id->valueint;
                            }
                        }
                    }
                }
                cJSON_Delete(root);
            }
            free(fileContent);
        }
        fclose(file);
    }

    // NPC not found or error reading file
    return -1;
}

// Function to get the quest ID for a given NPC and player
char *getQuestID(Player *player, const char *npcName)
{
    int npcID = getNPCID(npcName);
    if (npcID == -1)
    {
        // NPC not found
        return NULL;
    }

    // Get the quest level from the NPCInfo array
    int questLevel = player->NPCInfo[npcID][0];

    // Construct and return the quest ID string
    char *questID = (char *)malloc(20 * sizeof(char)); // Adjust size as needed
    if (questID != NULL)
    {
        sprintf(questID, "%d_%d", npcID, questLevel);
    }
    return questID;
}

// Function to parse dialogues.json for dialogues corresponding to Quest
char *questDialogues(const char *questID)
{
    printf("Debug 1\n");
    char *dialoguesString = NULL;

    // Read the contents of the dialogues.json file
    FILE *file = fopen("dialogue.json", "r");
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

// Function to interact with given NPC
void interactWith(Player *player, char *npc) // Requires Quest Submission
{
    printf("Debug 0 : %s\n", npc);

    // If any quest to submit:
    if (anyQuesttoSubmit(player, npc))
        getQuestReward(player, npc);

    char *NPCQuestID = getQuestID(player, npc);

    char *dialogues = questDialogues(NPCQuestID); // NPCQuestID instead of String
    if (dialogues != NULL)
    {
        printf("%s\n", dialogues);
        free(dialogues); // Free the allocated memory
    }

    // Quest description and reward
    char *questDescription = getQuestDescription(NPCQuestID);
    if (questDescription != NULL)
    {
        printf("%s\n", questDescription);
        free(questDescription);
    }

    // Quest Location
    char *questLocation = getQuestLocation(NPCQuestID);
    if (questLocation != NULL)
    {
        printf("You must go to %s to complete the Quest\n", questLocation);
        free(questLocation);
    }

    // Get choice to accept or reject
    printf("Enter 1 to accept this quest now:\nEnter 0 to reject:\n");
    int questChoice;
    scanf("%d", &questChoice);

    // If accepted, add to active quests
    if (questChoice == 1)
        activateQuest(player, npc, NPCQuestID);
}

// Function to choose NPCs from available NPCs
void chooseNPC(char **NPCsAvailable, Player *player)
{
    while (1)
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

// Function to get last word of string
char *getLastWordAfterLastSlash(const char *input)
{
    // Find the position of the last slash
    const char *lastSlash = strrchr(input, '/');

    if (lastSlash == NULL)
    {
        // No slash found, return a copy of the input string
        return strdup(input);
    }
    else
    {
        // Extract the substring after the last slash
        return strdup(lastSlash + 1);
    }
}

// Function to find the last location node in the currentLocation array of Player
void interactWithLocalNPCs(Player *player)
{
    // Find and return the last string in currentLocation array
    char *locationNode = getLastWordAfterLastSlash(player->currentLocation);

    // Now go to locations.json and find "locationNode", and return array of NPCs available
    char **NPCsAvailable = returnNPCsAvailable(locationNode);

    chooseNPC(NPCsAvailable, player);

    return;
}

int main()
{
    Player p1;
    p1.currentLocation = "WORLD/CITY/COLLOSEUM";

    interactWithLocalNPCs(&p1);
}
