#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

HANDLE console;
COORD CursorPosition;

// Function to delay output on terminal
void delay(float number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

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
                    // printf("Mouse clicked at (%d, %d)\n", mouseEvent.dwMousePosition.X, mouseEvent.dwMousePosition.Y);
                    return;
                }
            }
        }
    }
}

void printMessage(int x, int y, char message)
{
    // Get the handle to the standard output
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Set the cursor position to (x, y) and print the message
    COORD pos = {x, y};
    SetConsoleCursorPosition(hConsole, pos);
    printf("%c", message);
}

void printFalcon(int x, int y)
{
    // Get the handle to the standard output
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Set the cursor position to (x, y) and print the message
    COORD pos = {x, y};
    SetConsoleCursorPosition(hConsole, pos);
    printf("\x1b[34mV\x1b[0m");
}

void deleteFalcon(int x, int y)
{
    // Get the handle to the standard output
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Set the cursor position to (x, y) and print the message
    COORD pos = {x, y};
    SetConsoleCursorPosition(hConsole, pos);
    printf(" ");
}

void flyFalcon(int *falcon_x, int *falcon_y, int *prev_flight)
{
    (*falcon_x)++;
    if (*prev_flight == 0)
        (*falcon_y)--;
    if (*prev_flight == 1)
        (*falcon_y)++;
    if (*prev_flight == 2)
        (*falcon_y);

    int flight;
    if (*prev_flight == 0)
    {
        // 50% Down(0), 30% Straight, 20% Up
        int val = rand() % 100;
        if (val < 50)
            flight = 0;
        else if (val < 80 && val >= 50)
            flight = 2;
        else
            flight = 1;
    }
    else if (*prev_flight == 1)
    {
        // 50% Up(1), 30% Straight, 20% Down
        int val = rand() % 100;
        if (val < 50)
            flight = 1;
        else if (val < 80 && val >= 50)
            flight = 2;
        else
            flight = 0;
    }
    else
    {
        // 20% Straight, 40% Up, 40% Down
        int val = rand() % 100;
        if (val < 40)
            flight = 2;
        else if (val >= 80)
            flight = 0;
        else
            flight = 1;
    }
    *prev_flight = flight;
    return;
}

void printScopeFalcon(int x, int y, char c)
{
    // Get the handle to the standard output
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Set the cursor position to (x, y) and print the message
    COORD pos = {x, y};
    SetConsoleCursorPosition(hConsole, pos);
    printf("%c", c);
}

void openScopeFalcon(int x, int y, int side)
{
    for (int i = 0; i <= side / 2; i++)
    {
        printScopeFalcon(x + i, y + (side / 4), '_');
        printScopeFalcon(x - i, y + (side / 4), '_');
        printScopeFalcon(x + i, y - (side / 4), '_');
        printScopeFalcon(x - i, y - (side / 4), '_');
        printScopeFalcon(x + (side / 2), y + (i / 2), '|');
        printScopeFalcon(x + (side / 2), y - (i / 2), '|');
        printScopeFalcon(x - (side / 2), y + (i / 2), '|');
        printScopeFalcon(x - (side / 2), y - (i / 2), '|');
    }
    printScopeFalcon(x - (side / 2), y - (side / 2) + 2, ' ');
    printScopeFalcon(x + (side / 2), y - (side / 2) + 2, ' ');
}

void deleteScopeFalcon(int x, int y, int side)
{
    for (int i = 0; i <= side / 2; i++)
    {
        printScopeFalcon(x + i, y + (side / 4), ' ');
        printScopeFalcon(x - i, y + (side / 4), ' ');
        printScopeFalcon(x + i, y - (side / 4), ' ');
        printScopeFalcon(x - i, y - (side / 4), ' ');
        printScopeFalcon(x + (side / 2), y + (i / 2), ' ');
        printScopeFalcon(x + (side / 2), y - (i / 2), ' ');
        printScopeFalcon(x - (side / 2), y + (i / 2), ' ');
        printScopeFalcon(x - (side / 2), y - (i / 2), ' ');
    }
}

int playFalcon()
{
    printf("\e[1;1H\e[2J");
    printf("\n        \x1b[33m");
    char welcome[100]="WELCOME TO THE FALCONRY GAME";
    for(int i=0;i<strlen(welcome);i++)
    {
        putchar(welcome[i]);
        fflush(stdout);
        delay(0.1);
    }
    printf("\x1b[0m");
    printf("\n\n\n");
    system("clear");
    int click_x = -1, click_y = -1, arrows = 4, shot = 0;
    int spawn_y;
    while (spawn_y < 10 || spawn_y > 30)
        spawn_y = rand() % 31;

    int falcon_x = 0, falcon_y = spawn_y;

    int flight = rand() % 2; // Up=1. Down=0.
    printf("\e[1;1H\e[2J");
    printf("\x1b[33mEnter the space bar to open the scope. Press inside the scope to shoot\x1b[0m");
    printFalcon(falcon_x, falcon_y);
    delay(0.3);
    deleteFalcon(falcon_x, falcon_y);
    while (1)
    {
        flyFalcon(&falcon_x, &falcon_y, &flight);
        if (shot == 1)
        {
            if (click_x == falcon_x && click_y == falcon_y)
            {
                printMessage(click_x, click_y, '0');
                printf("\x1b[32mYou Win!!!!!\x1b[0m");
                return 1;
            }
            else
                printMessage(click_x, click_y, 'O');
        }
        if (falcon_x > 150)
        {    return 0;
             printf("\x1b[31mYou Lose the game\x1b[0m");
        }
        if (arrows == 0)
        {    return 0;
             printf("\x1b[31mYou Lose the game\x1b[0m");
        }
        printFalcon(falcon_x, falcon_y);
        shot = 0;
        click_x = 0, click_y = 0;
        if (_kbhit())
        {
            char key = _getch();
            if (key == ' ')
            {
                shot = 1;
                arrows--;
                openScopeFalcon(falcon_x, falcon_y, 8);
                MouseControl(&click_x, &click_y);
                printMessage(click_x, click_y, 'X');
                int temp_x = falcon_x, temp_y = falcon_y;
                deleteFalcon(falcon_x, falcon_y);
                flyFalcon(&falcon_x, &falcon_y, &flight);
                printFalcon(falcon_x, falcon_y);
                delay(0.3);
                printMessage(click_x, click_y, 'x');
                deleteFalcon(falcon_x, falcon_y);
                deleteScopeFalcon(temp_x, temp_y, 8);
            }
        }
        delay(0.3);
        deleteFalcon(falcon_x, falcon_y);
    }
}

int main()
{
    int falconRes = playFalcon();
    return falconRes;
}
