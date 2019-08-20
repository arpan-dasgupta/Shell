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

#define clear() printf("\033[H\033[J")

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

void chooseCommand(char home[], char *str)
{
    char *str1, *str2, *subtoken, *subtoken1, *subtoken2;
    char *token;
    char *saveptr1, *saveptr2;
    int j, i;

    for (j = 1, str1 = str;; j++, str1 = NULL)
    {
        token = strtok_r(str1, ";", &saveptr1);
        if (token == NULL)
            break;
        token = trimwhitespace(token);
        // printf("%d: %s\n", j, token);
        subtoken = strtok_r(token, " ", &saveptr2);
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
            subtoken = strtok_r(NULL, " ", &saveptr2);
            if (subtoken == NULL)
                subtoken = "";
            cd(home, subtoken);
            continue;
        }
        else if (strcmp(subtoken, "ls") == 0)
        {
            int l_flag = 0, a_flag = 0;
            char *dir = "";
            subtoken = strtok_r(NULL, " ", &saveptr2);
            if (subtoken == NULL)
                subtoken = "";
            subtoken1 = strtok_r(NULL, " ", &saveptr2);
            if (subtoken1 == NULL)
                subtoken1 = "";
            subtoken2 = strtok_r(NULL, " ", &saveptr2);
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
        else if (strcmp(subtoken, "exit") == 0)
        {
            exit(0);
        }
        else if (strcmp(subtoken, "clear") == 0)
        {
            clear();
        }
        // for (i = 0;i<)
        // for (str2 = token;; str2 = NULL)
        // {             );
        //     subtoken = strtok_r(str2, argv[3], &saveptr2);
        //     if (subtoken == NULL)
        //         break;
        //     printf(" --> %s\n", subtoken);
        // }
    }
}