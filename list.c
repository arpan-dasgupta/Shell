#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <string.h>

extern int alphasort();

#define clear() printf("\033[H\033[J")

int file_select(const struct dirent *entry)
{
    int y = ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0)) ? 0 : 1;
    return y;
}

void listfiles(char home[], char *dir, char *flags)
{
    struct dirent **files;
    int count, i;
    // printf("%s\n", home);
    if (strlen(dir) == 0)
    {
        dir = (char *)malloc(2);
        dir[0] = '.';
        dir[1] = '\0';
    }
    // printf("%s -- %d\n", dir, strlen(home));
    if (dir[0] == '~')
    {
        int sz = strlen(home) + strlen(dir) - 1;
        char *temp = dir;
        dir = (char *)malloc(strlen(home) + strlen(dir));
        for (i = sz - 1; i >= strlen(home); i--)
        {
            dir[i] = temp[i - strlen(home) + 1];
            // printf("%d ", i - strlen(home) + 1);
        }
        for (i = 0; i < strlen(home); i++)
            dir[i] = home[i];
        dir[sz] = '\0';
    }
    // printf("%s ++ \n", dir);
    count = scandir(dir, &files, file_select, alphasort);
    if (count < 0)
        perror("Error ");
    printf("Number of files = %d\n", count);
    for (i = 1; i < count + 1; ++i)
        printf("%s\n", files[i - 1]->d_name);
    printf("\n"); /* flush buffer */
}