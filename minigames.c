#include "ICS_Project.h"

// Minigames

// Hangtheman (1)................................................................................................

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
int z = 0; // To prevent printing unecessary Mouse click in the beginning
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
                    if (z == 0)
                    {
                        z = 1;
                        SetConsoleMode(hStdin, fdwMode & (8));
                        return;
                    }
                    SetConsoleMode(hStdin, fdwMode & (8));
                    // printf("Mouse clicked at (%d, %d)\n", mouseEvent.dwMousePosition.X, mouseEvent.dwMousePosition.Y);
                    return;
                }
            }
        }
    }
}

int lives = 8, choice, l = 0; // l to remove slight bugs during displaying
int num_of_correct_guesses;

void intro();                                                                                               // Intro pages
int select_choice();                                                                                        // To select the category
int return_current_status(char guessed_string[], char *current_status[], char selected_word[], int length); // Updating the current status of the string
void print_current_status(char *current_status[], int length);                                              // Printing the current status of the string
void display_hangman(int result);                                                                           // Displaying the updated Hangman podium
void sigint_handler(int sig);

int start_hangman()
{
    while (1)
    {

        srand(time(0));         // Generating the seed to randomise the text to be predicted
        printf("\e[1;1H\e[2J"); // To clear the terminal on Unix/Linux/Mac //Use: system("cls"); for Windows
        intro();
        char data[][500] = {
            "Rome wasn't built in a day",
            "All roads lead to Rome",
            "When in Rome, do as the Romans do",
            "Et tu, Brute?",
            "To the victor belong the spoils",
            "What we do in life echoes in eternity",
            "Veni, vidi, vici",
            "Senatus Populusque Romanus",
            "I came, I saw, I conquered"
            "Let him who desires peace prepare for war"
            // "You couldn't live with your own failure, and where does that get you? Back to me",
            // "The hardest choices require the strongest wills",
            // "I am deadpool",
            // "What is hate, if not love persevering",
            // "I am iron man",
            // "Whatever it takes",
            // "Just because something works, doesn't mean it can't be improved", // Marvel Quotes
            // "One small step for man, one giant leap for mankind",
            // "Be the change that you wish to see in the world",
            // "Live as if you were to die tomorrow. Learn as if you were to live forever",
            // "The future depends on what you do today",
            // "United we stand, divided we fall",
            // "I'll be back",
            // "Dream, dream, dream. Dreams transform into thoughts and thoughts result in action", // Famous quotes
            // "Avengers",
            // "Joker",
            // "Interstellar",
            // "Avatar",
            // "The dark knight", // Famous movies
            // "Introduction to computer science",
            // "Introduction to electrical engineering",
            // "Introduction to bioengineering",
            // "Engineering realization",
            // "Mathematics" // Graded sem 2 Courses
        };
        int n = 23;
        int selected_index;
        printf("\n\n");
        display_hangman(lives);
        for (int i = 0; i < 1000; i++) // To add a delay
            rand();

        choice = select_choice(); // Selecting the category
        if (choice == 1)
            selected_index = rand() % 11;
        // else if (choice == 2)
        //     selected_index = rand() % 7 + 7;
        // else if (choice == 3)
        //     selected_index = rand() % 5 + 14;
        // else if (choice == 4)
        //     selected_index = rand() % 5 + 19;
        // else
        // {
        //     printf("Wrong Choice\n");
        //     return 0;
        // }

        char selected_word[500];
        int length = strlen(data[selected_index]); // length of the selected word
        strcpy(selected_word, data[selected_index]);

        int num = 1, x = 0;
        char player1[500], player2[500];

        printf("\033[0;32m\n"); // Green colour
        // printf("Enter the number of players: ");
        // scanf("%d", &num);
        printf("\033[0m\n"); // Default colour
        int life[num];       // To store the lives of all the players

        while (x < num) // x = current player index
        {
            fgetc(stdin); // To Enter on the keyboard

            printf("\e[1;1H\e[2J");

            lives = 8;
            char guessed_string[500];

            char *current_status[length];

            for (int i = 0; i < length; i++) // Allocating memory to the current_status pointer array using DMA
                current_status[i] = (char *)malloc(sizeof(char));
            int spaces = 0;

            for (int i = 0; i < length; i++)
            {
                if (selected_word[i] == ' ')
                {
                    *current_status[i] = ' ';
                    spaces++; // To find the number of spaces
                }
                else // Inserting zero to the locations where a letter exists
                    *current_status[i] = '0';
            }

            int exit = 0; // To check if the whole string is correct

            num_of_correct_guesses = 0;

            printf("\033[0;36m\n"); // Cyan color
            printf("\n                       DANIE'S HANGMAN GAME   \n");
            printf("\033[0m\n"); // Default color

            while (num_of_correct_guesses < (length - spaces) && lives > 0)
            {
                if (choice == 1)
                    printf("\n\n             Marvel Quotes\n");
                else if (choice == 2)
                    printf("\n\n             Famous Quotes\n");
                else if (choice == 3)
                    printf("\n\n             Famous Movies\n");
                else if (choice == 4)
                    printf("\n\n             Graded Sem2 Courses\n");

                printf("\033[0;33m"); // Yellow color
                printf("\n\nPlayer%d\n", (x + 1));
                printf("\033[0m\n");
                display_hangman(lives);

                printf("\n**************************\n\n");
                print_current_status(current_status, length);

                printf("\n\nEnter a letter or directly predict the sentence!!\n\n!!!DO NOT REPEAT THE LETTERS ALREADY TYPED!!!\n\n\nNumber of lives left: %d\n", lives);

                printf("\033[0;32m\n");
                printf("\n\nYour Prediction::  ");
                fgets(guessed_string, length + 1, stdin); // Accepting the letter or the entire string
                printf("\033[0m\n");

                if (l == 0)
                {
                    printf("\033[0;36m\n");
                    printf("\n                       DANIE'S HANGMAN GAME   \n");
                    printf("\033[0m\n");
                }

                if (guessed_string[1] == '\n') // While using fgets, it automatically insert a \n to the end. To remove that
                    guessed_string[1] = 0;     // Zero can be inserted to a string to represent NULL/ otherwise a letter would be considered as a string

                if (strcmp("quit\n", guessed_string) == 0)
                {
                    printf("\e[1;1H\e[2J");
                    printf("\033[0;31m\n");
                    printf("You Quit!!!\n");
                    printf("\033[0m\n");
                    printf("The correct answer was: %s\n", selected_word);
                    display_hangman(0);
                    lives = 0;
                    break;
                }

                int status = return_current_status(guessed_string, current_status, selected_word, length);

                if (status == 1) // letter is present
                {
                    printf("\e[1;1H\e[2J");

                    printf("\033[0;32m\n"); // Green color
                    printf("Correct\n");
                    if (num_of_correct_guesses == length - spaces) // To check if the control has reached the end of the iteration
                    {

                        printf("\033[0m\n");
                        display_hangman(-1);
                        printf("\n**************************\n");
                        printf("\033[0;32m\n"); // green
                        printf("\nCongrats!! You have predicted successfully!!\n\n");
                        return_current_status(guessed_string, current_status, selected_word, length);
                        print_current_status(current_status, length);
                        printf("\033[0m\n"); // default
                        printf("\n********************\n");
                        break;
                    }
                    printf("\033[0m\n");
                }
                else if (status == 0) // letter guessed is absent in the string
                {
                    printf("\e[1;1H\e[2J");
                    printf("\033[0;31m\n");
                    printf("WRONG GUESS\nYou lose a life!!");
                    printf("\033[0m\n");
                    lives--;
                }

                else if (status == 2) // Correctly guessed the String
                {
                    printf("\e[1;1H\e[2J");
                    display_hangman(-1);
                    printf("\n**************************\n");
                    printf("\033[0;32m");
                    printf("\nCongrats!! You have predicted successfully!!\n\n");
                    return_current_status(guessed_string, current_status, selected_word, length);
                    print_current_status(current_status, length);
                    printf("\033[0m");
                    printf("\n********************\n");
                    fgetc(stdin);
                    break;
                }
                else if (status == 3) // Guessed string is wrong
                {
                    printf("\033[0;31m\n");
                    printf("You lose a life!!\n");
                    printf("\033[0m\n");
                }

                if (lives == 0)
                {
                    printf("\033[0;31m\n");
                    printf("You lose because you ran out of lives\n");
                    printf("\033[0m\n");
                    printf("The correct answer was: %s\n", selected_word);
                    display_hangman(lives); // DEAD!!
                }

                printf("\n********************\n");

                l = 0;
                if (status == 3) // If a wrong string was typed when we predicted the entire string, we get some extra characters in the input buffer
                {
                    printf("\033[0;31m\n");

                    if (lives == 0)
                        printf("You predicted the Wrong String!!!\n\n");
                    else
                        printf("You predicted the Wrong String!!! Click Enter to continue");
                    l = 1;
                    while ((getchar()) != '\n')
                        ; // In order to get rid of those extra characters
                    printf("\033[0m\n");
                    printf("\e[1;1H\e[2J");
                }
            }

            for (int i = 0; i < length; i++) // Freeing the memory which was allocated to the current_status
                free(current_status[i]);

            printf("\n\nNumber of lives left: %d\n", lives);
            life[x] = lives;
            x++; // Next player

            if (num > 1 && num != x)
            {
                printf("\033[0;36m");
                printf("\n\nClick Enter for the next player:");
                printf("\033[0m");
            }
        }
        int dupe[num];
        for (int i = 0; i < num; i++) // Duplicating the life array
            dupe[i] = life[i];
        printf("\033[0;36m");
        printf("\n\nClick Enter to display the Leaderboard!!!");
        fgetc(stdin);
        printf("\033[0m\n\n");
        printf("\e[1;1H\e[2J");

        printf("\033[0;36m\n");
        printf("       DANIE'S HANGMAN GAME\n\n");
        printf("\033[0m\n");

        printf("\n\n********************************************\n");
        printf("\033[0;33m\n");
        printf(" ________________________________________");
        printf(" \n|           !! LEADERBOARD !!            |\n");
        printf("|                                        |\n");
        printf("|                                        |\n");

        int winner = life[0], y, temp = 0, z = 0;
        for (int i = 0; i < num; i++) // Sorting the life array from highest to lowest life
        {
            y = i;
            winner = life[i];
            for (int j = i + 1; j < num; j++)
            {
                if (life[j] > winner)
                {
                    winner = life[j];
                    y = j;
                }
            }
            temp = life[i];
            life[i] = life[y];
            life[y] = temp;
        }
        int pnum = 0, w = 0;
        for (int i = 0; i < num; i++)
        {
            if (life[i] == life[i + 1] && i != num - 1) // To display the correct result when two players of same life exist
                continue;
            for (int j = 0; j < num; j++)
            {
                if (life[i] == dupe[j])
                {
                    // The different conditions here are just to correctly align the data in the leaderboard while printing
                    if (pnum + 1 > 9)
                        printf("| %d.  Player%d : Lives left: %d          |\n", ++pnum, j + 1, life[i]);
                    else if (j + 1 > 9)
                        printf("| %d.   Player%d : Lives left: %d          |\n", ++pnum, j + 1, life[i]);
                    else
                        printf("| %d.   Player%d  : Lives left: %d          |\n", ++pnum, j + 1, life[i]);
                }
            }
        }

        printf("|                                        |\n");
        printf("|________________________________________|\n\n");
        printf("\033[0m\n");

        printf("\n********************************************\n\n\n\n\n\n\n");

        return 0;
    }
}
int return_current_status(char guessed_string[], char *current_status[], char selected_word[], int length)
{
    int status = 0;
    if (guessed_string[1] == 0) // If a single letter was input by the user
    {
        for (int i = 0; i < length; i++) // To check whether the letter is present in the string or not
        {
            if (selected_word[i] == guessed_string[0] || tolower(selected_word[i]) == guessed_string[0])
            {
                *current_status[i] = selected_word[i];
                status = 1;
                num_of_correct_guesses++;
            }
        }
    }

    else // If the user directly wants to predict the string
    {
        if (strcmp(guessed_string, selected_word) == 0)
        {
            for (int i = 0; i < length; i++)
                *current_status[i] = guessed_string[i];
            return 2;
        }
        else
        {
            lives--;
            return 3;
        }
    }

    return status;
}
void print_current_status(char *current_status[], int length)
{
    for (int i = 0; i < length; i++)
    {
        if (*current_status[i] == ' ')
            printf("\n");
        else if (*current_status[i] == '0')
            printf("_ ");
        else
            printf("%c", *current_status[i]);
    }
    printf("\n");
}
int select_choice()
{

    printf("\n\n\n\nWelcome to Hang the Man Game\n");
    // printf("Choose a Category to play the Game.\n1. Marvel Quotes\n2. Famous Quotes\n3. Famous Movies\n4. Graded Sem 2 Courses\n\n\n");
    printf("\033[0;32m\n");
    // printf("Choice: ");
    int choice = 1;
    // scanf("%d", &choice);
    printf("\033[0m\n");
    printf("\n#*#*#*#*#*#*#*#*#*#*#*#*\n");
    fgetc(stdin); // To remove extra \n from the input stream which may mess with the code!!
    return choice;
}
void display_hangman(int result) // Displaying the hanged man based on the number of lives left
{
    if (result == 8)
        printf("   ___________________\n   |/\n   |\n   |\n   |\n   |\n   |\n   |\n   |________________________");
    if (result == 7)
        printf("   ___________________\n   |/         |\n   |\n   |\n   |\n   |\n   |\n   |\n   |________________________");
    if (result == 6)
        printf("   ___________________\n   |/         |\n   |          |\n   |\n   |\n   |\n   |\n   |\n   |________________________");
    if (result == 5)
        printf("   ___________________\n   |/         |\n   |          |\n   |          O\n   |\n   |\n   |\n   |\n   |________________________");
    if (result == 4)
        printf("   ___________________\n   |/         |\n   |          |\n   |          O\n   |          |\n   |          |\n   |\n   |\n   |________________________");
    if (result == 3)
        printf("   ___________________\n   |/         |\n   |          |\n   |          O\n   |         /|\n   |          |\n   |\n   |\n   |________________________");
    if (result == 2)
        printf("   ___________________\n   |/         |\n   |          |\n   |          O\n   |         /|\\\n   |          |\n   |\n   |\n   |________________________");
    if (result == 1)
        printf("   ___________________\n   |/         |\n   |          |\n   |          O\n   |         /|\\\n   |          |\n   |         / \n   |\n   |________________________");
    if (result == 0)
        printf("   ___________________\n   |/         |\n   |          |\n   |          O\n   |         /|\\               YOU DIED!!!\n   |          |\n   |         / \\        \n   |\n   |________________________");
    if (result == -1)
        printf("   ___________________\n   |/          \n   |\033[0;32m       YOU WIN!!\033[0m  \n   |           \n   |             \n   |         \\O/\n   |          |\n   |          |\n   |_________/_\\______________");
}

