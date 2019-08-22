#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define clear() printf("\033[H\033[J")

int numcom = 0;
char hist[23][1024];

void histAdd(char *com)
{
    int i;
    if (numcom < 20)
        numcom++;
    for (i = numcom - 1; i >= 0; i--)
    {
        strcpy(hist[i + 1], hist[i]);
    }
    strcpy(hist[0], com);
}

void histDisp(char *num)
{
    int a = 1;
    if (strlen(num) >= 2)
    {
        a = 10;
    }
    else
    {
        a = num[0] - '0';
    }
    a = (a < numcom) ? a : numcom;
    for (int i = 0; i < a; i++)
    {
        printf("%s\n", hist[i]);
    }
}