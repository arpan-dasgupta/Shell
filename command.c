#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include "list.h"
#include "pwd.h"
#include "echo.h"
#include "cd.h"
#include "others.h"
#include "pinfo.h"
#include "history.h"
#include "nightswatch.h"
#include "setenv.h"

#define clear() printf("\033[H\033[J")

struct comm
{
    int pid;
    char pname[1000];
};

char *trimwhitespace(char *str)
{
    char *end;
    while (isspace((unsigned char)*str))
        str++;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;
    end[1] = '\0';
    return str;
}

struct comm *chooseCommand(char home[], char *str)
{
    char *str1, *str2, *subtoken, *subtoken1, *subtoken2;
    char *token;
    char *saveptr1, *saveptr2;
    int j, i, retcount = 0;
    int temp[1024];
    char *t2[1024];

    histAdd(home, str);
    for (j = 1, str1 = str;; j++, str1 = NULL)
    {
        token = strtok_r(str1, ";", &saveptr1);
        if (token == NULL)
            break;
        token = trimwhitespace(token);
        // printf("%d: %s\n", j, token);
        subtoken = strtok_r(token, " \t", &saveptr2);
        if (subtoken == NULL)
            continue;
        if (strcmp(subtoken, "pwd") == 0)
        {
            pwd();
            continue;
        }
        else if (strcmp(subtoken, "echo") == 0)
        {
            subtoken = strtok_r(NULL, "", &saveptr2);
            if (subtoken == NULL)
                subtoken = "";
            echo(subtoken);
            printf("\n");
            continue;
        }
        else if (strcmp(subtoken, "cd") == 0)
        {
            subtoken = strtok_r(NULL, " \t", &saveptr2);
            if (subtoken == NULL)
                subtoken = "";
            cd(home, subtoken);
            continue;
        }
        else if (strcmp(subtoken, "ls") == 0)
        {
            int l_flag = 0, a_flag = 0;
            char *dir = "";
            subtoken = strtok_r(NULL, " \t", &saveptr2);
            if (subtoken == NULL)
                subtoken = "";
            subtoken1 = strtok_r(NULL, " \t", &saveptr2);
            if (subtoken1 == NULL)
                subtoken1 = "";
            subtoken2 = strtok_r(NULL, " \t", &saveptr2);
            if (subtoken2 == NULL)
                subtoken2 = "";
            if (strlen(subtoken2) > 0)
                if (subtoken2[0] == '-')
                {
                    if (strlen(subtoken2) == 2)
                        if (subtoken2[1] == 'l')
                            l_flag = 1;
                        else
                            a_flag = 1;
                    else if (strlen(subtoken2) == 3)
                    {
                        l_flag = 1;
                        a_flag = 1;
                    }
                }
                else
                    dir = subtoken2;
            if (strlen(subtoken1) > 0)
                if (subtoken1[0] == '-')
                {
                    if (strlen(subtoken1) == 2)
                        if (subtoken1[1] == 'l')
                            l_flag = 1;
                        else
                            a_flag = 1;
                    else if (strlen(subtoken1) == 3)
                    {
                        l_flag = 1;
                        a_flag = 1;
                    }
                }
                else
                    dir = subtoken1;
            if (strlen(subtoken) > 0)
                if (subtoken[0] == '-')
                {
                    if (strlen(subtoken) == 2)
                        if (subtoken[1] == 'l')
                            l_flag = 1;
                        else
                            a_flag = 1;
                    else if (strlen(subtoken) == 3)
                    {
                        l_flag = 1;
                        a_flag = 1;
                    }
                }
                else
                    dir = subtoken;
            listfiles(home, dir, l_flag, a_flag);
        }
        else if (strcmp(subtoken, "exit") == 0 || strcmp(subtoken, "q") == 0 || strcmp(subtoken, "quit") == 0)
        {
            exit(0);
        }
        else if (strcmp(subtoken, "clear") == 0)
        {
            clear();
        }
        else if (strcmp(subtoken, "pinfo") == 0)
        {
            subtoken = strtok_r(NULL, " \t", &saveptr2);
            if (subtoken == NULL)
                pinfo(0, "");
            else
            {
                pinfo(1, trimwhitespace(subtoken));
            }
        }
        else if (strcmp(subtoken, "history") == 0)
        {
            subtoken = strtok_r(NULL, " \t", &saveptr2);
            if (subtoken == NULL)
                histDisp("10");
            else
            {
                histDisp(subtoken);
            }
        }
        else if (strcmp(subtoken, "setenv") == 0)
        {
            subtoken = strtok_r(NULL, " \t", &saveptr2);
            if (subtoken == NULL)
            {
                printf("Invalid args (1/2 required)\n");
            }
            else
            {
                subtoken1 = strtok_r(NULL, " \t", &saveptr2);
                subtoken2 = strtok_r(NULL, " \t", &saveptr2);
                if (subtoken2 != NULL)
                {
                    printf("Invalid args (1/2 required)\n");
                }
                else
                    setenviron(subtoken, subtoken1);
            }
        }
        else if (strcmp(subtoken, "unsetenv") == 0)
        {
            subtoken = strtok_r(NULL, " \t", &saveptr2);
            if (subtoken == NULL)
            {
                printf("Invalid args (1 required)\n");
            }
            else
            {
                subtoken1 = strtok_r(NULL, " \t", &saveptr2);
                if (subtoken1 != NULL)
                {
                    printf("Invalid args (1 required)\n");
                }
                else
                    unsetenviron(subtoken);
            }
        }
        else if (strcmp(subtoken, "nightswatch") == 0)
        {
            subtoken = strtok_r(NULL, " \t", &saveptr2);
            subtoken1 = strtok_r(NULL, " \t", &saveptr2);
            subtoken2 = strtok_r(NULL, " \t", &saveptr2);
            subtoken = trimwhitespace(subtoken);
            subtoken1 = trimwhitespace(subtoken1);
            subtoken2 = trimwhitespace(subtoken2);
            if (strcmp(subtoken, "-n") != 0)
            {
                printf("Invalid command\n");
            }
            if (strcmp(subtoken2, "interrupt") == 0)
                nightint(subtoken1);
            else if (strcmp(subtoken2, "dirty") == 0)
                nightdirt(subtoken1);
            else
            {
                printf("Invalid command\n");
            }
        }
        else
        {
            char *subt[100];
            subt[0] = subtoken;
            subt[0] = trimwhitespace(subt[0]);
            int i = 1, fl = 0;
            while (i < 100)
            {
                subt[i] = strtok_r(NULL, " \t", &saveptr2);
                if (subt[i] == NULL)
                    break;
                subt[i] = trimwhitespace(subt[i]);
                if (strcmp(subt[i], "&") == 0)
                {
                    fl = 1;
                    i--;
                }
                i++;
            }
            int x = otherCommands(subtoken, subt, fl);
            if (fl == 1 && x != -1)
            {
                t2[retcount] = subtoken;
                temp[retcount++] = x;
            }
        }
    }

    struct comm *ans = (struct comm *)malloc(sizeof(struct comm) * (retcount + 1));
    ans[0].pid = retcount;
    for (i = 0; i < retcount; i++)
    {
        ans[i + 1].pid = temp[i];
        // ans[i + 1].pname = t2[i];
        for (j = 0; j < strlen(t2[i]); j++)
            ans[i + 1].pname[j] = t2[i][j];
    }
    return ans;
}