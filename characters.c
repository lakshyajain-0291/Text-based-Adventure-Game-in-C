#include"header files/headers.h"

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

