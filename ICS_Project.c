#include"ICS_Project.h"


//FROM world.c
void printRules()
{
    //make a function with time 
    // delay that has rules of the game to print
}

Player* gameInitializer(char *PlayerID)
{
    // asks player wheather to start a new game
    // (warns that load game will be deleted) 
    // or to load game and if the file does not exist 
    // it gives a prompt telling player to start a new game.
    // It also makes a new json file or load json files based on 
    // player choise and returns a player variable
    int input;
    Player *player;

    printf("\n1----Start a New Game");
    printf("\n2----Load an Old Game");
    printf("Pleade enter a Choise : ");
    scanf("%d",input);

    if (input == 1)
    {
        // Start a new game
        // Create a new player object and initialize its properties
        player=createNewPlayer(PlayerID);
        savePlayerData(player);
    }
    else if (input == 2)
    {
        // Load an old game
        player = loadPlayerData(PlayerID);
        if (player == NULL)
        {
            printf("No saved game found. Starting a new game...\n");
            player = createNewPlayer(PlayerID);
            savePlayerData(player);
        }
        else
        {
            printf("Old game loaded successfully.\n");
        }
    }
    else
    {
        printf("Invalid choice. Starting a new game...\n");
        player = createNewPlayer(PlayerID);
        savePlayerData(player);
    }

    return player;
}

void selectMode(int *state)
{
    printf("\n0----Choose Navigation  Mode");
    printf("\n1----Choose Interaction Mode");
    printf("\n2----Choose Quest       Mode");
    printf("\nChoose a Mode to continue your Journey : ");

    scanf("%d",state);

}

void processState(Player *player,int *state)
{
    if(*state==0)
        navigationMode(player,state);
    else
    if(*state==1)
        interactionMode(player,state);
    else
    if(*state==2)
        questMode(player,state);

}





//From player.c

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

int playMinigame(Player *player,char *gameName)
{

// Define an array of string-function pairs
//correct the names of minigame function to these
    StringFunctionPair functions[] = {
        // {"Combat", Combat},
        // {"Falconry", Falconry},
        //add more games
    };

    // to call a function based on input string
    int i;
    for (i = 0; i < sizeof(functions) / sizeof(functions[0]); i++) {
        if (strcmp(gameName, functions[i].str) == 0) {
            functions[i].func(); // Call the function associated with the input string
            return;
        }
    }
    printf("No function found for input: %s\n", gameName);

}




//From charcters.c

int getNpcNumber()
{
    FILE *file=fopen("../characters.json","r");
    if(file==NULL)
    {
        fprintf(stderr,"Failed to open file for reading\n");
        return NULL;
    }

    fseek(file,0,SEEK_END);
    long fileSize=ftell(file);
    fseek(file,0,SEEK_SET);

    char *fileContent=(char*)malloc(fileSize+1);
    if (fileContent == NULL) {
        fprintf(stderr, "Memory allocation failed for file content\n");
        fclose(file);
        return NULL;
    }

    size_t bytesRead = fread(fileContent, 1, fileSize, file);
    if (bytesRead != fileSize) {
        printf("Error reading file.\n");
        fclose(file);
        free(fileContent);
        return NULL;
    }
    
    fileContent[fileSize]=0;
    fclose(file);

    cJSON *root=cJSON_Parse(fileContent);
    if (root == NULL) {
        printf("Error parsing JSON data.\n");
        free(fileContent);
        return NULL;
    }

    cJSON *charactersArray=cJSON_GetObjectItem(root,"characters");

    return cJSON_GetArraySize(charactersArray);
}



//From item.h




//From locations.h