void intro()
{
    // printf("\033[0;34m\n");//blue

    printf("\n        \x1b[33m");
    char welcome[100] = "HANGMAN GAME";
    for (int i = 0; i < strlen(welcome); i++)
    {
        putchar(welcome[i]);
        fflush(stdout);
        // delay(0.2);
    }
    printf("\x1b[0m");
    printf("\n");
    printf("\033[1;31m\033[0m\n"); // red
    printf("\033[0m\n");           // default

    printf("\n\n      __________________________________________________________\n");
    printf("      |                     HANGMAN GAME                       |   \n");
    printf("      |                                                        |   \n");
    printf("      |          ____________               ____________       |   \n");
    printf("      |          |      |                   |                  |   \n");
    printf("      |          |      O                   |                  |   \n");
    printf("      |          |     /|\\                  |                  |   \n");
    printf("      |          |     / \\                  |    \\ O /         |   \n");
    printf("      |          |                          |      |           |   \n");
    printf("      |          |___________               |_____/_\\_____     |   \n");
    printf("      |                                                        |   \n");
    printf("      |         Welcome to Danie's Hangman Game!!              |   \n");
    printf("      |     Guess the string and save the stickman's life!     |   \n");
    printf("      |________________________________________________________|   \n");
    printf("\033[0;32m\n"); // Green
    printf("\n\n\nClick Enter to continue: ");
    fgetc(stdin);
    printf("\033[0m\n");
    printf("\e[1;1H\e[2J");
    printf("\033[0;36m\n");
    printf("\n                       DANIE'S HANGMAN GAME   \n");
    printf("\033[0m\n");

    printf("\033[0m\n");
    printf("\n\n       _____________________________________________________________\n");
    printf("      ||                                                          ||\n");
    printf("      ||                RULES AND REGULATIONS                     ||\n");
    printf("      ||                                                          ||\n");
    printf("      ||                                                          ||\n");
    printf("      ||    1. The game has both single and multiplayer modes.    ||\n");
    printf("      ||    2. A category can be chosen from a given set.         ||\n");
    printf("      ||    3. Any number of players can compete                  ||\n");
    printf("      ||       against each other.                                ||\n");
    printf("      ||    4. In a game, all the players will be given the       ||\n");
    printf("      ||       same word/quote randomly and the winner is decided ||\n");
    printf("      ||       on the basis of which player has accurately        ||\n");
    printf("      ||       guessed the answer consuming the least number      ||\n");
    printf("      ||       of lives.                                          ||\n");
    printf("      ||    5.Each player will have 8 lives.                      ||\n");
    printf("      ||    6.In every round, a player can either predict         ||\n");
    printf("      ||      a letter in the word/quote or the entire string     ||\n");
    printf("      ||      itself if confident.                                ||\n");
    printf("      ||    7. In either case, If their prediction is wrong,      ||\n");
    printf("      ||       they lose a life.                                  ||\n");
    printf("      ||    8. At any point in the game, if the player feels like ||\n");
    printf("      ||       giving up, they can do so by typing \"quit\".        ||\n");
    printf("      ||                                                          ||\n");
    printf("      ||                                                          ||\n");
    printf("      ||  ");
    printf("\033[0;31m"); // red
    printf("PRECAUTION!!: Do not re-enter your predicted letter!!");
    printf("\033[0m");
    printf("   ||\n");
    printf("      ||                                                          ||\n");
    printf("      ||__________________________________________________________||\n");

    printf("\033[0;32m\n");
    printf("\n\n\nClick Enter if you have properly read the Rules: ");
    fgetc(stdin);
    printf("\033[0m\n");
    printf("\e[1;1H\e[2J");
    printf("\033[0;36m\n");
    printf("\n                        HANGMAN GAME   \n");
    printf("\033[0m\n");

    return;
}

