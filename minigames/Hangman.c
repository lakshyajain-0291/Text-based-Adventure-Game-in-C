#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

int lives = 8, choice, l = 0; // l to remove slight bugs during displaying
int num_of_correct_guesses;

void intro();                                                                                               // Intro pages
int select_choice();                                                                                        // To select the category
int return_current_status(char guessed_string[], char *current_status[], char selected_word[], int length); // Updating the current status of the string
void print_current_status(char *current_status[], int length);                                              // Printing the current status of the string
void display_hangman(int result);                                                                           // Displaying the updated Hangman podium

int main()
{
  srand(time(0)); // Generating the seed to randomise the text to be predicted
  system("clear"); // To clear the terminal on Unix/Linux/Mac //Use: system("cls"); for Windows
  intro();
  char data[][500] = {
      "You couldn't live with your own failure, and where does that get you? Back to me",
      "The hardest choices require the strongest wills",
      "I am deadpool",
      "What is hate, if not love persevering",
      "I am iron man",
      "Whatever it takes",
      "Just because something works, doesn't mean it can't be improved" // Marvel Quotes
      "One small step for man, one giant leap for mankind",
      "Be the change that you wish to see in the world",
      "Live as if you were to die tomorrow. Learn as if you were to live forever",
      "The future depends on what you do today",
      "United we stand, divided we fall",
      "I'll be back",
      "Dream, dream, dream. Dreams transform into thoughts and thoughts result in action", // Famous quotes
      "Avengers",
      "Joker",
      "Interstellar",
      "Avatar",
      "The dark knight", // Famous movies
      "Introduction to computer science",
      "Introduction to electrical engineering",
      "Introduction to bioengineering",
      "Engineering realization",
      "Mathematics" // Graded sem 2 Courses
  };
  int n = 23;
  int selected_index;
  printf("\n\n");
  display_hangman(lives);
  for (int i = 0; i < 1000; i++)  //To add a delay
    rand();

  choice = select_choice(); // Selecting the category
  if (choice == 1)
    selected_index = rand() % 7;
  else if (choice == 2)
    selected_index = rand() % 7 + 7;
  else if (choice == 3)
    selected_index = rand() % 5 + 14;
  else if (choice == 4)
    selected_index = rand() % 5 + 19;
  else
  {
    printf("Wrong Choice\n");
    return 0;
  }

  char selected_word[500];
  int length = strlen(data[selected_index]); // length of the selected word
  strcpy(selected_word, data[selected_index]);

  int num, x = 0;
  char player1[500], player2[500];

  printf("\033[0;32m\n"); // Green colour
  printf("Enter the number of players: ");
  scanf("%d", &num);
  printf("\033[0m\n"); // Default colour
  int life[num];       // To store the lives of all the players

  while (x < num) // x = current player index
  {
    fgetc(stdin); // To Enter on the keyboard
    system("clear");

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
      printf("\n\nPlayer%d\n",(x+1));
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
        guessed_string[1] = 0;       // Zero can be inserted to a string to represent NULL/ otherwise a letter would be considered as a string

      if (strcmp("quit\n", guessed_string) == 0)
      {
        system("clear");

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
        system("clear");

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
        system("clear");
        printf("\033[0;31m\n");

        printf("WRONG GUESS\nYou lose a life!!");
        printf("\033[0m\n");
        lives--;
      }

      else if (status == 2) // Correctly guessed the String
      {
        system("clear");
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
        system("clear");

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
        while ((getchar()) != '\n'); // In order to get rid of those extra characters
        printf("\033[0m\n");
        system("clear");
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
  system("clear");

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

  printf("\n\n\n\nWelcome to Danie's Hangman Game\n");
  printf("Choose a Category to play the Game.\n1. Marvel Quotes\n2. Famous Quotes\n3. Famous Movies\n4. Graded Sem 2 Courses\n\n\n");
  printf("\033[0;32m\n");
  printf("Choice: ");
  int choice;
  scanf("%d", &choice);
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

  printf("\033[0;36m\n");
  printf("\n                       DANIE'S HANGMAN GAME   \n");
  printf("\033[0m\n");
  printf("\033[1;31m\033[0m\n"); // red
  printf("\033[0m\n");           // default

  printf("\n\n      __________________________________________________________\n");
  printf("      |                DANIE'S HANGMAN GAME                    |   \n");
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
  system("clear");
  printf("\033[0;36m\n");
  printf("\n                       DANIE'S HANGMAN GAME   \n");
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
  system("clear");
  printf("\033[0;36m\n");
  printf("\n                       DANIE'S HANGMAN GAME   \n");
  printf("\033[0m\n");

  return;
}