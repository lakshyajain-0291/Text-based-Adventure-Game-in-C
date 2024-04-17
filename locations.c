#include"header files/headers.h"

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
