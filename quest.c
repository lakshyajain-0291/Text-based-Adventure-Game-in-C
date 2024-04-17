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

    int n=sizeof(player->activeQuests)/sizeof(char*);
    for(int i=0;i<n;i++)
    {
        char *questId=player->activeQuests[i];
        if(strcmp(questId,cJSON_GetObjectItem(ptr,"QuestID")->valuestring)==0)
        {
            
        }
        
    }
}
