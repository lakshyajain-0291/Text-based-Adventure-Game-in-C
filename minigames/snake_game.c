#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/select.h>
#include<termios.h>
#include<fcntl.h>
 
struct body{
    int x;
    int y;
    struct body* next;
};
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m" // Reset to default color

void draw(int i, int j, char *ptr,char arr[30][60]) 
{
    if (i <= 29) 
    {
        if ((i == 0 || i == 29) && j != 0 && j != 59) 
        {
            *ptr = '-';
        } 
        else if ((j == 0 || j == 59) && i != 0 && i != 29) 
        {
            *ptr = '|';
        } 
        else 
        {
            *ptr= ' ';
        }
        if (j <59) 
        {
            j+=1;
            ptr++;
            draw(i, j, ptr,arr); 
        } 
        else 
        {
            if(i<29)
            {
                i+=1;
                j=0;
                ptr=&arr[i][j];
                draw(i, j, ptr,arr); 
            }
        }
    }
}
void compartment(char arr[30][60])
{
    draw(0,0,&arr[0][0],arr);
    for(int i=8;i<10;i++)
    {
        for(int j=9;j<50;j++)
        {
            arr[i][j]='*';
        }
    }
    for(int i=21;i<23;i++)
    {
        for(int j=9;j<50;j++)
        {
            arr[i][j]='*';
        }
    }
    for(int i=1;i<7;i++)
    {
        arr[i][1]='*';
        arr[i][58]='*';
    }
    for(int i=24;i<29;i++)
    {
        arr[i][1]='*';
        arr[i][58]='*';
    }
    for(int j=2;j<9;j++)
    {
        arr[1][j]='*';
        arr[28][j]='*';
    }
    for(int j=51;j<58;j++)
    {
        arr[1][j]='*';
        arr[28][j]='*';
    }
}


int generate(int number)
{
    srand(time(NULL));
    return (rand()%(number-1)+1);
}

int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    
    return 0;
}

int w(char arr[30][60],int *score,int *sp,struct body* tail)
{
    struct body *current=tail;
    while(current->next!=NULL)
    {
        current=current->next;
    }
    if(arr[current->x-1][current->y]=='x')
    {
        struct body *t=(struct body*)malloc(sizeof(struct body));
        t->x=current->x-1;
        t->y=current->y;
        t->next=NULL;
        current->next=t;
        arr[t->x][t->y]='0';
        *score+=2*(*sp);
        int g1=generate(29);
        int g2=generate(59);
        while(arr[g1][g2]=='0' || arr[g1][g2]=='*'||arr[g1-1][g2]=='0')
        {
            g1=generate(29);
            g2=generate(59);
        }
        arr[g1][g2]='x';
        return 1;
    }

    else if(arr[current->x-1][current->y]!='*' && arr[current->x-1][current->y]!='0')
    {
        if(arr[current->x-1][current->y]=='-')
        {
            struct body *tmp=tail;
            arr[tail->x][tail->y]=' ';
            while(tmp->next!=NULL)
            {
                tmp->x=tmp->next->x;
                tmp->y=tmp->next->y;
                tmp=tmp->next;
            }
            tmp->x=28;
            arr[tmp->x][tmp->y]='0';
        }
        else
        {
            struct body *tmp=tail;
            arr[tail->x][tail->y]=' ';
            while(tmp->next!=NULL)
            {
                tmp->x=tmp->next->x;
                tmp->y=tmp->next->y;
                tmp=tmp->next;
            }
            tmp->x=tmp->x-1;
            arr[tmp->x][tmp->y]='0';
        }
        return 1;
    }
    else            
    {
        return 0;
    }
}

