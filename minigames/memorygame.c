/*
A matrix of 4*4 is randomly generated with integers 12345678, each occuring twice.
User clicks to hashes. Both get revealed. If both correspond to same number, progress++, and both numbers get revealed throughout
If not, attempts--. 
To win: User has to reveal all numbers within the given attempts or he loses.
*/

/*
Progress:
    completed without UI
*/

#include <windows.h>
#include <stdio.h>
#include <time.h>

// Function to delay output on terminal
void delay(float number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * (int)number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

// Function to take Mouse Input (Output = coordinates of Mouse Click on Terminal)
void MouseControl(int *x, int *y)
{
    HANDLE hStdin;
    DWORD fdwMode;
    INPUT_RECORD irInBuf[128];
    DWORD cNumRead;

    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "Error: Unable to get standard input handle\n");
        return;
    }

    // Enable mouse input events
    fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT;
    if (!SetConsoleMode(hStdin, fdwMode))
    {
        fprintf(stderr, "Error: Unable to set console mode\n");
        return;
    }

    while (1)
    {
        if (!ReadConsoleInput(hStdin, irInBuf, 128, &cNumRead))
        {
            fprintf(stderr, "Error: Unable to read console input\n");
            return;
        }

        for (DWORD i = 0; i < cNumRead; i++)
        {
            if (irInBuf[i].EventType == MOUSE_EVENT)
            {
                MOUSE_EVENT_RECORD mouseEvent = irInBuf[i].Event.MouseEvent;
                if (mouseEvent.dwEventFlags == 0 && mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
                {
                    *x = mouseEvent.dwMousePosition.X;
                    *y = mouseEvent.dwMousePosition.Y;
                    printf("Mouse clicked at (%d, %d)\n", mouseEvent.dwMousePosition.X, mouseEvent.dwMousePosition.Y);
                    return;
                }
            }
        }
    }
}

// Function to swap two integers
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function to shuffle an array
void shuffle_array(int arr[], int n)
{
    srand(time(NULL)); // Seed for random number generator
    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1); // Generate random index from 0 to i
        swap(&arr[i], &arr[j]);   // Swap elements at i and j
    }
}

// Function to generate the random matrix
void genMat(int size, int num, int mat[size][size])
{
    int arr[num];
    int count = 0;
    for (int i = 0; i < num; i++)
        arr[i] = i + 1;
    shuffle_array(arr, num);
    while (count < num)
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size / 2; j++)
            {
                mat[i][j] = arr[count];
                count++;
            }
        }
    count = 0;
    shuffle_array(arr, num);
    while (count < num)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = size / 2; j < size; j++)
            {
                mat[i][j] = arr[count];
                count++;
            }
        }
    }
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
            printf("%d ", mat[i][j]);
        printf("\n");
    }
    delay(0.5);
}

// Functions for terminal display (Need UI Change)
void printMemory1(int size, int num, int mat[size][size], int prog_arr[num])
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            int k;
            for (k = 0; k < num; k++)
            {
                if (mat[i][j] == prog_arr[k])
                {
                    break;
                }
            }
            if (k != num)
                printf("%d ", mat[i][j]);
            else
                printf("# ");
        }
        printf("\n");
    }
}

void printMemory2(int size, int num, int mat[size][size], int prog_arr[num], int click1_x, int click1_y, int click2_x, int click2_y)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            int k;
            for (k = 0; k < num; k++)
            {
                if (mat[i][j] == prog_arr[k])
                {
                    break;
                }
            }
            if (k != num)
                printf("%d ", mat[i][j]);

            else if (i == click1_y && j == click1_x / 2)
            {
                printf("%d ", mat[i][j]);
            }
            else if (i == click2_y && j == click2_x / 2)
            {
                printf("%d ", mat[i][j]);
            }
            else
                printf("# ");
        }
        printf("\n");
    }
}

// Main Game function
void gameMemory(int size, int num, int mat[size][size], int *attempts, int prog_arr[num], int *progress)
{
    printf("\e[1;1H\e[2J");
    printMemory1(size, num, mat, prog_arr);
    int click1_x, click1_y, click2_x, click2_y;

    // taking in input through mouse
    MouseControl(&click1_x, &click1_y);
    MouseControl(&click2_x, &click2_y);

    // If clicks correspond to same number, progress
    if (mat[click1_y][click1_x / 2] == mat[click2_y][click2_x / 2])
    {
        prog_arr[*progress] = mat[click1_y][click1_x / 2];
        (*progress)++;
        (*attempts)++;
    }

    printMemory2(size, num, mat, prog_arr, click1_x, click1_y, click2_x, click2_y);
    delay(2);
}

// Function to call game
int playMemory(int size, int num, int attempts)
{
    int mat[size][size], progress = 0;
    int prog_arr[num];
    for (int i = 0; i < num; i++)
        prog_arr[i] = 0;
    genMat(size, num, mat);
    while (1)
    {

        gameMemory(size, num, mat, &attempts, prog_arr, &progress);
        attempts--;
        printf("Progress = %d\nAttempts Remaining = %d\n ", progress, attempts);
        delay(1);
        if (attempts <= 0)
            return 0;
        if (progress == num)
            return 1;
    }
}

int main()
{
    int size = 4, attempts = 15;
    int resMemory = playMemory(size, size * size / 2, attempts);
    if (resMemory == 0)
        printf("You Lose!\n");
    else if (resMemory == 1)
        printf("You Win!!\n");
}