int Hangtheman()
{
    int x = start_hangman();
}

// Snakegame (2)...................................................................................................
int Snakegame()
{
    printf("Sorry, this minigame is available for MacOS only\n");
    return 1;
}

// Minesweeper (3) .................................................................................................
// Function to delay output on terminal

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
        if ((*field)[x * size + y] != -1)
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
    // UI for Game introduction
    char title[100] = "Welcome to Minesweeper";
    for (int i = 0; i < strlen(title); i++)
    {
        printf("%c", title[i]);
        // delay(0.2);
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
    int c = 0;
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
            HANDLE hStdin;
            DWORD fdwMode;
            DWORD cNumRead;
            SetConsoleMode(hStdin, fdwMode & ~(ENABLE_MOUSE_INPUT));

            return 1;
        }

        // Check lose condition
        if (flag_mode == 0 && yesnoMine(click_y, click_x / 2, field, size))
        {
            free(field);
            free(revealed);
            printf("\n\033[31mGame over! You stepped on a mine.\n\033[0m\n\n");
            HANDLE hStdin;
            DWORD fdwMode;
            DWORD cNumRead;
            SetConsoleMode(hStdin, fdwMode & ~(ENABLE_MOUSE_INPUT));
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

int Minesweeper()
{
    printf("This game may cause problems in future control, proceed at your own risk\nYou may choose to skip this game (complete quest automatically)\n");
    printf("Enter (0) to play:\n");
    printf("Enter (1) to skip minigame\n");
    int choice;
    scanf(" %d", &choice);
    if (choice == 1)
        return 1;
    int resMine = playMinesweeper(6);
    // Minesweeper Rules

    printf("\n\n       _____________________________________________________________\n");
    printf("      ||                                                          ||\n");
    printf("      ||                RULES AND REGULATIONS                     ||\n");
    printf("      ||                                                          ||\n");
    printf("      ||    1. The game will automatically generate a minefield of||\n");
    printf("      ||       a specified size (6x6). The minefield              ||\n");
    printf("      ||       contains hidden cells, some of which may contain   ||\n");
    printf("      ||       mines.                                             ||\n");
    printf("      ||    2. Objective: The objective of Minesweeper is to      ||\n");
    printf("      ||       uncover all the cells on the grid that do not      ||\n");
    printf("      ||       contain mines without detonating any of the mines. ||\n");
    printf("      ||       Controls:                                          ||\n");
    printf("      ||       Mouse Click: Click on a cell to reveal it.         ||\n");
    printf("      ||       If the revealed cell contains a mine, the game ends||\n");
    printf("      ||    3. Flagging Mode (Optional):                          ||\n");
    printf("      ||       Press \"F\"to toggle flagging mode.In flagging mode  ||\n");
    printf("      ||       you can flag cells that you suspect contain mines  ||\n");
    printf("      ||       to avoid accidentally clicking on them. Flagging a ||\n");
    printf("      ||       cell prevents it from being clicked.               ||\n");
    printf("      ||    4. Mouse Movement: Move the mouse cursor to position  ||\n");
    printf("      ||       the click.                                         ||\n");
    printf("      ||    5. Number of Flags Left: The number of remaining flags||\n");
    printf("      ||       is displayed on the screen.                        ||\n");
    printf("      ||    6. Gameplay:                                          ||\n");
    printf("      ||       Revealing Cells: Left-click on a cell to reveal it.||\n");
    printf("      ||       If the cell is empty, it will show the number of   ||\n");
    printf("      ||       adjacent mines. If it contains a mine,the game ends||\n");
    printf("      ||    7. In flagging mode, left-click on a cell to flag it  ||\n");
    printf("      ||       as containing a mine.                              ||\n");
    printf("      ||       Win Condition: You win the game when all non-mine  ||\n");
    printf("      ||       cells are revealed.                                ||\n");
    printf("      ||       Lose Condition: You lose the game if you reveal a  ||\n");
    printf("      ||       cell containing a mine.                            ||\n");
    printf("      ||                                                          ||\n");
    printf("      ||||\n");
    return resMine;
}

// Memorygame (4)..............................................................................................

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
                printf("\x1b[33m# \x1b[0m");
        }
        printf("\n");
    }
    printf("\x1b[34m\n\n\nMEMORY GAME!!!! \n\x1b[0m\n");
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
                printf("\x1b[33m# \x1b[0m");
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
    printf("\e[1;1H\e[2J");
    printf("\n        \x1b[33m");
    char welcome[100] = "WELCOME TO THE MEMORY GAME";
    for (int i = 0; i < strlen(welcome); i++)
    {
        printf("%c", welcome[i]);
        // delay(0.2);
    }
    printf("\x1b[0m");
    printf("\n\n\n");
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
        {
            return 0;
            printf("You Lose!\n");
        }
        if (progress == num)
        {
            return 1;
            printf("You Win!!\n");
        }
    }
}
int start_memory()
{
    int size = 4, attempts = 15;
    int resMemory = playMemory(size, size * size / 2, attempts);
}

