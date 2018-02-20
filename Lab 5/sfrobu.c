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
#include <sys/stat.h>
#include <sys/types.h>

int flag = 0;

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


int main(int argc, const char *argv[])
{
    if (argc > 1)
    {
        if (argv[1] == "-f")
            flag = 1;
    }
    
    struct stat file;
    if (fstat(0, &file))
    {
        fprintf(stderr, "Could not read file!");
        exit(1);
    }
    
    char **sepWords;
    char *unseparatedWords;
    
    char curr = ' ';
    char prev = ' ';
    
    if (S_ISREG(file.st_mode))
    {
        long size = file.st_size;
        long i = 0;
        long numWords = 0;
        
        unseparatedWords = (char*)malloc(size);
        if (unseparatedWords == NULL)
        {
            fprintf(stderr, "Cannot allocate memory!");
            exit(1);
        }
        while (read(0, &curr, 1) > 0)
        {
            if (curr == ' ' && prev == ' ')  // skip duplicate spaces
            {
                continue;
            }
            
            unseparatedWords[i] = curr;
            i++;
            
            if (curr == ' ')
                numWords++;
            
            prev = curr;
            
        }
        
        if (prev != ' ')
        {
            unseparatedWords[i] = ' ';
            i++;
            numWords++;
        }
        
        int lengthOfWords = i;
        sepWords = (char**)malloc(numWords * sizeof(char*));
        if (sepWords == NULL)
        {
            fprintf(stderr, "Cannot allocate memory!");
            exit(1);
        }
        
        // ADD WORDS TO WORDS ARRAY //
        
        int w, k;
        int count = 0;
        for (k = 0; k < numWords; k++)
        {
            for (w = 0; unseparatedWords[count + w] != ' ' && (count + w) < lengthOfWords; w++)
            {
                sepWords[k][w] = unseparatedWords[count + w];
            }
            sepWords[k][w] = ' ';
            count = w + 1;
        }
    }
    else if (!S_ISREG(file.st_mode))    // IRREGULAR FILE //
    {
        int allocationSize = 0;   // allocate 0 bytes to start //
        unseparatedWords = (char) malloc(allocationSize);
        
        long i = 0;
        char curr = ' ';
        char prev = ' ';
        while (read(0, &curr, 1) > 0)
        {
            allocationSize++;
            unseparatedWords = (char)realloc(allocationSize);
            if (unseparatedWords == NULL)
            {
                fprintf(stderr, "Cannot allocate memory!");
                exit(1);
            }
            
            if (curr == ' ' && prev == ' ')  // skip duplicate spaces
            {
                continue;
            }
            
            unseparatedWords[i] = curr;
            i++;
            
            if (curr == ' ')
                numWords++;
            
            prev = curr;
        }
        
        if (prev != ' ')
        {
            allocationSize++;
            unseparatedWords = (char)realloc(allocationSize);
            if (unseparatedWords == NULL)
            {
                fprintf(stderr, "Cannot allocate memory!");
                exit(1);
            }
            
            unseparatedWords[i] = ' ';
            i++;
            numWords++;
        }
        
        int lengthOfWords = i;
        sepWords = (char**)malloc(numWords * sizeof(char*));
        if (sepWords == NULL)
        {
            fprintf(stderr, "Cannot allocate memory!");
            exit(1);
        }
        
        // ADD WORDS TO WORDS ARRAY //
        
        int w, k;
        int count = 0;
        for (k = 0; k < numWords; k++)
        {
            for (w = 0; unseparatedWords[count + w] != ' ' && (count + w) < lengthOfWords; w++)
            {
                sepWords[k][w] = unseparatedWords[count + w];
            }
            sepWords[k][w] = ' ';
            count = w + 1;
        }
        
    }

    
    qsort(sepWords, numWords, sizeof(char*), compare);  // SORT //
    
    int count1 = 0;
    int count2 = 0;
    while (count1 < sizeOfWords)    // PRINT WORDS IN SORTED ORDER
    {
        while (words[count1][count2] != ' ')
        {
            if (write(0, &words[count1][count2], 1) < 0)
            {
                fprintf(stderr, "Error writing!");
                exit(1);
            }
            count2++;
        }
        if (write(0, ' ', 1) < 0)
        {
            fprintf(stderr, "Error writing!");
            exit(1);
        }
        count1++;
        count2 = 0;
    }

    free(unseparatedWords);
    free(sepWords);
    
}
