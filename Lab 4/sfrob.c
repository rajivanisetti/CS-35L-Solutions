//
//  main.c
//  sfrob
//
//  Created by Rajiv Anisetti on 2/12/18.
//  Copyright © 2018 Rajiv Anisetti. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int frobcmp(char const *a, char const *b)
{
    int count = 0;
    while (a[count] != ' ' && b[count] != ' ')
    {
        char afrob = (a[count] ^ 42);
        char bfrob = (b[count] ^ 42);
        if (afrob < bfrob)
        {
            return -1;
        }
        else if (afrob > bfrob)
        {
            return 1;
        }
        else
        {
            count++;
        }
    }
    
    // PREFIX //
    
    if (a[count] == ' ' && b[count] != ' ')
    {
        return -1;
    }
    else if (a[count] != ' ' && b[count] == ' ')
    {
        return 1;
    }
    
    // EQUAL //
    
    return 0;
    
}

int compare(const void *x, const void* y)       // FOR QSORT //
{
    const char* a = *(const char**) x;
    const char* b = *(const char**) y;
    return frobcmp(a, b);
}

void printError()
{
    fprintf(stderr, "Could not allocate memory!");
    exit(1);
}

void checkStdin()
{
    if (ferror(stdin))
    {
        fprintf(stderr, "Error reading input");
        exit(1);
    }
}


int main(void)
{
    char **words = (char**) malloc(sizeof(char*));  // WORDS ARRAY //
    char *word = (char*) malloc(sizeof(char));      // CSTRING //
    
    if (words == NULL || word == NULL)
    {
        printError();
    }
    
    int sizeOfWords = 0;
    int sizeOfWord = 0;
    
    char previousCharacter = ' ';
    char currentCharacter = getchar();
    checkStdin();
    
    while (currentCharacter != EOF || !feof(stdin))
      {   
        if ((previousCharacter == ' ' || previousCharacter == '\n')  && (currentCharacter  == ' ' || currentCharacter == '\n'))   // IGNORE MULTIPLE SPACES
        {
            currentCharacter = getchar();
            checkStdin();
            previousCharacter = ' ';
            continue;
        }
        else
        {
            sizeOfWord++;
            word = (char*) realloc(word, sizeOfWord * sizeof(char));
            if (word == NULL)
            {
                printError();
            }
        }
        
        if (currentCharacter == ' ' || currentCharacter == '\n') // THEN PLACE WORD IN ARRAY OF WORDS //
        {
	    word[sizeOfWord - 1] = ' ';
            sizeOfWords++;
            words = (char**) realloc(words, sizeOfWords * sizeof(char*));
            if (words == NULL)
            {
                printError();
            }
            else
            {
                words[sizeOfWords - 1] = word;
            }
            
            sizeOfWord = 0;
            word = (char*) malloc(sizeof(char));
        }
        else    // APPEND TO WORD //
        {
	  //  printf("adding %c to word", currentCharacter);
            word[sizeOfWord - 1] = currentCharacter;
        }
        
        previousCharacter = currentCharacter;
          currentCharacter = getchar();
          checkStdin();
    }
    
    if (previousCharacter != ' ' && previousCharacter != '\n') // LAST WORD NOT GUARANTEED TO HAVE SPACE //
    {
        sizeOfWord++;   // ADD SPACE TO END //
        word = (char*) realloc(word, sizeOfWord * sizeof(char));
        if (word == NULL)
        {
            printError();
        }
        else
        {
            word[sizeOfWord - 1] = ' ';
        }
        
        sizeOfWords++;  // ADD WORD TO ARRAY OF WORDS //
        words = (char**) realloc(words, sizeOfWords * sizeof(char*));
        if (words == NULL)
        {
            printError();
        }
        else
        {
            words[sizeOfWords - 1] = word;
        }
    }
    
    qsort(words, sizeOfWords, sizeof(char*), compare);  // SORT //
    
    int count1 = 0;
    int count2 = 0;
    while (count1 < sizeOfWords)    // PRINT WORDS IN SORTED ORDER
    {
        while (words[count1][count2] != ' ')
        {
            putchar(words[count1][count2]);
            count2++;
        }
        putchar(' ');
        count1++;
        count2 = 0;
    }

    free(word);
    free(words);
    
}