int Memory()
{
    printf("This game may cause problems in future control, proceed at your own risk\nYou may choose to skip this game (complete quest automatically)\n");
    printf("Enter (0) to play:\n");
    printf("Enter (1) to skip minigame\n");
    int choice;
    scanf(" %d", &choice);
    if (choice == 1)
        return 1;
    int x = start_memory();
    return x;
}

// PrisonEscape (5) .......................................................................................................

void print_prison(int size, int i, int j, int k);
// Known Error: X is Y, Y is X. Needs extensive UI changes
void printPrisonEscape(int size, int prison[2 * size][2 * size], cell_pos exit, cell_pos pl_pos, cell_pos guard_pos)
{
    printf("\n        \x1b[33m");
    printf("PRISON ESCAPE GAME");
    printf("\x1b[0m\n\n\n\n");
    for (int i = 0; i < 2 * size; i++)
    {
        for (int j = 0; j < 2 * size; j++)
        {
            if (pl_pos.x == i && pl_pos.y == j)
            {
                printf("\x1b[33m"); // Yellow
                printf("P ");
                printf("\x1b[0m"); // Reset
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
            else if ((i == 0) || (j == 0) || (i == 2 * size - 1) || (j == 2 * size - 1) || (i % 2 != 0) || (j % 2 != 0))
                print_prison(size, i, j, prison[i][j]);

            else
            {
                printf("%d ", prison[i][j]); // Zero are the positions where the player can move
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
        printPrisonEscape(size, prison, exit, pl_pos, guard_pos);
        printf("\033[31mGuard caught you!\n\033[31m\n\n\n\n\n");
        sleep(3);
        return;
    }

    if (pl_pos.x == exit.x && pl_pos.y == exit.y)
    {
        printPrisonEscape(size, prison, exit, pl_pos, guard_pos);
        printf("\033[32mCongratulations! You escaped!\n\n\n\n\n\033[0m");
        sleep(2);
        return;
    }
}
void movePrison(int size, int prison[2 * size][2 * size], cell_pos *pl_pos, char pl_move, cell_pos exit)
{
    switch (pl_move)
    {
    case 'D':
        if (pl_pos->y + 2 < 2 * size && prison[pl_pos->x][(pl_pos->y) + 1] == 0)
        {
            pl_pos->y += 2;
        }
        break;

    case 'W':
        if (pl_pos->x - 2 >= 0 && prison[(pl_pos->x) - 1][(pl_pos->y)] == 0)
        {
            pl_pos->x += -2;
        }
        break;

    case 'A':
        if (pl_pos->y - 2 >= 0 && prison[(pl_pos->x)][(pl_pos->y) - 1] == 0)
        {
            pl_pos->y += -2;
        }
        break;

    case 'S':
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
    pl_move = toupper(pl_move);
    printf("\e[1;1H\e[2J");
    movePrison(size, prison, pl_pos, pl_move, exit);
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
            guard_move = 'S';
            movePrison(size, prison, guard_pos, guard_move, exit);
            result(size, prison, exit, *pl_pos, *guard_pos);
            count++;
        }

        else if (pl_pos->x < guard_pos->x && prison[(guard_pos->x) - 1][guard_pos->y] == 0)
        {
            guard_move = 'W';
            movePrison(size, prison, guard_pos, guard_move, exit);
            result(size, prison, exit, *pl_pos, *guard_pos);
            count++;
        }

        else if ((pl_pos->x == guard_pos->x && pl_pos->y > guard_pos->y) && prison[(guard_pos->x)][(guard_pos->y) + 1] == 0)
        {
            guard_move = 'D';
            movePrison(size, prison, guard_pos, guard_move, exit);
            result(size, prison, exit, *pl_pos, *guard_pos);
            count++;
        }

        else if ((pl_pos->x == guard_pos->x && pl_pos->y < guard_pos->y) && prison[(guard_pos->x)][(guard_pos->y) - 1] == 0)
        {
            guard_move = 'A';
            movePrison(size, prison, guard_pos, guard_move, exit);
            result(size, prison, exit, *pl_pos, *guard_pos);
            count++;
        }
    }
    printPrisonEscape(size, prison, exit, *pl_pos, *guard_pos);
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
        // prison[11][2] = 1;
        prison[11][4] = 1;
        prison[11][6] = 1;
        // prison[11][12] = 1;
        prison[13][0] = 1;
        prison[13][6] = 1;
        prison[13][8] = 1;
        prison[13][12] = 1;
        prison[14][3] = 1;
        prison[14][9] = 1;
    }
}

int playPrisonEscape(int size, int map)
{
    printf("\e[1;1H\e[2J");

    int prison[size * 2][size * 2];
    makePrison(size, prison);
    cell_pos exit;
    cell_pos pl_pos;
    cell_pos guard_pos;
    if (map == 0)
    {
        exit.x = 8;
        exit.y = 14;

        pl_pos.x = 14;
        pl_pos.y = 2;

        guard_pos.x = 4;
        guard_pos.y = 8;
    }
    else if (map == 1)
    {
        exit.x = 14;
        exit.y = 8;

        pl_pos.x = 10;
        pl_pos.y = 10;

        guard_pos.x = 6;
        guard_pos.y = 4;
    }
    else if (map == 2)
    {
        exit.x = 2;
        exit.y = 10;

        pl_pos.x = 14;
        pl_pos.y = 2;

        guard_pos.x = 14;
        guard_pos.y = 14;
    }
    printf("\n        \x1b[33m");
    char welcome[100] = "WELCOME TO THE PRISON ESCAPE GAME";
    for (int i = 0; i < strlen(welcome); i++)
    {
        putchar(welcome[i]);
        fflush(stdout);
        // delay(1);
    }
    printf("\x1b[0m");
    printf("\n\n\n");
    printf("\e[1;1H\e[2J");
    printPrisonEscape(size, prison, exit, pl_pos, guard_pos);

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
            printf("\e[1;1H\e[2J");
            printPrisonEscape(size, prison, exit, pl_pos, guard_pos);
            return 0;
            break; // guard caught player
        }
    }
}

