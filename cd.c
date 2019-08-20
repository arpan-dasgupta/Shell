#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define clear() printf("\033[H\033[J")

void cd(char home[], char *str)
{
    if (strlen(str) == 0)
    {
        str = (char *)malloc(2);
        str[0] = '~';
        str[1] = '\0';
    }
    int i;
    if (str[0] == '~')
    {
        int sz = strlen(home) + strlen(str) - 1;
        char *temp = str;
        str = (char *)malloc(strlen(home) + strlen(str));
        for (i = sz - 1; i >= strlen(home); i--)
        {
            str[i] = temp[i - strlen(home) + 1];
        }
        for (i = 0; i < strlen(home); i++)
            str[i] = home[i];
        str[sz] = '\0';
    }
    int x = chdir(str);
    if (x == -1)
        perror("Error ");
}