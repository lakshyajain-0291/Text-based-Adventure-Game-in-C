/*
Player Input: H/B/L H/B/L;
Player attack & Player Defense.
*/

#include <stdio.h>
#include <stdlib.h>
void print_combat(int plStamina, int oppStamina)
{
    printf("player stamina = %d opponent Stamina = %d\n", plStamina, oppStamina);
    printf("\n\n");
}
void game(int plWeapon, int *plStamina, int oppWeapon, int *oppStamina, int pl_att, int pl_def, int opp_att, int opp_def)
{

    if (pl_att != opp_def)
        *oppStamina -= plWeapon * pl_att;
    if (*oppStamina <= 0)
        return;
    if (opp_att != pl_def)
        *plStamina -= oppWeapon * opp_att;
    if (*plStamina <= 0)
        return;
}
void Move(int plWeapon, int *plStamina, int oppWeapon, int *oppStamina)
{
    char pl1, pl2;
    int pl_att, pl_def, opp_att, opp_def;
    scanf("%c %c", &pl1, &pl2);

    int val1 = rand() % (100), val2 = rand() % (100), val3 = rand() % (100);
    if (val1 <= 33)
        opp_att = 4;
    else if (val1 <= 66 && val1 > 33)
        opp_att = 2;
    else
        opp_att = 1;

    if (val2 <= 33)
        opp_def = 4;
    else if (val2 <= 66 && val2 > 33)
        opp_def = 2;
    else
        opp_def = 1;

    if (pl1 == 'H')
        pl_att = 4;
    else if (pl1 == 'B')
        pl_att = 2;
    else if (pl1 == 'L')
        pl_att = 1;

    if (pl2 == 'H')
        pl_def = 4;
    else if (pl2 == 'B')
        pl_def = 2;
    else if (pl2 == 'L')
        pl_def = 1;

    game(plWeapon, plStamina, oppWeapon, oppStamina, pl_att, pl_def, opp_att, opp_def);

    printf("Player Attacks %c\t", pl1);
    printf("Player Defends %c\n", pl2);
    printf("Opponent Attacks ");
    if (opp_att == 1)
        printf("L\t");
    if (opp_att == 2)
        printf("B\t");
    if (opp_att == 4)
        printf("H\t");

    printf("Opponent Defends ");
    if (opp_def == 1)
        printf("L\n");
    if (opp_def == 2)
        printf("B\n");
    if (opp_def == 4)
        printf("H\n");
}

int playCombat(int plWeapon, int plStamina, int oppWeapon, int oppStamina)
{
    while (1)
    {
        Move(plWeapon, &plStamina, oppWeapon, &oppStamina);
        print_combat(plStamina, oppStamina);
        if (oppStamina <= 0)
        {
            printf("You Win!\n");
            return 1;
        }
        if (plStamina <= 0)
        {
            printf("You Lose!");
            return 0;
        }
    }
}
int start_combat()
{
    printf("Welcome to Combat Game\n\n");
    printf("Enter Player Move: H(head)/B(body)/L(leg) & H/B/L for Player Attack and Defense\t");
    int plWeapon = 15, plStamina = 100, oppWeapon = 10, oppStamina = 100;
    playCombat(plWeapon, plStamina, oppWeapon, oppStamina);
    return 0;
}

int main()
{
    int x=start_combat();
    return x;
}