int a(char arr[30][60],int *score,int *sp,struct body* tail)
{
    struct body *current=tail;
    while(current->next!=NULL)
    {
        current=current->next;
    }
    if(arr[current->x][current->y-1]=='x')
    {
        struct body *t=(struct body*)malloc(sizeof(struct body));
        t->x=current->x;
        t->y=current->y-1;
        t->next=NULL;
        current->next=t;
        arr[t->x][t->y]='0';
        *score+=2*(*sp);
        int g1=generate(29);
        int g2=generate(59);
        while(arr[g1][g2]=='0' || arr[g1][g2]=='*' || arr[g1][g2+1]=='0')
        {
            g1=generate(29);
            g2=generate(59);
        }
        arr[g1][g2]='x';
        return 1;
    }

    else if(arr[current->x][current->y-1]!='*' && arr[current->x][current->y-1]!='0')
    {
        if(arr[current->x][current->y-1]=='|')
        {
            struct body *tmp=tail;
            arr[tail->x][tail->y]=' ';
            while(tmp->next!=NULL)
            {
                tmp->x=tmp->next->x;
                tmp->y=tmp->next->y;
                tmp=tmp->next;
            }
            tmp->y=58;
            arr[tmp->x][tmp->y]='0';
        }
        else
        {
            struct body *tmp=tail;
            arr[tail->x][tail->y]=' ';
            while(tmp->next!=NULL)
            {
                tmp->x=tmp->next->x;
                tmp->y=tmp->next->y;
                tmp=tmp->next;
            }
            tmp->y=tmp->y-1;
            arr[tmp->x][tmp->y]='0';
        }
        return 1;
    }
    else            
    {
        return 0;
    }
}

int s(char arr[30][60],int *score,int *sp,struct body* tail)
{
    struct body *current=tail;
    while(current->next!=NULL)
    {
        current=current->next;
    }
    if(arr[current->x+1][current->y]=='x')
    {
        struct body *t=(struct body*)malloc(sizeof(struct body));
        t->x=current->x+1;
        t->y=current->y;
        t->next=NULL;
        current->next=t;
        arr[t->x][t->y]='0';
        *score+=2*(*sp);
        int g1=generate(29);
        int g2=generate(59);
        while(arr[g1][g2]=='0' || arr[g1][g2]=='*' ||arr[g1+1][g2]=='0')
        {
            g1=generate(29);
            g2=generate(59);
        }
        arr[g1][g2]='x';
        return 1;
    }

    else if(arr[current->x+1][current->y]!='*' && arr[current->x+1][current->y]!='0')
    {
        if(arr[current->x+1][current->y]=='-')
        {
            struct body *tmp=tail;
            arr[tail->x][tail->y]=' ';
            while(tmp->next!=NULL)
            {
                tmp->x=tmp->next->x;
                tmp->y=tmp->next->y;
                tmp=tmp->next;
            }
            tmp->x=1;
            arr[tmp->x][tmp->y]='0';
        }
        else
        {
            struct body *tmp=tail;
            arr[tail->x][tail->y]=' ';
            while(tmp->next!=NULL)
            {
                tmp->x=tmp->next->x;
                tmp->y=tmp->next->y;
                tmp=tmp->next;
            }
            tmp->x=tmp->x+1;
            arr[tmp->x][tmp->y]='0';
        }
        return 1;
    }
    else            
    {
        return 0;
    }
}

int d(char arr[30][60],int *score,int *sp,struct body* tail)
{
    struct body *current=tail;
    while(current->next!=NULL)
    {
        current=current->next;
    }
    if(arr[current->x][current->y+1]=='x')
    {
        struct body *t=(struct body*)malloc(sizeof(struct body));
        t->x=current->x;
        t->y=current->y+1;
        t->next=NULL;
        current->next=t;
        arr[t->x][t->y]='0';
        *score+=2*(*sp);
        int g1=generate(29);
        int g2=generate(59);
        while(arr[g1][g2]=='0' || arr[g1][g2]=='*' || arr[g1][g2-1]=='0')
        {
            g1=generate(29);
            g2=generate(59);
        }
        arr[g1][g2]='x';
        return 1;
    }

    else if(arr[current->x][current->y+1]!='*' && arr[current->x][current->y+1]!='0')
    {
        if(arr[current->x][current->y+1]=='|')
        {
            struct body *tmp=tail;
            arr[tail->x][tail->y]=' ';
            while(tmp->next!=NULL)
            {
                tmp->x=tmp->next->x;
                tmp->y=tmp->next->y;
                tmp=tmp->next;
            }
            tmp->y=1;
            arr[tmp->x][tmp->y]='0';
        }
        else
        {
            struct body *tmp=tail;
            arr[tail->x][tail->y]=' ';
            while(tmp->next!=NULL)
            {
                tmp->x=tmp->next->x;
                tmp->y=tmp->next->y;
                tmp=tmp->next;
            }
            tmp->y=tmp->y+1;
            arr[tmp->x][tmp->y]='0';
        }

        return 1;
    }
    else            
    {
        return 0;
    }
}

