#include "headers.h"

int main()
{
    char *input=(char*)malloc(sizeof(char)*100);
    char *playerName=(char*)malloc(sizeof(char)*100);
    int playerInput,gamePlay;
    Player *player=NULL;

    printf("\nDo youwant to start the game (0/1) : ");
    scanf("%d",&playerInput);
    gamePlay=playerInput;

    if(playerInput)
    {
        printf("\nEnter your Name : ");
        scanf("%s",playerName);
        player=(Player*)malloc(sizeof(Player));
        player=getPlayerInfo(playerName);//to make
    }

    while(gamePlay)
    {
        int playerState=1;
        displayPlayerinfo();//to make

        printf("\nEnter your command : ");
        fgets(input,sizeof(input),stdin);

        processInput(input,&player,&playerState);//to make

        updateGameState(&player);

        //make a universal condition to end,save and load the the game....
    }
}