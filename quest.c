#include"header files/headers.h"

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
