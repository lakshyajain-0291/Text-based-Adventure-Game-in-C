/*
12345
13789
21000
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

void sound(int n);
int dig(int n, int num, int i)
{
    return (int)(num / pow(10, n - i - 1)) % 10;
}
void wordlegame(int n, int num, int guess)
{
    for(int i=0; i<n; i++)
    {
        int digG = dig(n, guess, i), count=1;
        for(int j=0; j<n; j++)
        {
            int digN = dig(n, num, j);
            if(digG==digN && i==j)
            {
                printf("\x1b[32m2\x1b[0m");
                count=0;
                break;
            }
            else if(digG==digN && i!=j)
            {
                printf("\x1b[33m1\x1b[0m");
                count=0;
                break;
            }
        }
        if(count==1) printf("\x1b[31m0\x1b[0m");
    }
    printf("\n");
}
int playWordle(int n, int attempts)
{
    system("clear");
    printf("\n        \x1b[33m");
    char welcome[100]="WELCOME TO THE WORDLE GAME";
    for(int i=0;i<strlen(welcome);i++)
    {
        putchar(welcome[i]);
        fflush(stdout);
        usleep(100000);
    }
    printf("\x1b[0m");
    printf("\n\n\n");
    int num;
    srand(time(0));
    for(int i=0;i<1000;i++)
     rand();
    while(num<(int)pow(10, n-1) || num>=(int)pow(10, n))
    {
        num = rand()%(int)(pow(10, n));
    }
    int guess;
    printf(" __________________________________________                \x1b[33m     1 - Number is present but not at the accurate location\x1b[0m\n");
    printf("|                                                          \x1b[32m     2 - Number is present and is at the accurate location\x1b[0m\n");
    printf("|                                                          \x1b[31m     0 - Number is not present in the string \x1b[0m\n");
    while (attempts > 0)
    {
        //printf("\033[K");
        printf("|       Guess the 5 digit number number: ");
        scanf("%d", &guess);
        printf("|\x1b[34m                       Encrypted Code:  \x1b[0m");
        if (guess == num)
        {
            for(int i=0; i<n; i++) {
                printf("\x1b[32m2\x1b[0m");
            }
            printf("\n");

            return 1;
        }
        wordlegame(n, num, guess);
        attempts--;
        printf("|                                          \n");
        printf("|\x1b[36m       Number of attempts left: %d\n\x1b[0m",attempts);
        printf("|                                          \n");
    }
    printf("|       The number was %d\n", num);
    return 0;
}

int main()
{
    sound(0);
    int n = 5, attempts = 5, resWordle;
    resWordle = playWordle(n, attempts);

    if (resWordle == 1)
    { 
        sound(1);
        printf("|\x1b[32m       Good Game!!!\n\x1b[0m");
        sleep(2);
    }
    else if (resWordle == 0)
        printf("|\x1b[31m       You Lose!!\n\x1b[0m");
    
    printf("|__________________________________________");
    system("killall afplay");
    return 0;
}
void sound(int n)
{
    const char *soundFilePath;
  if(n==0)
   soundFilePath = "./Sound Files/wordle_score.mp3"; //Music
  if(n==1)
   soundFilePath = "./Sound Files/yes_man.mp3"; 
  char command[300];
  snprintf(command, sizeof(command), "afplay \"%s\" &", soundFilePath);
  system(command);
}