int start_prison_escape()
{
    srand(time(0));
    for (int i = 0; i < 1000; i++) // To properly randomise
        rand();
    int map = rand() % 3;
    int o = playPrisonEscape(8, map);
    return o;
}

int PrisonEscape()
{
        // Prison Escape game
        printf("\n\n       _____________________________________________________________\n");
        printf("      ||                                                          ||\n");
        printf("      ||                RULES AND REGULATIONS                     ||\n");
        printf("      ||                                                          ||\n");
        printf("      ||                                                          ||\n");
        printf("      ||    1. You are the prisoner and is trying to escape the   ||\n");
        printf("      ||       prison. But you are noticed by the Guard who       ||\n");
        printf("      ||       started chasing you.                               ||\n");
        printf("      ||    2. You need to reach the Exit without getting caught. ||\n");
        printf("      ||    3. Prisoner(P)(You), Guard(G) and Exit(E)             ||\n");
        printf("      ||       are marked on the map. & represents obstacles.     ||\n");
        printf("      ||    4. In a game, use W,A,S,D, to move the prisoner.      ||\n");
        printf("      ||       W - Top : A - Left : S - Down : D - Right          ||\n");
        printf("      ||    5.The Guard's algorithm is such that he move two steps||\n");
        printf("      ||      always in the direction of the prisoner             ||\n");
        printf("      ||      So you can trap the Guard and escape the prison.    ||\n");
        printf("      ||                                                          ||\n");
        printf("      ||||\n");
    return start_prison_escape();
}

