#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>

// Function to delay output on terminal
void delay(float number_of_seconds)
{
    // Converting time into milliseconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time---
    clock_t start_time = clock();

    // Looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}
int z=0;  //To prevent printing unecessary Mouse click in the beginning
// Function to control mouse input
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
                    if(z==0)
                    {
                        z=1;
                        return;
                    }
                    printf("Mouse clicked at (%d, %d)\n", mouseEvent.dwMousePosition.X, mouseEvent.dwMousePosition.Y);
                    return;
                }
            }
        }
    }
}

// Function to generate the minefield
void genField(int size, int **field)
{
    // Initialize random number generator
    srand(time(NULL));

    // Allocate memory for the minefield
    *field = (int *)malloc(size * size * sizeof(int));

    // Initialize field with 0s
    for (int i = 0; i < size * size; i++)
    {
        (*field)[i] = 0;
    }

    // Generate mines
    int mines = size * size / 6; // Adjust the density of mines as needed
    for (int k = 0; k < mines; k++)
    {
        int x = rand() % size;
        int y = rand() % size;
        if((*field)[x*size +y]!=-1)
         (*field)[x * size + y] = -1; // -1 represents a mine
        else 
         k--;
    }

    // Calculate the number of mines in proximity for each cell
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if ((*field)[i * size + j] != -1)
            { // If the cell is not a mine
                int count = 0;
                for (int dx = -1; dx <= 1; dx++)
                {
                    for (int dy = -1; dy <= 1; dy++)
                    {
                        int nx = i + dx;
                        int ny = j + dy;
                        if (nx >= 0 && nx < size && ny >= 0 && ny < size && (*field)[nx * size + ny] == -1)
                        {
                            count++;
                        }
                    }
                }
                (*field)[i * size + j] = count;
            }
        }
    }
}

// Function to reveal clicked position
void reveal(int size, int x, int y, int *field, int *revealed)
{
    if (x < 0 || x >= size || y < 0 || y >= size || revealed[x * size + y])
        return;

    revealed[x * size + y] = 1;

    if (field[x * size + y] == 0)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                reveal(size, x + dx, y + dy, field, revealed);
            }
        }
    }
}

// Function to flag or unflag clicked position
void flag(int size, int x, int y, int *field, int *revealed, int *flag_count)
{
    if (x < 0 || x >= size || y < 0 || y >= size)
        return;

    // Check if the clicked position is already flagged
    if (revealed[x * size + y] == -1)
    {
        // Unflag the position
        revealed[x * size + y] = 0;
        (*flag_count)++;
    }
    else
    {
        // Flag the position
        revealed[x * size + y] = -1;
        (*flag_count)--;
    }

    // You may want to add UI updates here for flagging/unflagging
}

// Function to print the minefield with revealed cells
void printReveal(int size, int *revealed, int *field, int flag_mode)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (!revealed[i * size + j])
            {
                printf("\033[32m# \033[0m"); // Hidden cell
            }
            else if (revealed[i * size + j] == -1)
            {
                printf("\033[31mF \033[0m"); // Flagged cell
            }
            else
            {
                printf("%d ", field[i * size + j]); // Number of adjacent mines
            }
        }

        // Print Flag Mode indicator
        if (i == size / 2)
        {
            printf("\t%d \033[33m:flagmode\033[0m", flag_mode);
        }

        printf("\n");
    }
}

// Function to check if a cell contains a mine
int yesnoMine(int x, int y, int *field, int size)
{
    if (field[x * size + y] == -1)
        return 1;
    return 0;
}

// Function to count the number of revealed cells
int numRevealed(int size, int *revealed)
{
    int count = 0;
    for (int i = 0; i < size * size; i++)
    {
        if (revealed[i] == 1)  
            count++;
    }
   // printf("%d\n", count); // Debugging output
    delay(1);
    return count;
}

// Function to cheat and reveal the entire minefield
void cheat(int size, int *field)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            printf("%d ", field[i * size + j]); // Print the content of each cell
        }
        printf("\n");
    }
    delay(2);
}

// Function to play Minesweeper
int playMinesweeper(int size)
{
    printf("\e[1;1H\e[2J");

    printf("\033[34m");
    //UI for Game introduction
        char title[100]="Welcome to Minesweeper";
        for(int i=0;i<strlen(title);i++)
        {
            printf("%c",title[i]);
            delay(0.2);
        }
    printf("\033[0m");
    
    int *field;
    int *revealed;
    int flag_count = size * size / 6; // Initial flag count equals mine count
    int click_x, click_y;
    int flag_mode = 0; // Flag mode indicator

    // Allocate memory for the revealed array
    revealed = (int *)malloc(size * size * sizeof(int));

    // Initialize revealed array to all zeros (initially non revealed)
    for (int i = 0; i < size * size; i++)
    {
        revealed[i] = 0;
    }

    // Generate the minefield
    genField(size, &field);

    // Cheat to reveal the entire minefield (for debugging purposes)
    // cheat(size, field);
    // Main game loop
    int c=0;
    while (1)
    {
        // Take user input
        MouseControl(&click_x, &click_y);

        // Change mode to "Flag" by pressing "F" input
        if (click_x == 2 * size + 4 && click_y == size / 2)
        {
            flag_mode = !flag_mode;
        }

        // Check victory condition
        if (numRevealed(size, revealed) == (size * size) - size * size / 6)
        {
            free(field);
            free(revealed);
            printf("\033[36m\nCongratulations! You have won!\n\n\033[0m");
            return 1;
        }

        // Check lose condition
        if (flag_mode == 0 && yesnoMine(click_y, click_x / 2, field, size))
        {
            free(field);
            free(revealed);
            printf("\n\033[31mGame over! You stepped on a mine.\n\033[0m\n\n");
            return 0;
        }

        // Reveal or flag clicked areas based on flag mode
        if (flag_mode == 1)
        {
            flag(size, click_y, click_x / 2, field, revealed, &flag_count);
        }
        else
        {
            reveal(size, click_y, click_x / 2, field, revealed);
        }

        

        // Clear the screen and print the updated minefield
        printf("\e[1;1H\e[2J");

        printReveal(size, revealed, field, flag_mode);
        printf("\nNumber of Flags Left: %d\n", flag_count);
        printf("\033[34m\n\nMINESWEEPER\n\033[0m");
        printf("Status: ");

    }
}

int main()
{
    int resMine = playMinesweeper(6);
    return 0;
}