void navigationMode(Player *player,int *state)
{
    FILE *file=fopen("../navigations.json","r");
    if(file==NULL)
    {
        fprintf(stderr,"Failed to open file for reading\n");
        return ;
    }

    fseek(file,0,SEEK_END);
    long fileSize=ftell(file);
    fseek(file,0,SEEK_SET);

    char *fileContent=(char*)malloc(fileSize+1);
    if (fileContent == NULL) {
        fprintf(stderr, "Memory allocation failed for file content\n");
        fclose(file);
        return ;
    }

    size_t bytesRead = fread(fileContent, 1, fileSize, file);
    if (bytesRead != fileSize) {
        printf("Error reading file.\n");
        fclose(file);
        free(fileContent);
        return ;
    }
    
    fileContent[fileSize]=0;
    fclose(file);

    cJSON *root=cJSON_Parse(fileContent);
    if (root == NULL) {
        printf("Error parsing JSON data.\n");
        free(fileContent);
        return;
    }

    cJSON *currentLocationInJson=cJSON_GetObjectItem(root,"locations");

    cJSON *children=cJSON_GetObjectItem(currentLocationInJson,"children");

    char *token_prev=NULL;
    char *token=strtok(player->currentLocation,"/");

    while(token!=NULL)
    {
        cJSON *child=children->child;

        while(child!=NULL)
        {
            cJSON *childName=cJSON_GetObjectItem(child,"name");
            if(!strcmp(token,childName->valuestring))
            {
                children=cJSON_GetObjectItem(child,"children");
                currentLocationInJson=child;
                break;
            }
            
            child=child->next;
        }

        token_prev=token;
        token=strtok(NULL,"/");
    }

    char input;

    if(strcmp(player->currentLocation,"WORLD")==0)
    {
        for(int i=0;i<cJSON_GetArraySize(children);i++)
        {
            cJSON *child=cJSON_GetArrayItem(children,i);
            printf("\n%d...to go to %s",i+1,cJSON_GetObjectItem(child,"name")->valuestring);
        }

        printf("\nOr Enter i/I for Interaction Mode");
        printf("\nOr Enter q/Q for Quest Mode");
        printf("\nOr Enter e/E to Exit the Game");

        printf("\nMake a Choise to continue : ");
        input=getchar();
        if(input=='e'||input=='E')
        {
            *state =-1;
            return;
        }

        if(input=='i'||input=='I')
        {
            *state = 1;
            return;
        }

        if(input=='q' || input=='Q')
        {
            *state=2;
            return;
        }

        while(input<='0' || input>(cJSON_GetArraySize(children)+'0'))
        {
            printf("\nWell such a place doesn't exist. Enter a Valid Location!");
            printf("\nChoose a Location to move to : ");
            input=getchar();
        }
    }
    else
    {
        printf("\n0...to go out of %s",token_prev);
        for(int i=0;i<cJSON_GetArraySize(children);i++)
        {
            cJSON *child=cJSON_GetArrayItem(children,i);
            printf("\n%d...to go to %s",i+1,cJSON_GetObjectItem(child,"name")->valuestring);
        }

        printf("\nOr Enter i/I for Interaction Mode");
        printf("\nOr Enter q/Q for Quest Mode");
        printf("\nOr Enter e/E to Exit the Game");

        printf("\nMake a Choise to continue : ");
        input=getchar();
        if(input=='e'||input=='E')
        {
            *state =-1;
            return;
        }

        if(input=='i'||input=='I')
        {
            *state = 1;
            return;
        }

        if(input=='q' || input=='Q')
        {
            *state=2;
            return;
        }

        while(input<'0' || input>(cJSON_GetArraySize(children)+'0'))
        {
            printf("\nWell such a place doesn't exist. Enter a Valid Location!");
            printf("\nChoose a Location to move to : ");
            input=getchar();
        }
    }

    if(input-'0')
    {
        cJSON *child=cJSON_GetArrayItem(children,input-1);
        strcat(player->currentLocation,"/");
        strcat(player->currentLocation,cJSON_GetObjectItem(child,"name")->valuestring);
    }
    else
    {
        char *str=player->currentLocation;
        int j;

        for(int i=0;str[i];i++)
        {
            if(str[i]=='/')j=i;
        }

        str[j]=0;
    }
}




//From quest.h

void questMode(Player *player,int *state)
{
    FILE *file=fopen("../quests.json","r");
    if (!file) {
        fprintf(stderr, "Failed to open quests.json\n");
        return;
    }

    fseek(file,0,SEEK_END);
    long fileSize=ftell(file);
    fseek(file,0,SEEK_SET);

    char *fileContent = (char *)malloc(fileSize + 1);
    if (!fileContent) {
        fprintf(stderr, "Memory allocation failed for file content\n");
        fclose(file);
        return;
    }

    fread(fileContent,1,fileSize,file);
    fclose(file);
    fileContent[fileSize]=0;

    cJSON *root=cJSON_Parse(fileContent);
    if (!root) {
        const char *error = cJSON_GetErrorPtr();
        if (error != NULL) {
            fprintf(stderr, "Error before: %s\n", error);
        }
        cJSON_Delete(root);
        free(fileContent);
        return;
    }

    cJSON *questsArray=cJSON_GetObjectItem(root,"quests");
    cJSON *ptr=questsArray->child;

    char *cur_loc=player->currentLocation;
    int j=0,i=0;
    int choises[10];
    char input;

    for(int i=0;cur_loc[i];i++)
        if(cur_loc[i]=="/")j=i;

    cur_loc=player->currentLocation+j;

    int n=sizeof(player->activeQuests)/sizeof(char*);
    j=0;

    for(int i=0;i<n;i++)
    {

        char *questId=player->activeQuests[i];
        while(ptr)
        {
            if(strcmp(questId,cJSON_GetObjectItem(ptr,"QuestID")->valuestring)==0 && strcmp(cur_loc,cJSON_GetObjectItem(ptr,"Location")->valuestring)==0)
                choises[j++]=i;

            ptr=ptr->next;
        }
    }
    choises[j]=-1;
    
    for(i=0;i<10 && choises[i]>=0;i++)
    {
        cJSON *child=cJSON_GetArrayItem(questsArray,choises[i]);
        printf("\n%d...to start quest- %s",i+1,cJSON_GetObjectItem(child,"name")->valuestring);
    }

    printf("\nOr Enter i/I for Interaction Mode");
    printf("\nOr Enter n/N for Navigation Mode");
    printf("\nOr Enter e/E to Exit the Game");

    printf("\nMake a Choise to continue : ");
    input=getchar();
    if(input=='e'||input=='E')
    {
        *state =-1;
        return;
    }

    if(input=='i'||input=='I')
    {
        *state = 1;
        return;
    }

    if(input=='n' || input=='N')
    {
        *state=0;
        return;
    }

    while(input<='0' || input>i+'0')
    {
        printf("\nWell such a quest doesn't exist. Enter a Valid Choise!");
        printf("\nMake a Choise to continue : ");
        input=getchar();
    }

    cJSON *child=cJSON_GetArrayItem(questsArray,choises[i]);
    playMiniGame(player,cJSON_GetObjectItem(child,"Minigame")->valuestring);//in player.c

}



//From story.h