void print_prison(int size, int i, int j, int k)
{

    if (i == 0)
    {
        if (j == 2 * size - 1)
            printf("_");
        else
            printf("__");
    }
    else if (j == 0)
    {
        if (i == 2 * size - 1)
            printf("|_");
        else
            printf("| ");
    }
    else if (j == 2 * size - 1)
        printf("|");
    else if (i == 2 * size - 1)
        printf("__");
    else if ((i % 2 != 0 || j % 2 != 0) && k == 1)
    {
        printf("\x1b[36m");
        printf("& "); // Obstacles
        printf("\x1b[0m");
    }
    else if ((i % 2 != 0 || j % 2 != 0) && k == 0)
        printf("  ");
}

// Lockpicking (6) ................................................................................................................

int dig(int n, int num, int i)
{
    return (int)(num / pow(10, n - i - 1)) % 10;
}
void wordlegame(int n, int num, int guess)
{
    for (int i = 0; i < n; i++)
    {
        int digG = dig(n, guess, i), count = 1;
        for (int j = 0; j < n; j++)
        {
            int digN = dig(n, num, j);
            if (digG == digN && i == j)
            {
                printf("\x1b[32m2\x1b[0m");
                count = 0;
                break;
            }
            else if (digG == digN && i != j)
            {
                printf("\x1b[33m1\x1b[0m");
                count = 0;
                break;
            }
        }
        if (count == 1)
            printf("\x1b[31m0\x1b[0m");
    }
    printf("\n");
}
int playWordle(int n, int attempts)
{
    // printf("\e[1;1H\e[2J");
    printf("\n        \x1b[33m");
    char welcome[100] = "WELCOME TO THE LOCKPICKING GAME";
    for (int i = 0; i < strlen(welcome); i++)
    {
        putchar(welcome[i]);
        fflush(stdout);
    }
    printf("\x1b[0m");
    printf("\n\n\n");
    int num;
    srand(time(0));
    for (int i = 0; i < 1000; i++)
        rand();
    while (num < (int)pow(10, n - 1) || num >= (int)pow(10, n))
    {
        num = rand() % (int)(pow(10, n));
    }
    int guess;
    printf(" __________________________________________                \x1b[33m     1 - Number is present but not at the accurate location\x1b[0m\n");
    printf("|                                                          \x1b[32m     2 - Number is present and is at the accurate location\x1b[0m\n");
    printf("|                                                          \x1b[31m     0 - Number is not present in the string \x1b[0m\n");
    while (attempts >= 0)
    {
        // printf("\033[K");
        printf("|       Guess the 5 digit number number: ");
        scanf("%d", &guess);
        printf("|\x1b[34m                       Encrypted Code:  \x1b[0m");
        if (guess == num)
        {
            for (int i = 0; i < n; i++)
            {
                printf("\x1b[32m2\x1b[0m");
            }
            printf("\n");

            return 1;
        }
        wordlegame(n, num, guess);
        attempts--;
        printf("|                                          \n");
        printf("|\x1b[36m       Number of attempts left: %d\n\x1b[0m", attempts);
        printf("|                                          \n");
    }
    printf("|       The number was %d\n", num);
    return 0;
}
int start_wordle()
{
    int n = 5, attempts = 5, resWordle;
    resWordle = playWordle(n, attempts);

    if (resWordle == 1)
    {
        printf("|\x1b[32m       Good Game!!!\n\x1b[0m");
        printf("|__________________________________________");
        sleep(2);
        return 1;
    }
    else if (resWordle == 0)
    {
        printf("|\x1b[31m       You Lose!!\n\x1b[0m");
        printf("|__________________________________________");
        return 0;
    }
    return 0;
}

