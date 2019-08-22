#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <ctype.h>
#include <termios.h>
// #include "conio.h"

char interinfo[100000];
char meminfo[100000];

int toInt(char *val)
{
    int ans = 0;
    for (int i = 0; i < strlen(val); i++)
    {
        ans = ans * 10 + (val[i] - '0');
    }
    return ans;
}

char *trimwhitespace3(char *str)
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

void enable_raw_mode()
{
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON | ECHO); // Disable echo as well
    tcsetattr(0, TCSANOW, &term);
}

void disable_raw_mode()
{
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &term);
}

int _kbhit()
{
    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);
    return byteswaiting > 0;
}

void nightint(char *time)
{
    int interval = toInt(time);
    struct timeval t1, t2;
    double elapsedTime;
    gettimeofday(&t1, NULL);
    {
        int x = 0;
        FILE *status = fopen("/proc/interrupts", "r");
        while (1)
        {
            char c = fgetc(status);
            if (feof(status))
            {
                break;
            }
            interinfo[x++] = c;
        }
        fclose(status);
        interinfo[x] = '\0';
        char *str1, *str2, *subtoken, *subtoken1, *subtoken2;
        char *token;
        char token2[1024];
        char *saveptr1, *saveptr2;
        int j, i;

        token = strtok_r(interinfo, "\n", &saveptr1);
        token = trimwhitespace3(token);
        // for (i = 0; i <= strlen(token); i++)
        //     token2[i] = token[i];
        // subtoken = strtok_r(token, " \t", &saveptr2);
        // subtoken = trimwhitespace3(subtoken);
        printf("\t %s\n", token);
    }
    while (1)
    {
        enable_raw_mode();
        if (!(_kbhit()))
        {
            disable_raw_mode();
            gettimeofday(&t2, NULL);
            elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
            elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;
            if (elapsedTime >= interval * 1000)
            {
                int x = 0;
                FILE *status = fopen("/proc/interrupts", "r");
                while (1)
                {
                    char c = fgetc(status);
                    if (feof(status))
                    {
                        break;
                    }
                    interinfo[x++] = c;
                }
                fclose(status);
                interinfo[x] = '\0';
                char *str1, *str2, *subtoken, *subtoken1, *subtoken2;
                char *token;
                char token2[1024];
                char *saveptr1, *saveptr2;
                int j, i;

                for (j = 1, str1 = interinfo;; j++, str1 = NULL)
                {
                    token = strtok_r(str1, "\n", &saveptr1);
                    if (token == NULL)
                        break;
                    token = trimwhitespace3(token);
                    // printf("%s\n", token);
                    for (i = 0; i <= strlen(token); i++)
                        token2[i] = token[i];
                    subtoken = strtok_r(token, " \t", &saveptr2);
                    subtoken = trimwhitespace3(subtoken);
                    if (subtoken == NULL)
                        continue;
                    if (strcmp(subtoken, "1:") == 0)
                    {
                        printf("%s\n", token2);
                    }
                }
                gettimeofday(&t1, NULL);
            }
        }
        else
        {
            disable_raw_mode();
            char i = getchar();
            if (i == 'q')
                break;
        }
    }
}

void nightdirt(char *time)
{
    int interval = toInt(time);
    struct timeval t1, t2;
    double elapsedTime;
    gettimeofday(&t1, NULL);
    while (1)
    {
        enable_raw_mode();
        if (!(_kbhit()))
        {
            disable_raw_mode();
            gettimeofday(&t2, NULL);
            elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
            elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;
            if (elapsedTime >= interval * 1000)
            {
                int x = 0;
                FILE *status = fopen("/proc/meminfo", "r");
                while (1)
                {
                    char c = fgetc(status);
                    if (feof(status))
                    {
                        break;
                    }
                    meminfo[x++] = c;
                }
                fclose(status);
                meminfo[x] = '\0';
                char *str1, *str2, *subtoken, *subtoken1, *subtoken2;
                char *token;
                char token2[1024];
                char *saveptr1, *saveptr2;
                int j, i;

                for (j = 1, str1 = meminfo;; j++, str1 = NULL)
                {
                    token = strtok_r(str1, "\n", &saveptr1);
                    if (token == NULL)
                        break;
                    token = trimwhitespace3(token);
                    // printf("%s\n", token);
                    for (i = 0; i <= strlen(token); i++)
                        token2[i] = token[i];
                    subtoken = strtok_r(token, " \t", &saveptr2);
                    subtoken = trimwhitespace3(subtoken);
                    if (subtoken == NULL)
                        continue;
                    if (strcmp(subtoken, "Dirty:") == 0)
                    {
                        printf("%s\n", token2);
                    }
                }
                gettimeofday(&t1, NULL);
            }
        }
        else
        {
            disable_raw_mode();
            char i = getchar();
            if (i == 'q')
                break;
        }
    }
}