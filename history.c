#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define clear() printf("\033[H\033[J")

int numcom = 20;
char hist[23][1024];

void comRead(char home[])
{
    for (int i = 0; i < 20; i++)
    {
        strcpy(hist[i], "null\n");
    }
    int cline = 0, cpoint = 0;
    char filpath[100], file[100] = "/.ash_history";
    int z = 0;
    for (int i = 0; i < strlen(home); i++)
        filpath[z++] = home[i];
    for (int i = 0; i < strlen(file); i++)
        filpath[z++] = file[i];
    filpath[z] = '\0';
    FILE *status = fopen(filpath, "ab+");
    while (1)
    {
        char c = fgetc(status);
        if (feof(status))
        {
            break;
        }
        hist[cline][cpoint++] = c;
        if (c == '\n')
        {
            hist[cline][cpoint] = '\0';
            cline++;
            cpoint = 0;
            if (cline == 20)
                break;
        }
    }
    fclose(status);
}

void comWrite(char home[])
{
    int cline = 0, cpoint = 0;
    char filpath[100], file[100] = "/.ash_history";
    int z = 0;
    for (int i = 0; i < strlen(home); i++)
        filpath[z++] = home[i];
    for (int i = 0; i < strlen(file); i++)
        filpath[z++] = file[i];
    filpath[z] = '\0';
    FILE *status = fopen(filpath, "w");
    while (1)
    {
        char c = hist[cline][cpoint];
        if (c != '\0')
            fputc(hist[cline][cpoint++], status);
        if (feof(status))
        {
            break;
        }
        if (c == '\0')
        {
            cline++;
            cpoint = 0;
            if (cline == 20)
                break;
        }
    }
    fclose(status);
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
        printf("%s", hist[i]);
    }
}

void histAdd(char home[], char *com)
{
    comRead(home);
    int i;
    if (numcom < 20)
        numcom++;
    for (i = numcom - 1; i >= 0; i--)
    {
        strcpy(hist[i + 1], hist[i]);
    }
    strcpy(hist[0], com);
    int sz = strlen(com);
    hist[0][sz] = '\n';
    hist[0][sz + 1] = '\0';
    comWrite(home);
}