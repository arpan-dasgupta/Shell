#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>

#define clear() printf("\033[H\033[J")
char pinf[1000000];

char *trimwhitespace2(char *str)
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
void pinfo(int wf, char *pid)
{
    if (wf == 1)
    {
        pid = trimwhitespace2(pid);
        char ad1[100] = "/proc/", ad2[100] = "/proc/", ad3[100] = "/proc/";
        for (int i = 0; i < strlen(pid); i++)
        {
            ad1[i + 6] = pid[i];
            ad3[i + 6] = pid[i];
            ad2[i + 6] = pid[i];
        }
        {
            ad1[strlen(pid) + 6] = '/';
            ad1[strlen(pid) + 7] = 'e';
            ad1[strlen(pid) + 8] = 'x';
            ad1[strlen(pid) + 9] = 'e';
            ad1[strlen(pid) + 10] = '\0';
        }
        {
            ad2[strlen(pid) + 6] = '\0';
        }
        {
            ad3[strlen(pid) + 6] = '/';
            ad3[strlen(pid) + 7] = 's';
            ad3[strlen(pid) + 8] = 't';
            ad3[strlen(pid) + 9] = 'a';
            ad3[strlen(pid) + 10] = 't';
            ad3[strlen(pid) + 11] = 'u';
            ad3[strlen(pid) + 12] = 's';
            ad3[strlen(pid) + 13] = '\0';
        }
        char exe[1024], pidexe[1024];
        // char state[1024];
        int ret, ret2, ret3;

        ret = readlink(ad1, exe, sizeof(exe) - 1);
        // ret2 = readlink(ad2, pidexe, sizeof(pidexe) - 1);
        if (ret == -1)
        {
            fprintf(stderr, "ERROR\n");
            return;
        }
        exe[ret] = 0;
        pidexe[ret2] = 0;
        int x = 0;
        {
            FILE *status = fopen(ad3, "r");
            while (1)
            {
                char c = fgetc(status);
                if (feof(status))
                {
                    break;
                }
                pinf[x++] = c;
            }
            fclose(status);
        }
        pinf[x] = '\n';
        char *str1, *str2, *subtoken, *subtoken1, *subtoken2;
        char *token;
        char token2[1024];
        char *saveptr1, *saveptr2;
        int j, i;

        printf("PID: \t %s\n", pid);
        for (j = 1, str1 = pinf;; j++, str1 = NULL)
        {
            token = strtok_r(str1, "\n", &saveptr1);
            if (token == NULL)
                break;
            token = trimwhitespace2(token);
            // printf("%s\n", token);
            for (i = 0; i <= strlen(token); i++)
                token2[i] = token[i];
            subtoken = strtok_r(token, " \t", &saveptr2);
            subtoken = trimwhitespace2(subtoken);
            if (subtoken == NULL)
                continue;
            if (strcmp(subtoken, "VmSize:") == 0)
            {
                printf("%s\n", token2);
            }
            else if (strcmp(subtoken, "State:") == 0)
            {
                printf("%s\n", token2);
            }
        }
        printf("Executable path: %s\n", exe);
    }
    else
    {
        char exe[1024], pidexe[1024];
        // char state[1024];
        int ret, ret2, ret3;

        ret = readlink("/proc/self/exe", exe, sizeof(exe) - 1);
        ret2 = readlink("/proc/self", pidexe, sizeof(pidexe) - 1);
        if (ret == -1 || ret2 == -1)
        {
            fprintf(stderr, "ERROR\n");
            return;
        }
        exe[ret] = 0;
        pidexe[ret2] = 0;
        int x = 0;
        {
            FILE *status = fopen("/proc/self/status", "r");
            while (1)
            {
                char c = fgetc(status);
                if (feof(status))
                {
                    break;
                }
                pinf[x++] = c;
            }
            fclose(status);
        }
        pinf[x] = '\n';
        char *str1, *str2, *subtoken, *subtoken1, *subtoken2;
        char *token;
        char token2[1024];
        char *saveptr1, *saveptr2;
        int j, i;

        printf("PID: \t %s\n", pidexe);
        for (j = 1, str1 = pinf;; j++, str1 = NULL)
        {
            token = strtok_r(str1, "\n", &saveptr1);
            if (token == NULL)
                break;
            token = trimwhitespace2(token);
            // printf("%s\n", token);
            for (i = 0; i <= strlen(token); i++)
                token2[i] = token[i];
            subtoken = strtok_r(token, " \t", &saveptr2);
            subtoken = trimwhitespace2(subtoken);
            if (subtoken == NULL)
                continue;
            if (strcmp(subtoken, "VmSize:") == 0)
            {
                printf("%s\n", token2);
            }
            else if (strcmp(subtoken, "State:") == 0)
            {
                printf("%s\n", token2);
            }
        }
        printf("Executable path: %s\n", exe);
    }
}