#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_LENGTH 50

void encrypt(char *word, char *code, int shift)
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

    // Read words from dictionary file and count the number of words
    char **words = malloc(1000 * sizeof(char *));
    if (words == NULL)
    {
        printf("Memory allocation failed.\n");
        fclose(dictionary);
        return 0;
    }

    for (int i = 0; i < 1000; i++)
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
    encrypt(word, code, shift);

    printf("Encrypted word: %s\n", code);

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
        printf("Attempts left: %d\n", attemptsLeft);
        printf("Enter your decryption attempt: ");
        scanf("%s", decryptedWord);

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
            printf("Congratulations! You decrypted the word.\n");
            return 1;
        }
        else
        {
            printf("Incorrect attempt. Here's a hint: ");

            // Change a random character in the code to match the original word
            int index = rand() % strlen(word);
            code[index] = word[index];

            printf("%s\n", code);
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
        printf("You ran out of attempts. The correct word was: %s\n", words[randomIndex]);
        return 0;
    }
}

int main()
{
    int attempts = 5; // Number of attempts
    int resEavesdrop = decryptor(attempts);
    return 0;
}
