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
#include <unistd.h>
#include <limits.h>
#include <ctype.h>

int flag = 0;
long numComparisons = 0;

int frobcmp(char const *a, char const *b)
{
    int count = 0;
    while (a[count] != ' ' && b[count] != ' ')
    {
      char afrob;
      char bfrob;
      if (flag)
	{
	  char aunfrob = (unsigned char)( a[count] ^ 42);
	  char bunfrob = (unsigned char)(b[count] ^ 42);
	  if (((aunfrob < 0 || aunfrob > UCHAR_MAX) && aunfrob != EOF) || ((aunfrob < 0 || aunfrob > UCHAR_MAX) && aunfrob != EOF))
	    {
	      fprintf(stderr, "Cannot unfrobnicate character!");
	      exit(1);
	    }
	  afrob = toupper(a[count] ^ 42);
	  bfrob = toupper(b[count] ^ 42);
	}
      else
	{
	  afrob = (a[count] ^ 42);
          bfrob = (b[count] ^ 42);
	}
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
    numComparisons = numComparisons + 1;
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

void sfrob(long sizeOfFile)
{
    char *unseparatedWords = (char*)malloc(sizeOfFile * sizeof(char));
    char **sepWords = (char**)malloc(sizeof(char*));
    
    if (unseparatedWords == NULL || sepWords == NULL)
    {
        fprintf(stderr, "Memory allocation failed!");
        exit(1);
    }
    
    long sizeOfUnsep = 0;
    long sizeOfSep = 0;
    long maxSize = sizeOfFile;
    
    char curr = ' ';
    char prev = ' ';
    
    while (read(0, &curr, 1) > 0)
    {
        if (curr == ' ' && prev == ' ')
        {
            continue;
        }
        else
        {
            sizeOfUnsep++;
            if (sizeOfUnsep >= maxSize)
            {
                maxSize++;
                unseparatedWords = (char*)realloc(unseparatedWords, maxSize * sizeof(char));
                if (unseparatedWords == NULL)
                {
                    fprintf(stderr, "Memory allocation failed!");
                    exit(1);
                }
            }
            unseparatedWords[sizeOfUnsep - 1] = curr;
        }
        
        prev = curr;
    }
    
        if (prev != ' ')
    {
        sizeOfUnsep++;
        if (sizeOfUnsep >= maxSize)
        {
            maxSize++;
            unseparatedWords = (char*)realloc(unseparatedWords, maxSize * sizeof(char));
            if (unseparatedWords == NULL)
            {
                fprintf(stderr, "Memory allocation failed!");
                exit(1);
            }
        }
        unseparatedWords[sizeOfUnsep - 1] = ' ';
	} 

    long i;
    for (i = 0; i < sizeOfUnsep; i++)
    {
        if (unseparatedWords[i] == ' ')
            sizeOfSep++;
    }
    

    int k;
    int start = 0;
    int sepcount = 0;
    for (k = 0; k < sizeOfUnsep; k++)
    {
        if (unseparatedWords[k] == ' ')
        {
            sepWords = (char**) realloc(sepWords, (sepcount + 1)  * sizeof(char*));
            if (sepWords == NULL)
            {
                printf("Could not allocate memory!");
                exit(1);
            }
            sepWords[sepcount] = unseparatedWords + start;
            sepcount++;
            start = k + 1;
        }
    }

   
    qsort(sepWords, sizeOfSep, sizeof(char*), compare);  // SORT //
    
    int count1 = 0;
    int count2 = 0;
    char space = ' ';
    while (count1 < sizeOfSep)    // PRINT WORDS IN SORTED ORDER
    {
        while (sepWords[count1][count2] != ' ')
        {
            if (write(1, &sepWords[count1][count2], 1) < 0)
            {
                fprintf(stderr, "Error writing!");
                exit(1);
            }
            count2++;
        }
        if (write(1, &space, 1) < 0)
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


int main(int argc, const char *argv[])
{
     if (argc > 1)
    {
        if (!strncmp(argv[1], "-f", 2))
	  {
            flag = 1;
	  }
    } 

    struct stat file;
    
    if (fstat(0, &file) < 0)
    {
        fprintf(stderr, "Could not read file!");
        exit(1);
    }
    
    if (S_ISREG(file.st_mode))
    {
        long size = file.st_size;
        sfrob(size);
    }
    else
    {
        sfrob(sizeof(char));
    }

    // printf("Number of Comparisons: %u\n", numComparisons);

}
