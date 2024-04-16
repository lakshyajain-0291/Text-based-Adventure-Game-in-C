#include"header files/headers.h"

Player *createNewPlayer(char *playerID)
{
    Player *newPlayer = (Player*)malloc(sizeof(Player));
    if (newPlayer == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(EXIT_FAILURE);
    }

    newPlayer->stats = (Stats*)malloc(sizeof(Stats));
    if (newPlayer->stats == NULL) 
    {
    fprintf(stderr, "Memory allocation failed\n");
    exit(EXIT_FAILURE);
    }

    newPlayer->inventory = (Inventory*)malloc(sizeof(Inventory));
    if (newPlayer->inventory == NULL) 
    {
    fprintf(stderr, "Memory allocation failed\n");
    exit(EXIT_FAILURE);
    }

    newPlayer->currentLocation = (char**)malloc(sizeof(char*)*2);
    if (newPlayer->currentLocation == NULL) 
    {
    fprintf(stderr, "Memory allocation failed\n");
    exit(EXIT_FAILURE);
    }

    // Initialize player properties
    strcpy(newPlayer->id, playerID);
    strcpy(newPlayer->name, "Gaius Marcus Agrippa");
    newPlayer->level = 1;

    newPlayer->stats->HP = 100;
    newPlayer->stats->atk = 10;
    newPlayer->stats->def = 5;
    newPlayer->stats->agi = 8;
    newPlayer->stats->str = 12;
    newPlayer->stats->dex = 9;
    newPlayer->stats->intel = 6;
    newPlayer->stats->luck = 7;

    newPlayer->inventory->size = 10;
    // Initialize inventory items as needed
    // For simplicity, let's leave them empty for now

    newPlayer->wtdLevel = 0;
    newPlayer->xp = 0;
    newPlayer->gold = 50;

    strcpy(newPlayer->currentLocation,"WORLD/CITY");
    // char *token=strtok("WORLD/CITY","/");
    // for(int i=0;token!=NULL;i++)
    // {
    //     newPlayer->currentLocation[i]=token;
    //     token=strtok(NULL,"/");
    // }
    
    newPlayer->activeQuests=NULL;

    int numOfNpcs=getNpcNumber();//reading chharcters.json for no of npcs

    newPlayer->npcInfo=(int**)malloc(sizeof(int*)*numOfNpcs);
    
    for(int i=0;i<numOfNpcs;i++)
    {
        newPlayer->npcInfo[i]=(int*)malloc(sizeof(int)*3);
        newPlayer->npcInfo[i][QUEST_LVL]=0;
        newPlayer->npcInfo[i][QUEST_STATUS]=0;
        newPlayer->npcInfo[i][RELATONSHIP]=0;
    }

    return newPlayer;
}

void savePlayerData(Player *player)
{
    cJSON *root=cJSON_CreateObject();
    cJSON *playerObject= cJSON_CreateObject();
    cJSON *statsObject= cJSON_CreateObject();
    cJSON *inventoryObject= cJSON_CreateObject();

    cJSON_AddItemToObject(root,"player",playerObject);

    cJSON_AddStringToObject(playerObject,"id",player->id);
    cJSON_AddStringToObject(playerObject,"name",player->name);
    cJSON_AddNumberToObject(playerObject,"level",player->level);
    cJSON_AddNumberToObject(playerObject,"wtdLevel",player->wtdLevel);
    cJSON_AddNumberToObject(playerObject,"xp",player->xp);
    cJSON_AddNumberToObject(playerObject,"gold",player->gold);
    cJSON_AddStringToObject(playerObject,"currentLocation",player->currentLocation);

    cJSON_AddItemToObject(playerObject,"stats",statsObject);

    cJSON_AddNumberToObject(statsObject,"hp",player->stats->HP);
    cJSON_AddNumberToObject(statsObject,"atk",player->stats->atk);
    cJSON_AddNumberToObject(statsObject,"def",player->stats->def);
    cJSON_AddNumberToObject(statsObject,"agi",player->stats->agi);
    cJSON_AddNumberToObject(statsObject,"str",player->stats->str);
    cJSON_AddNumberToObject(statsObject,"dex",player->stats->dex);
    cJSON_AddNumberToObject(statsObject,"intel",player->stats->intel);
    cJSON_AddNumberToObject(statsObject,"luck",player->stats->luck);

    cJSON_AddItemToObject(playerObject,"inventory",inventoryObject);

    cJSON_AddNumberToObject(inventoryObject,"size",player->inventory->size);
    cJSON *itemsArray=cJSON_AddArrayToObject(inventoryObject,"items");

    for(int i=0;i<player->inventory->size;i++)
    {
        cJSON *item=cJSON_CreateObject();
        cJSON_AddItemToArray(itemsArray,item);
        cJSON_AddStringToObject(item,"item",player->inventory->items[i]);
    }

    cJSON *npcsArray=cJSON_AddArrayToObject(playerObject,"NPCinfo");

    int n=sizeof(player->npcInfo)/sizeof(int*);
    for(int i=0;i<n;i++)
    {
        cJSON *npc=cJSON_CreateObject();
        cJSON_AddItemToArray(itemsArray,npc);
        cJSON_AddNumberToObject(npc,"npc_id",i);
        cJSON_AddNumberToObject(npc,"quest_lvl",player->npcInfo[i][QUEST_LVL]);
        cJSON_AddNumberToObject(npc,"quest_status",player->npcInfo[i][QUEST_STATUS]);
        cJSON_AddNumberToObject(npc,"relationship",player->npcInfo[i][RELATONSHIP]);
    }


    char *fileName;
    fileName=(char*)malloc(sizeof(char)*50);
    strcpy(fileName,player->id);
    strcat(fileName,".json");

    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file for writing\n");
        return;
    }

    char *jsonData = cJSON_Print(root);
    fprintf(file, "%s", jsonData);
    fclose(file);
    free(jsonData);
    cJSON_Delete(root);
}