void print(char arr[30][60],int *score)
{
    printf(YELLOW"\t\t THE REBEL'S CRY (SNAKE GAME)\n\n"RESET);
    for(int i=0;i<30;i++)
    {
        for(int j=0;j<60;j++)
        {
            if(arr[i][j]=='*')
            {
                printf(RED"%c"RESET,arr[i][j]);
            }
            else if(arr[i][j]=='0')
            {
                printf(YELLOW"%c"RESET,arr[i][j]);
            }
            else
            {
                printf("%c",arr[i][j]);
            }
        }
        printf("\n");
    }
    printf("Score : %d\n",*score);
}

int update(int data[10][3],int *m,int *score)
{
    int count=0;
    FILE *fptr;
    if(*m==1)
    {
        for(int i=0;i<10;i++)
        {
            if(*score>data[i][0])
            {
                count=1;
                for(int j=9;j>i+1;j--)
                {
                    data[j][0]=data[j-1][0];
                }
                data[i][0]=*score;
                break;
            }
        }
    }
    else if(*m==2)
    {
        for(int i=0;i<10;i++)
        {
            if(*score>data[i][1])
            {
                count=1;
                for(int j=9;j>i+1;j--)
                {
                    data[j][1]=data[j-1][1];
                }
                data[i][1]=*score;
                break;
            }
        }

    }
    else if(*m==3)
    {
        for(int i=0;i<10;i++)
        {
            if(*score>data[i][2])
            {
                count=1;
                for(int j=9;j>i+1;j--)
                {
                    data[j][2]=data[j-1][2];
                }
                data[i][2]=*score;
                break;
            }
        }

    }
    fptr = fopen("snake_game.csv", "w");
    for(int i=0;i<10;i++)
    {
        fprintf(fptr," %d %d %d\n",data[i][0],data[i][1],data[i][2]);
    }
    return count;

}

