#include <stdio.h>

typedef struct cell
{
    int x, y;
} cell_pos;

void print_prison(int size,int i,int j,int k);
//Known Error: X is Y, Y is X. Needs extensive UI changes
void print(int size, int prison[2 * size][2 * size], cell_pos exit, cell_pos pl_pos, cell_pos guard_pos)
{
    for (int i = 0; i < 2 * size; i++)
    {
        for (int j = 0; j < 2 * size; j++)
        {
            if (pl_pos.x == i && pl_pos.y == j)
            {
                printf("\x1b[33m"); //Yellow
                printf("P ");
                printf("\x1b[0m");  //Reset
            }
            else if (guard_pos.x == i && guard_pos.y == j)
            {
                printf("\x1b[31m");
                printf("G ");
                printf("\x1b[0m");
            }
            else if (exit.x == i && exit.y == j)
            {
                printf("\x1b[32m");
                printf("E ");
                printf("\x1b[0m");
            }
            else if((i==0)||(j==0)||(i==2*size-1)||(j==2*size-1)||(i%2!=0)||(j%2!=0))
             print_prison(size,i,j,prison[i][j]);
            
            else
            {
                printf("%d ", prison[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n\n");
}

void result(int size, int prison[2 * size][2 * size], cell_pos exit, cell_pos pl_pos, cell_pos guard_pos)
{
    if (pl_pos.x == guard_pos.x && pl_pos.y == guard_pos.y)
    {
        printf("Guard caught you!\n");
        return;
    }

    if (pl_pos.x == exit.x && pl_pos.y == exit.y)
    {
        printf("Congratulations! You escaped!\n");
        return;
    }
}
void move(int size, int prison[2 * size][2 * size], cell_pos *pl_pos, char pl_move, cell_pos exit)
{
    switch (pl_move)
    {
    case 'W':
        if (pl_pos->y + 2 < 2 * size && prison[pl_pos->x][(pl_pos->y) + 1] == 0)
        {
            pl_pos->y += 2;
        }
        break;

    case 'A':
        if (pl_pos->x - 2 >= 0 && prison[(pl_pos->x) - 1][(pl_pos->y)] == 0)
        {
            pl_pos->x += -2;
        }
        break;

    case 'S':
        if (pl_pos->y - 2 >= 0 && prison[(pl_pos->x)][(pl_pos->y) - 1] == 0)
        {
            pl_pos->y += -2;
        }
        break;

    case 'D':
        if (pl_pos->x + 2 < 2 * size && prison[(pl_pos->x) + 1][(pl_pos->y)] == 0)
        {
            pl_pos->x += 2;
        }
        break;

    default:
        break;
    }
}

void playermove(int size, int prison[2 * size][2 * size], cell_pos *pl_pos, cell_pos guard_pos, cell_pos exit)
{
    char pl_move;
    printf("Enter your move (W/A/S/D): ");
    scanf(" %c", &pl_move);
    move(size, prison, pl_pos, pl_move, exit);
    print(size, prison, exit, *pl_pos, guard_pos);
    result(size, prison, exit, *pl_pos, guard_pos);
}

void guardmove(int size, int prison[2 * size][2 * size], cell_pos *pl_pos, cell_pos *guard_pos, cell_pos exit) // guard's move's algo
{
    int count = 0;
    char guard_move;
    for (int i = 0; i < 2 && count < 2; i++)
    {
        if (pl_pos->x > guard_pos->x && prison[(guard_pos->x) + 1][guard_pos->y] == 0)
        {
            guard_move = 'D';
            move(size, prison, guard_pos, guard_move, exit);
            print(size, prison, exit, *pl_pos, *guard_pos);
            result(size, prison, exit, *pl_pos, *guard_pos);
            count++;
        }

        else if (pl_pos->x < guard_pos->x && prison[(guard_pos->x) - 1][guard_pos->y] == 0)
        {
            guard_move = 'A';
            move(size, prison, guard_pos, guard_move, exit);
            print(size, prison, exit, *pl_pos, *guard_pos);
            result(size, prison, exit, *pl_pos, *guard_pos);
            count++;
        }

        else if ((pl_pos->x == guard_pos->x && pl_pos->y > guard_pos->y) && prison[(guard_pos->x)][(guard_pos->y) + 1] == 0)
        {
            guard_move = 'W';
            move(size, prison, guard_pos, guard_move, exit);
            print(size, prison, exit, *pl_pos, *guard_pos);
            result(size, prison, exit, *pl_pos, *guard_pos);
            count++;
        }

        else if ((pl_pos->x == guard_pos->x && pl_pos->y < guard_pos->y) && prison[(guard_pos->x)][(guard_pos->y) - 1] == 0)
        {
            guard_move = 'S';
            move(size, prison, guard_pos, guard_move, exit);
            print(size, prison, exit, *pl_pos, *guard_pos);
            result(size, prison, exit, *pl_pos, *guard_pos);
            count++;
        }
    }
}

void makePrison(int size, int prison[2 * size][2 * size])
{
    if (size == 8)
    {
        for (int i = 0; i < 2 * size; i++)
        {
            for (int j = 0; j < 2 * size; j++)
            {
                prison[i][j] = 0;
            }
        }
        prison[0][11] = 1;
        prison[1][0] = 1;
        prison[2][3] = 1;
        prison[3][8] = 1;
        prison[3][10] = 1;
        prison[4][7] = 1;
        prison[4][11] = 1;
        prison[5][4] = 1;
        prison[5][6] = 1;
        prison[5][14] = 1;
        prison[6][3] = 1;
        prison[6][5] = 1;
        prison[6][7] = 1;
        prison[7][2] = 1;
        prison[7][14] = 1;
        prison[8][9] = 1;
        prison[8][11] = 1;
        prison[10][3] = 1;
        prison[10][7] = 1;
        prison[11][2] = 1;
        prison[11][4] = 1;
        prison[11][6] = 1;
        prison[11][12] = 1;
        prison[13][0] = 1;
        prison[13][6] = 1;
        prison[13][8] = 1;
        prison[13][12] = 1;
        prison[14][3] = 1;
        prison[14][9] = 1;
    }
}

int playPrisonEscape(int size)
{
    int prison[size * 2][size * 2];
    makePrison(size, prison);
    cell_pos exit = {14,8};
    cell_pos pl_pos = {10, 10};
    cell_pos guard_pos = {6, 4};
    print(size, prison, exit, pl_pos, guard_pos);

    while (1)
    {
        playermove(size, prison, &pl_pos, guard_pos, exit);
        if (pl_pos.x == exit.x && pl_pos.y == exit.y)
        {
            return 1;
            break; // player escaped
        }
        guardmove(size, prison, &pl_pos, &guard_pos, exit);
        if (pl_pos.x == guard_pos.x && pl_pos.y == guard_pos.y)
        {
            return 0;
            break; // guard caught player
        }
    }
}

int main()
{
    int size;
    printf("Enter the size of the prison: ");
    scanf("%d", &size);
    playPrisonEscape(size);
    return 0;
}

void print_prison(int size,int i,int j,int k)
{
    
    if(i==0)
     {
        if(j==2*size-1)
         printf("_");
        else
         printf("__");
     }
    else if(j==0)
    {
        if(i==2*size-1)
         printf("|_");
        else
         printf("| ");
    }
    else if(j==2*size-1)
     printf("|");
    else if(i==2*size-1)
     printf("__");
    else if((i%2!=0||j%2!=0)&& k==1)
    {
        printf("\x1b[36m");
        printf("& ");
        printf("\x1b[0m");
    }
    else if((i%2!=0||j%2!=0)&& k==0)
     printf("  ");
            
        
    
}