Player *loadPlayerData(char *playerID)
{
    char *fileName;
    strcpy(fileName,playerID);
    strcat(fileName,".json");

    FILE *file=fopen(fileName,"r");
    if(file==NULL)
    {
        fprintf(stderr,"Failed to open file for reading\n");
        return NULL;
    }

    fseek(file,0,SEEK_END);
    long fileSize = ftell(file);
    fseek(file,0,SEEK_SET);

    char *fileContent = (char*)malloc(fileSize + 1);
    if (fileContent == NULL) {
        fprintf(stderr, "Memory allocation failed for file content\n");
        fclose(file);
        return NULL;
    }

    fread(fileContent,1,fileSize,file);
    fclose(file);
    fileContent[fileSize]=0;

    cJSON *root=cJSON_Parse(fileContent);
    cJSON *playerObject=cJSON_GetObjectItem(root,"player");

    Player *player=(Player*)malloc(sizeof(Player));
    if (player == NULL) {
        fprintf(stderr, "Memory allocation failed for player\n");
        cJSON_Delete(root);
        free(fileContent);
        return NULL;
    }


    // Load player properties from JSON object
    strcpy(player->id,cJSON_GetObjectItem(playerObject,"id")->valuestring);
    strcpy(player->name,cJSON_GetObjectItem(playerObject,"name")->valuestring);
    player->level=cJSON_GetObjectItem(playerObject,"level")->valueint;
    player->wtdLevel=cJSON_GetObjectItem(playerObject,"wtdLevel")->valueint;
    player->xp=cJSON_GetObjectItem(playerObject,"xp")->valueint;
    player->gold=cJSON_GetObjectItem(playerObject,"gold")->valueint;
    strcpy(player->currentLocation,cJSON_GetObjectItem(playerObject,"currentLocation")->valuestring);

    // Load player stats from JSON object
    cJSON *statsObject=cJSON_GetObjectItem(playerObject,"stats");
    player->stats->HP = cJSON_GetObjectItem(statsObject, "HP")->valueint;
    player->stats->atk = cJSON_GetObjectItem(statsObject, "atk")->valueint;
    player->stats->def = cJSON_GetObjectItem(statsObject, "def")->valueint;
    player->stats->agi = cJSON_GetObjectItem(statsObject, "agi")->valueint;
    player->stats->str = cJSON_GetObjectItem(statsObject, "str")->valueint;
    player->stats->dex = cJSON_GetObjectItem(statsObject, "dex")->valueint;
    player->stats->intel = cJSON_GetObjectItem(statsObject, "intel")->valueint;
    player->stats->luck = cJSON_GetObjectItem(statsObject, "luck")->valueint;


    cJSON *inventoryObject=cJSON_GetObjectItem(playerObject,"inventory");
    player->inventory->size = cJSON_GetObjectItem(inventoryObject, "size")->valueint;
    cJSON *itemsArray=cJSON_GetObjectItem(inventoryObject,"items");

    cJSON *item=NULL;
    int i=0;
    cJSON_ArrayForEach(item,itemsArray)
    {
        strcpy(player->inventory->items[i], cJSON_GetObjectItem(item,"item")->valuestring);
        i++;
    }


    cJSON *npcsArray=cJSON_GetObjectItem(playerObject,"NPCinfo");

    cJSON *npc=NULL;
    int i=0;
    cJSON_ArrayForEach(npc,npcsArray)
    {
        player->npcInfo[i][QUEST_LVL]= cJSON_GetObjectItem(npc,"quest_lvl")->valueint;
        player->npcInfo[i][QUEST_STATUS]= cJSON_GetObjectItem(npc,"quest_status")->valueint;
        player->npcInfo[i][RELATONSHIP]= cJSON_GetObjectItem(npc,"relationship")->valueint;
        i++;
    }

    cJSON_Delete(root);
    free(fileContent);
    return player;
    
}

