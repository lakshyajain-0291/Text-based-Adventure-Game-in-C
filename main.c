#include "ICS_Project.h"
// #include"cJSON.c"


int state;

int main()
{
    printf("\e[1;1H\e[2J");

    char *playerID = (char *)malloc(sizeof(char) * MAX_PLAYER_ID_LENGTH);
    if (playerID == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    printStory("\nEnter Login ID : ",BYEL,HIG);
    if (scanf("%s", playerID) != 1)
    {
        fprintf(stderr, "Error reading player ID\n");
        free(playerID);
        return EXIT_FAILURE;
    }
    flushInputBuffer();

    if(strcmp(playerID,"items")==0||strcmp(playerID,"dialogue")==0||strcmp(playerID,"locations")==0||strcmp(playerID,"navigations")==0||strcmp(playerID,"characters")==0||strcmp(playerID,"quests")==0)
    {
        printStory("You have spoken with the venom of an EVIL tongue.",BRED,HIG);
        printStory("\nTHE END",BRED,HIG); /// EXIT STATEMENT
        printStory("\nThank you for playing ",BYEL,HIG); /// EXIT STATEMENT
        printStory("THE VINDICATION",BHRED,HIG); /// EXIT STATEMENT
        return 0;
    }
    Player *player = NULL;

    // printRules();
    printf("\e[1;1H\e[2J");
    
    // printStory("Press Enter to Continue", YEL, MED);
    printStory("\nRules:\n\n", BHMAG, HIG);
    printStory("1. Traverse the realms of navigation (n), questing (q), and interaction (i), \neach representing a facet of protagonist's journey through tragedy and redemption. \nAs a fleeting respite from the harsh realities of Marcus's world, \nthe option to exit (e) serves as a reminder of the possibility, \nalbeit tragic, of escape from one's past through the ultimate act.\n\n", BHMAG, LOW);
    printStory("2. Navigate the intricate landscape of Marcus's world, \nwhere each turn and decision holds the potential to alter his path forever. \nFrom winding streets to hidden passages, explore the depths of navigation (n) mode \nand chart a course through the twists and turns of fate.\n\n", BHMAG, LOW);
    printStory("3. Engage with characters who embody the trials and tribulations of Marcus's existence, \ndelving into the depths of human suffering and forging connections in a world haunted by tragedy. \nNavigate through the intricate web of relationships and dilemmas, \neach interaction (i) a mirror reflecting Marcus's internal turmoil and external conflicts.\n\n", BHMAG, LOW);
    printStory("4. Undertake quests as allegories for Marcus's struggles, \nconfronting demons both internal and external as he seeks redemption in the face of overwhelming adversity. \nLet each quest be a stepping stone on Marcus's journey towards reconciliation and absolution, \na testament to the resilience of the human spirit amidst the darkest of trials.\n\n", BHMAG, LOW);
    printStory("5. In the protagonist's world, missteps and failed quests can lead to a grim end. \nWrong choices or unfulfilled quests may spell death, \nsending the protagonist back to the city with only their sword, and their XP and gold reset. \nBut even in death, redemption's spark remains, \noffering the protagonist a chance to rewrite their fate until they themselves choose to depart (e).\n\n",BHMAG,LOW);
    printStory("6. Unveil the depths of your strength and the treasures of your inventory by invoking the letter 'p'. \nDelve into the essence of your character, \nwhere each item and attribute tells a story of your journey's trials and triumphs.\n\n",BHMAG,LOW);
    // printStory(rules, HYEL, 10);

    player = gameInitializer(playerID);
    if (player == NULL)
    {
        fprintf(stderr, "Failed to initialize player\n");
        free(playerID);
        return EXIT_FAILURE;
    }
    // printStory("\nid-%s",player->id,31,0);
    // printf("\nname-%s",player->name);
    // printf("\nlevel-%d",player->level);
    // printf("\nstats,hp-%d",player->stats->HP);
    // printf("\ninventory,items[0]-%s",player->inventory->items[0]);
    // printf("\ninventory,aitems[0]-%d",player->inventory->activeItems[0]);
    // printf("\ngold-%d",player->gold);
    // printf("\ncur_loc-%s",player->currentLocation);
    // printf("\nact_que[0]-%s",player->activeQuests[0]);
    // printf("\nnpcInfo[0][0]-%d",player->NPCInfo[0][0]);
//     printPrologue();

    printf("\n");
    selectState(player,&state); // function that can be called from anywhere by player to choose a state
    while (state >= 0)
        processState(player, &state); // function that processes the state

    printStory("\nTHE END",BRED,HIG); /// EXIT STATEMENT
    printStory("\nThank you for playing ",BYEL,HIG); /// EXIT STATEMENT
    printStory("THE VINDICATION",BHRED,HIG); /// EXIT STATEMENT

    savePlayerData(player);
    // Cleanup
    freePlayer(player);
    return EXIT_SUCCESS;
}
