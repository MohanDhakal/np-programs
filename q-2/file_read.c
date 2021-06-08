
#include <stdio.h>
#include <stdlib.h>

int moduloAbyB(int a, int b)
{
    if (b == 0)
        return 0;

    return (a % b);
}

//send first argument as a registration number Id which will be 
//divided by 11 to find the line number
int main(int argc, char **argv)
{
    int lineNumber;
    FILE *fp;
    //to get the line detail
    char ch[100];
    //opening file in reading mode
    fp = fopen("users.txt", "r");

    //checking if file doesn't exits
    if (fp == NULL)
    {
        printf("no file exists");
        exit(0);
    }
    int count = 0;

    //fgets reading stops after encountering EOF or newline
    while (fgets(ch, sizeof(ch), fp) != NULL)
    {

        lineNumber = moduloAbyB(atoi(argv[1]), 11);
        // printf("%d", lineNumber);
        if (lineNumber == count)
        {
            fputs(ch, stdout);
            break;
        }
        count++;
    }
    printf("\n");
    fclose(fp);
}
