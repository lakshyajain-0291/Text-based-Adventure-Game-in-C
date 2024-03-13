/*
Initiate grid (6*6)


Player Position 
*/

#include<stdio.h>
typedef struct cell {
    int x, y;
} cell_pos;

void result(int size, int prison[2*size][2*size], cell_pos exit, cell_pos pl_pos, cell_pos guard_pos)
{
    if(pl_pos.x == guard_pos.x && pl_pos.y == guard_pos.y)
    {
        //CAPTURE
        return;
    }

    if(pl_pos.x == exit.x && pl_pos.y == exit.y)
    {
        //ESCAPE
        return;
    }
}

void move(int size, int prison[2*size][2*size], cell_pos *pl_pos, char pl_move, cell_pos exit)
{
    switch (pl_move)
    {
    case 'W':
        if(pl_pos->y + 2 < 2*size && prison[pl_pos->x][(pl_pos->y)+1] == 0)
        {
            pl_pos->y += 2;
        }
        break;

    case 'A':
        if(pl_pos->x - 2 >= 0)
        {
            pl_pos->x += -2;
        }
        break;

    case 'S':
        if(pl_pos->y - 2 >= 2)
        {
            pl_pos->y += -2;
        }
        break;

    case 'D':
        if(pl_pos->x + 2 < 2*size)
        {
            pl_pos->x += 2;
        }
        break;

    default:
        break;
    }
}

void playermove(int size, int prison[2*size][2*size], cell_pos *pl_pos, cell_pos guard_pos, cell_pos exit)
{
    char pl_move;
    scanf("%c", &pl_move);
    move(size, prison, pl_pos, pl_move, exit);
    result(size, prison, exit, *pl_pos, guard_pos);
}

void guardmove(int size, int prison[2*size][2*size], cell_pos *pl_pos, cell_pos *guard_pos, cell_pos exit)//guard's move's algo
{
    int count=0;
    char guard_move;
    for(int i=0; i<2 && count<2; i++)
    {
        if((pl_pos->x > guard_pos->x && pl_pos->y == guard_pos->y) && prison[(guard_pos->x)+1][guard_pos->y]==0) 
        {
            guard_move='D';
            move(size, prison, guard_pos, guard_move, exit);
            result(size, prison, exit, *pl_pos, *guard_pos);    
            count++;
        }

        else if((pl_pos->x < guard_pos->x && pl_pos->y == guard_pos->y) && prison[(guard_pos->x)-1][guard_pos->y]==0) 
        {
            guard_move='A';
            move(size, prison, guard_pos, guard_move, exit);
            result(size, prison, exit, *pl_pos, *guard_pos);    
            count++;
        }

        else if((pl_pos->x == guard_pos->x && pl_pos->y > guard_pos->y) && prison[(guard_pos->x)][(guard_pos->y)+1]==0) 
        {
            guard_move='W';
            move(size, prison, guard_pos, guard_move, exit);
            result(size, prison, exit, *pl_pos, *guard_pos);    
            count++;
        }

        else if((pl_pos->x == guard_pos->x && pl_pos->y > guard_pos->y) && prison[(guard_pos->x)][(guard_pos->y)-1]==0) 
        {
            guard_move='S';
            move(size, prison, guard_pos, guard_move, exit);
            result(size, prison, exit, *pl_pos, *guard_pos);    
            count++;
        }
    }
}

void guard();
void makePrison(int size, int prison[2*size][2*size])
{
    //walk area
    for(int i=0; i<2*size; i=i+2)
        for(int j=0; j<2*size; j=j+2)
            prison[i][j]=0;

    //wall input        
    for(int i=1; i<2*size; i=i+2)
        for(int j=1; j<2*size; j=j+2)
        {
            prison[i][j];
        }
}

void OPplayPrisonEscape()
{
    int size;
    int prison[size*2][size*2];

    cell_pos exit;
    cell_pos pl_spawn;
    cell_pos guard_spawn;

    cell_pos pl_pos = pl_spawn;
    cell_pos guard_pos = guard_spawn;

    makePrison(size, prison);
}
int main() {

}

