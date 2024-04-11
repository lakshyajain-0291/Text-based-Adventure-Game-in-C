#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_WORD_LENGTH 50

//Errors in UI: Box lines not forming correctly at few places due to unpredictable length of the word
void encryption(char *word, char *code, int shift)
{
    int i;
    int length = strlen(word);

    // Copy the original word to the code
    strcpy(code, word);

    // Encrypt the code using the given shift
    for (i = 0; i < length; i++)
    {
        if (code[i] >= 'A' && code[i] <= 'Z')
        {
            code[i] = ((code[i] - 'A' + shift + 26) % 26) + 'A'; // Ensure the result is non-negative
        }
        else if (code[i] >= 'a' && code[i] <= 'z')
        {
            code[i] = ((code[i] - 'a' + shift + 26) % 26) + 'a'; // Ensure the result is non-negative
        }
    }
}

int decryptor(int attempts)
{
    FILE *dictionary = fopen("dictionary.txt", "r");
    if (dictionary == NULL)
    {
        printf("Error: Unable to open dictionary file.\n");
        return 0;
    }

    
    FILE *temp=fopen("dictionary.txt","r"); //Counting the number of words in the dictionary
    char w[MAX_WORD_LENGTH];
    int x=0;
    while((fscanf(temp,"%s",w))!=EOF)
        x++;
    fclose(temp);

    char **words = malloc(x * sizeof(char *));
    if (words == NULL)
    {
        printf("Memory allocation failed.\n");
        fclose(dictionary);
        return 0;
    }

    for (int i = 0; i < x; i++)
    {
        words[i] = malloc(MAX_WORD_LENGTH * sizeof(char));
        if (words[i] == NULL)
        {
            printf("Memory allocation failed.\n");
            for (int j = 0; j < i; j++)
            {
                free(words[j]);
            }
            free(words);
            fclose(dictionary);
            return 0;
        }
    }

    int wordCount = 0;
    while (fgets(words[wordCount], MAX_WORD_LENGTH, dictionary))
    {
        words[wordCount][strlen(words[wordCount]) - 1] = '\0'; // Remove newline character
        wordCount++;
    }
    
    srand(time(NULL));
    int randomIndex = rand() % wordCount; // Generate a random index to select a word from the dictionary
    char *word = malloc(MAX_WORD_LENGTH * sizeof(char));
    if (word == NULL)
    {
        printf("Memory allocation failed.\n");
        for (int i = 0; i < wordCount; i++)
        {
            free(words[i]);
        }
        free(words);
        fclose(dictionary);
        return 0;
    }
    strcpy(word, words[randomIndex]);

    char *code = malloc(MAX_WORD_LENGTH * sizeof(char));
    if (code == NULL)
    {
        printf("Memory allocation failed.\n");
        free(word);
        for (int i = 0; i < wordCount; i++)
        {
            free(words[i]);
        }
        free(words);
        fclose(dictionary);
        return 0;
    }

    int shift = rand() % 27 - 13; // Generate a random shift value from -13 to 13
    encryption(word, code, shift);
    
    printf("_____________________________________________________________________________________\n");
    printf("|                                                                                   |\n");
    printf("|                                                                                   |\n");

    printf("|        Encrypted word: %-59s|\n", code);

    int attemptsLeft = attempts;
    char *decryptedWord = malloc(MAX_WORD_LENGTH * sizeof(char));
    if (decryptedWord == NULL)
    {
        printf("Memory allocation failed.\n");
        free(word);
        free(code);
        for (int i = 0; i < wordCount; i++)
        {
            free(words[i]);
        }
        free(words);
        fclose(dictionary);
        return 0;
    }

    while (attemptsLeft > 0)
    {
        printf("|        \x1b[33mAttempts left:\x1b[0m %-26d                                  |\n", attemptsLeft);
        printf("|        Enter your decryption attempt: ");
        scanf("%s", decryptedWord);
        int k=55-strlen(decryptedWord);
        
         


        if (strcmp(decryptedWord, words[randomIndex]) == 0)
        {
            fclose(dictionary);
            free(word);
            free(code);
            free(decryptedWord);
            for (int i = 0; i < wordCount; i++)
            {
                free(words[i]);
            }
            free(words);
            printf("|        Congratulations! You decrypted the word.                                   |\n");
            printf("|                                                                                   |\n");
            printf("|___________________________________________________________________________________|\n");
            return 1;
        }
        else
        {
            printf("|        \x1b[31mIncorrect attempt.\x1b[0m Here's a hint: ");

            // Change a random character in the code to match the original word
            int index = rand() % strlen(word);
            code[index] = word[index];
            int x=0;
            while(x!=strlen(word))
            {
                if(x==index)
                { 
                    printf("\x1b[32m%c\x1b[0m",code[x]);
                    x++;
                    continue;
                }
                printf("%c",code[x]);
                 x++;
            }
           printf("\n");
            attemptsLeft--;
        }
    }

    if (attemptsLeft == 0)
    {
        fclose(dictionary);
        free(word);
        free(code);
        free(decryptedWord);
        for (int i = 0; i < wordCount; i++)
        {
            free(words[i]);
        }
        free(words);
        printf("        You ran out of attempts. The correct word was: %s\n", words[randomIndex]);
        return 0;
    }
}

int main()
{
    system("clear");
    printf("\n        \x1b[33m");
    char welcome[100]="WELCOME TO THE EAVESDROP GAME";
    for(int i=0;i<strlen(welcome);i++)
    {
        putchar(welcome[i]);
        fflush(stdout);
        usleep(100000);
    }
    printf("\x1b[0m");
    printf("\n");
    int attempts = 5; // Number of attempts
    int resEavesdrop = decryptor(attempts);
    return 0;
}
