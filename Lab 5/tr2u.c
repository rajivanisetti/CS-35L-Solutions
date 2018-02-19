//
//  main.c
//  tr2u.c
//
//  Created by Rajiv Anisetti on 2/18/18.
//  Copyright © 2018 Rajiv Anisetti. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int checkDuplicateBytes(const char* file, unsigned long length);
int isInFrom(char* from, unsigned long len, char input, int* index);


int main(int argc, char * argv[])
{
    char *from, *to;
    
    if (argc != 3)
    {
        fprintf(stderr, "Incorrect number of arguments; please use 2 arguments!");
        exit(1);
    }
    
    from = argv[1];
    to = argv[2];
    
    if (strlen(from) != strlen(to))
    {
        fprintf(stderr, "Input files not of the same size!");
        exit(1);
    }
    
    if (checkDuplicateBytes(from, strlen(from)))
    {
        fprintf(stderr, "From contains duplicate bytes!");
        exit(1);
    }

    char curr[1];

    while (read(0, curr, 1)  > 0)
    {
        int index = 0;
        if (isInFrom(from, strlen(from), curr[0], &index))
        {
	  curr[0] = to[index]; 
        }
	
	write(1, curr, 1);
    }

    return 0;
}

int checkDuplicateBytes(const char* file, unsigned long length)
{
    int i;
    for (i = 0; i < length; i++)
    {
        int k;
        for (k = i + 1; k < length; k++)
        {
            if (file[i] == file[k])
                return 1;
        }
    }
    
    return 0;
}

int isInFrom(char* from, unsigned long len, char input, int* index)
{
    int i;
    for (i = 0; i < len; i++)
    {
        if (input == from[i])
        {
            *index = i;
            return 1;
        }
    }
    
    return 0;
}