int Lockpicking()
{
    // Num Wordle
    printf("\n\n       _____________________________________________________________\n");
    printf("      ||                                                          ||\n");
    printf("      ||                RULES AND REGULATIONS                     ||\n");
    printf("      ||                                                          ||\n");
    printf("      ||                                                          ||\n");
    printf("      ||    1. You need to predict the 5 digit number within 5    ||\n");
    printf("      ||       attempts by making use of the encrypted code       ||\n");
    printf("      ||    2. The Code 1: The number is present but location of  ||\n");
    printf("      ||       the number is wrong in the number predicted        ||\n");
    printf("      ||       The Code 2: The number is present and the location ||\n");
    printf("      ||       of the number is correct.                          ||\n");
    printf("      ||       The Code 0: The number is not present in the number||\n");
    printf("      ||                                                          ||\n");
    printf("      ||||\n");
    int u = start_wordle();
    if(u)
        printf("You Won!\n");
    else printf("You Lost\n");
    return u;
}

// Falconry (7) ................................................................................................................
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
    char welcome[100] = "WELCOME TO THE FALCONRY GAME";
    for (int i = 0; i < strlen(welcome); i++)
    {
        putchar(welcome[i]);
        fflush(stdout);
        delay(0.1);
    }
    printf("\x1b[0m");
    printf("\n\n\n");
    printf("\e[1;1H\e[2J");
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
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                COORD pos = {0, 0};
                SetConsoleCursorPosition(hConsole, pos);
                SetConsoleTextAttribute(hConsole, 7);
                return 1;
            }
            else
                printMessage(click_x, click_y, 'O');
        }
        if (falcon_x > 150)
        {
            return 0;
            printf("\x1b[31mYou Lose the game\x1b[0m");
            // Get the handle to the standard output
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            COORD pos = {0, 0};
            SetConsoleCursorPosition(hConsole, pos);
            SetConsoleTextAttribute(hConsole, 7);
        }
        if (arrows == 0)
        {
            return 0;
            printf("\x1b[31mYou Lose the game\x1b[0m");
            // Get the handle to the standard output
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            COORD pos = {0, 0};
            SetConsoleCursorPosition(hConsole, pos);
            SetConsoleTextAttribute(hConsole, 7);
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

int falconry()
{
    printf("This game may cause problems in future control, proceed at your own risk\nYou may choose to skip this game (complete quest automatically)\n");
    printf("Enter (0) to play:\n");
    printf("Enter (1) to skip minigame\n");
    int choice;
    scanf(" %d", &choice);
    if (choice == 1)
        return 1;
    int falconRes = playFalcon();

    return falconRes;
}

// Combat (8) ............................................................................................................

void print_combat(int plStamina, int oppStamina)
{
    printf("player stamina = %d opponent Stamina = %d\n", plStamina, oppStamina);
    printf("\n\n");
}
void gameCombat(int plWeapon, int *plStamina, int oppWeapon, int *oppStamina, int pl_att, int pl_def, int opp_att, int opp_def)
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
    scanf(" %c %c", &pl1, &pl2);

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

    gameCombat(plWeapon, plStamina, oppWeapon, oppStamina, pl_att, pl_def, opp_att, opp_def);

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

int Combat()
{
    printStory("Welcome to Combat Game\n\n",URED,MED);
    printStory("Enter Player Move: H(head)/B(body)/L(leg) & H/B/L for Player Attack and Defense\t",BWHT,LOW);
    int plWeapon = 15, plStamina = 100, oppWeapon = 10, oppStamina = 100;
    int x = playCombat(plWeapon, plStamina, oppWeapon, oppStamina);
    return x;
}

// MINIGAMES