void play(char c,int *score,char arr[30][60],int *sp,struct body* h,int *m,int data[10][3])
{
    
    char temp=c;
    int cont=1;
    while(cont)
    {
        if(kbhit())
        {
            c=getchar();
        }
        if(c=='x')
        {
            system("clear");
            printf("Are you sure you want to quit\n y : Yes \t n : No");
            char t=getchar();
            if(t=='y')
            {
                break;
            }
            else
            {
                continue;
            }
        }
        else if(c=='w' && temp!='s' )
        {
                cont=w(arr,score,sp,h);
        }
        else if(c=='s' && temp!='w')
        {
                cont=s(arr,score,sp,h);
        }
        else if(c=='a' && temp!='d')
        {
                cont=a(arr,score,sp,h);
        }
        else if(c=='d' && temp!='a')
        {
                cont=d(arr,score,sp,h);
        }

        else
        {
            if(temp=='a')
            {
                cont=a(arr,score,sp,h);
            }
            else if(temp=='d')
            {
                cont=d(arr,score,sp,h);
            }
            else if(temp=='s')
            {
                cont=s(arr,score,sp,h);
            }
            else if(temp=='w')
            {
                cont=w(arr,score,sp,h);
            }
            else
            {
                continue;
            }
        }
        if(temp=='w'||temp=='s')
        {
            usleep(600000/(*sp));
        }
        else if(temp=='a'|| temp=='d')
        {
            usleep(300000/(*sp));
        }
        system("clear");
        print(arr,score);
        if((c=='w'&&temp!='s')||(c=='a'&&temp!='d')||(c=='s'&&temp!='w')||(c=='d'&&temp!='a'))
        {
            temp=c;
        }
    }
    system("clear");
    if(update(data,m,score))
    {
        printf(YELLOW "High Score!\n" RESET);
        printf(YELLOW "Your Score is : %d\n" RESET,*score); 
    }
    else{
        printf(YELLOW "GAME OVER... \n" RESET);
        printf(YELLOW "Your Score is : %d\n" RESET,*score);

    } 
    usleep(250000);
}
void enter()
{
    system("clear");
    char arr[]="WELCOME TO THE REBEL'S CRY (SNAKE GAME)..";
    printf("\t\t\t\t");
    for(int i=0;i<strlen(arr);i++)
    {
        printf(YELLOW"%c"RESET,arr[i]);
        fflush(stdout);
        usleep(150000);
    }
    printf("\n");
    system("clear");

}
void map(char arr[30][60],int *m)
{
    char ch;
    while(1)
    {
        system("clear");
        if(*m==1)
        {
            printf("1. Classic Box\n2. Compartment\n3. Apartment\n4. Back\n\ncurrent map: Compartment\n");
        }
        ch=getchar();
        if(ch=='1')
        {
            draw(0,0,&arr[0][0],arr);
            compartment(arr);
            *m=2;
        }
        else 
        {
            break;
        }
    }
}
void highscore(int data[10][3],int *m)
{
    char ch=' ';
    while(ch!='b')
    {
        if(*m==1)
        {
            printf("High Scores in the REBEL'S CRY SNAKE GAME:\n");
            for(int i=0;i<10;i++)
            {
                printf("%d. %d\n",i+1,data[i][0]);
            }
            printf("Back : b\n");
        }
        ch=getchar();
    }
}
void help()
{
    printf("This game contains several maps.\n Your aim should be to collect as many fruits as possible until the game is over.\n");
    printf("1. The movements of the snake is caused by pressing  the keys w,s,a,d\n");
    printf("2. Pressing any other key will not cause any change in motion\n");
    printf("3. When the snake is  moving, you cannot change the motion of the snake directly in the opposite direction.\n  You need to moving in either of the remianing direction and then move backwards\n");
    printf("4. If the snake goes at the white boundaries, it will come from opposite side and move in same direction.\n");
    printf("5. The game gets over when the snake either hits its own body or it hits the red walls\n");
    printf("6. To eat the fruit, move over the fruit . The length of the snake gets increased by 1 unit.\n\n");
    printf("7. To change the map, go to the maps option. There are 3 maps available");
    printf("8. To increase the speed go to speed option and increase it or decrease it ");
    printf("   ...press any key and enter to go back\n");
    char c=getchar();
}
void sound(int n)
{
  const char *soundFilePath;
  if(n==0)
   soundFilePath = "./Sound Files/snakeGameBg.mp3"; //Music
  if(n==1)
   soundFilePath = "./Sound Files/beep.mp3"; 
  if(n==2)
   soundFilePath = "./Sound Files/yes_man.mp3"; 
  if(n==3)
   soundFilePath = "./Sound Files/wasted.mp3";
  char command[300];
  snprintf(command, sizeof(command), "afplay \"%s\" &", soundFilePath);
  system(command);
}
int starting()
{
    sound(0);
    int data[10][3]={{0}};
    int m=1, speed=2, i=0;
    char t='y';
    while(t=='y')        //The game runs while the condition is yes 
    {
        struct body* tail=(struct body*)malloc(sizeof(struct body));
        tail->x=15;
        tail->y=30;
        tail->next=NULL;
        char arr[30][60];
        if(i==0)
        {
            enter();
        }
        fflush(stdin);
        compartment(arr);
        int g1=generate(29);
        int g2=generate(59);
        arr[g1][g2]='x';
        if(g1==15 && g2==30)
        {
            arr[g1][g2]=' ';
            int g1=generate(29);
            int g2=generate(59);
            arr[g1][g2]='x';
        }
        arr[15][30]='0';
        int score=0;
        char c=' ';
        system("clear");
        if(1)
        {
            print(arr,&score);
            play(c,&score,arr,&speed,tail,&m,data);
            system("killall afplay");
            if(score >10)
             return 0;
            else 
             return 1;
        }
        i++;
        system("clear");
        struct body* temp=tail;
        while(temp->next!=NULL)
        {
            struct body* t=temp;
            t=temp;
            temp=temp->next;
            free(t);
        }
        free(temp);
    }  
    return 0;
}


int main()
{
    int x=starting();
    
}

