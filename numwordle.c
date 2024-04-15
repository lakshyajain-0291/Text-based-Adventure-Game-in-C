/*
12345
13789
21000
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
                printf("2");
                count=0;
                break;
            }
            else if(digG==digN && i!=j)
            {
                printf("1");
                count=0;
                break;
            }
        }
        if(count==1) printf("0");
    }
    printf("\n");
}
int playWordle(int n, int attempts)
{
    system("clear");
    int num;
    while(num<(int)pow(10, n-1) || num>=(int)pow(10, n))
    {
        num = rand()%(int)(pow(10, n));
    }
    int guess;
    while (attempts > 0)
    {
        printf("Guess the number:\n");
        scanf("%d", &guess);
        if (guess == num)
        {
            for(int i=0; i<n; i++) {
                printf("2");
            }
            printf("\n");

            return 1;
        }
        wordlegame(n, num, guess);
        attempts--;
    }
    printf("The number was %d\n", num);
    return 0;
}

int main()
{
    int n = 5, attempts = 5, resWordle;
    resWordle = playWordle(n, attempts);

    if (resWordle == 1)
        printf("GG!\n");
    else if (resWordle == 0)
        printf("You Lose!\n");
}
